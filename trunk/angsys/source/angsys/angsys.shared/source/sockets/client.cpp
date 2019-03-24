#include "pch.h"
#include <ang/core/sockets.h>
#include <ang/core/timer.h>
#include "socket_library.h"

using namespace ang;
using namespace ang::core;
using namespace ang::core::connectivity;

client::client()
	: m_handle(INVALID_SOCKET)
	, m_state(async::async_action_status::initializing)
{
}

client::client(server_t server, socket_t handle)
	: m_handle(INVALID_SOCKET)
	, m_state(async::async_action_status::initializing)
{
	attach(server, handle);
}

client::client(astring port, astring device)
	: m_handle(INVALID_SOCKET)
	, m_state(async::async_action_status::initializing)
{
	connect(port, device);
}

client::~client()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::connectivity::client);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::connectivity::client, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::connectivity::client, object);

void client::dispose()
{
	disconnect_sync();
	m_data_recived_event.clear();
}

bool client::wait(async::async_action_status_t state)const
{
	if (has_thread_access())
		return false;
	async::scope_locker<async::mutex_t> lock = m_mutex;
	if (m_state > state)return false;
	m_cond.waitfor(m_mutex, [&]() { return !(state & m_state); });
	return true;
}

bool client::wait(async::async_action_status_t state, dword ms)const
{
	if (has_thread_access())
		return false;

	dword last_time = (dword)(time::get_performance_time_us() / 1000.0);
	dword current = 0;

	async::scope_locker<async::mutex_t> lock = m_mutex;
	if (m_state > state)return false;
	while (!(state & m_state))
	{
		m_cond.wait(m_mutex, ms);
		async::thread::sleep(1);
		current = (dword)(time::get_performance_time_us() / 1000);
		if (ms <= (current - last_time))
			break;
		else ms -= (current - last_time);
		last_time = current;
	}
	return state & m_state;
}

async::async_action_status_t client::status()const
{
	return m_state;
}

bool client::cancel()
{
	async::scope_locker<async::mutex_t> lock = m_mutex;
	if (m_state & (async::async_action_status::finished | async::async_action_status::attached))
		return false;

	m_state = async::async_action_status::canceled;
	m_cond.signal();

	return true;
}

void client::result()const
{
	async::scope_locker<async::mutex_t> lock = m_mutex;
	if (!has_thread_access()) m_cond.waitfor(m_mutex, [&]() ->bool
	{
		return m_state <= async::async_action_status::running;//  !(async_action_status::finished | async_action_status::wait_for_then).is_active(state);
	});
}

bool client::has_thread_access()const
{
	return m_async_worker.is_empty() ? true : m_async_worker->has_thread_access();
}

bool client::is_connected()const
{
	return m_handle != INVALID_SOCKET;
}

async::iasync<bool> client::connect(astring port, astring device)
{
	async::scope_locker<async::mutex_t> lock = m_mutex;
	if (is_connected())
		return null;

	m_library = win_sock_lib::instance();

	if (m_library.is_empty())
		return null;

	m_async_worker = async::thread::create_thread();
	m_state = async::async_action_status::wait_for_start;
	m_cond.signal();

	//client_t auto_save = this;
	return m_async_worker->run_async<bool>([=](async::iasync<bool> task)->bool
	{
		async::scope_locker<async::mutex_t> lock = m_mutex;

		if (m_state & async::async_action_status::finished)
		{
			disconnect_sync();
			return false;
		}	

		int nret = 0;
		addrinfo hints, *result = NULL;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		nret = getaddrinfo((castr_t)device, (castr_t)port, &hints, &result);
		if (nret != 0)
		{
			disconnect_sync();
			return false;
		}

		m_handle = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (m_handle == INVALID_SOCKET)
		{
			nret = WSAGetLastError();
			freeaddrinfo(result);
			disconnect_sync();
			return false;
		}

		nret = ::connect(m_handle, result->ai_addr, (int)result->ai_addrlen);
		if (nret == SOCKET_ERROR) {
			closesocket(m_handle);
			m_handle = INVALID_SOCKET;
			disconnect_sync();
			return false;
		}

		freeaddrinfo(result);

		m_state = async::async_action_status::running;
		m_cond.signal();
		m_async_worker->idle_event += bind(this, &client::listening);

		return true;
	});

}

bool client::attach(server_t server, socket_t h)
{
	async::scope_locker<async::mutex_t> lock = m_mutex;
	if (h == INVALID_SOCKET)
		return false;
	m_library = win_sock_lib::instance();
	m_state = async::async_action_status::attached;
	m_cond.signal();
	m_handle = h;
	m_server = server;
	return true;
}

async::iasync<bool> client::disconnect()
{
	async::scope_locker<async::mutex_t> lock = m_mutex;
	if (!is_connected())
		return null;
	return m_async_worker->run_async<bool>(
		bind(this, &client::disconnect_sync));
}

bool client::disconnect_sync(async::iasync<bool>)
{
	async::scope_locker<async::mutex_t> lock = m_mutex;
	if (!m_async_worker.is_empty())
	{
		m_async_worker->exit();
		m_async_worker = null;
	}

	if (is_connected())
	{
		::shutdown(m_handle, SD_SEND);
		closesocket(m_handle);
		m_handle = INVALID_SOCKET;
		m_server = null;
		m_library = null;
	}

	m_state = async::async_action_status::completed;
	m_cond.signal();

	return true;
}

void client::read_all()
{
	while (is_connected())
	{
		auto data = client::read(false);
		if (data.is_empty())
			break;
		m_data_recived_event(data);
	}
}

void client::listening()
{
	if (m_state & async::async_action_status::finished)
	{
		disconnect();
		return;
	}
	auto data = client::read(false);
	if (!data.is_empty())
	{
		m_data_recived_event(data);
	}
}

array<byte> client::read(bool wait)
{
	if (!is_connected() || !has_thread_access())
		return null;
	wsize size = 0;

	int len = recv(m_handle, (char*)&size, sizeof(size), (!wait ? MSG_PEEK : 0));
	if (len > 0 && size > 0)
	{	
		if (!wait)
			recv(m_handle, (char*)&size, sizeof(size), 0);

		array<byte> out(size);
		recv(m_handle, (char*)&out[0], (int)size, 0);
		return out.get();
	}
	return null;
}

wsize client::write(array_view<byte> data)
{
	static constexpr wsize MAX_DATA_SIZE = 0X400;

	if (!is_connected() || !has_thread_access())
		return 0;

	wsize size = data.size();
	if (size > 0)
	{
		wsize res = 0, total_send = 0;
		send(m_handle, (char*)&size, (int)size_of(size), 0);
		while (size > total_send)
		{
			res += send(m_handle, (char*)&data[total_send], (int)min(MAX_DATA_SIZE, (size - total_send)), 0);
			if (res == SOCKET_ERROR)
				return total_send;
			total_send += res;
		}
		return total_send;
	}
	return 0;
}

async::iasync<array<byte>> client::read_async(void)
{
	if (!is_connected())
		return null;

	auto action = [=](async::iasync<array<byte>> task)->array<byte>
	{
		wsize size = 0;
		int len = recv(m_handle, (char*)&size, sizeof(size), 0);
		if (len > 0 && size > 0)
		{
			array<byte> out(size);
			recv(m_handle, (char*)&out[0], (int)size, 0);
			return out.get();
		}
		return null;
	};

	if(!m_async_worker.is_empty())
		return m_async_worker->run_async<array<byte>>(action);

	if (m_server.is_valid())
		return m_server.lock()->create_task<array<byte>>(action);

	return null;
}

async::iasync<wsize> client::write_async(array<byte> data)
{
	static constexpr wsize MAX_DATA_SIZE = 0X400;

	if (!is_connected())
		return null;

	auto action = [=](async::iasync<wsize> task)->wsize
	{
		wsize size = data->size();
		if (size > 0)
		{
			wsize res = 0, total_send = 0;
			send(m_handle, (char*)&size, (int)size_of(size), 0);
			while (size > total_send)
			{
				res += send(m_handle, (char*)&data[total_send], (int)min(MAX_DATA_SIZE, (size - total_send)), 0);
				if (res == SOCKET_ERROR)
					return total_send;
				total_send += res;
			}
			return total_send;
		}
		return 0;
	};

	if (!m_async_worker.is_empty())
		return m_async_worker->run_async<wsize>(action);

	if (m_server.is_valid())
		return m_server.lock()->create_task<wsize>(action);

	return null;
}

async::iasync<wsize> client::write_async(array_view<byte> data)
{
	return write_async(array<byte>(data)); //makes a data copy
}

