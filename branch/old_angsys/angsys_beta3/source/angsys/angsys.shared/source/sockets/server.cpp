#include "pch.h"
#include <ang/core/sockets.h>
#include <ang/core/timer.h>
#include "socket_library.h"

using namespace ang;
using namespace ang::core;
using namespace ang::core::connectivity;

win_sock_lib* win_sock_lib::instance_manager(win_sock_lib* ptr, bool write)
{
	static win_sock_lib* _instance = null;
	if (write) {
		_instance = ptr;
	}
	return _instance;
}

win_sock_lib* win_sock_lib::instance() 
{
	static async::mutex_t s_mutex;
	async::scope_locker<async::mutex_t> lock = s_mutex;

	if (!is_instanced()) {
		try { new win_sock_lib(); }
		catch (...) {}
	}
	return instance_manager(null, false);
}

bool win_sock_lib::is_instanced()
{
	return !!instance_manager(null, false);
}

win_sock_lib::win_sock_lib()
{
	int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0) {
		throw_exception(except_code::unsupported);
	}

	if (is_instanced())
		throw_exception(except_code::two_singleton);
	instance_manager(this, true);

}

win_sock_lib::~win_sock_lib()
{
	instance_manager(null, true);
	WSACleanup();
}

///////////////////////////////////////////////////////////////

server::server()
	: m_handle(INVALID_SOCKET)
	, m_state(async::async_action_status::initializing)
{
	m_library = win_sock_lib::instance();
	m_async_worker = async::thread::create_thread();
}

server::~server()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::connectivity::server);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::connectivity::server, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::connectivity::server, object);

void server::dispose()
{
	shutdown_sync();
	m_async_worker->exit();
	m_async_worker = nullptr;
	m_library = 0;
}

bool server::wait(async::async_action_status_t state)const
{
	if (has_thread_access())
		return false;
	async::scope_locker<async::mutex_t> lock = m_mutex;
	if (m_state > state)return false;
	m_cond.waitfor(m_mutex, [&]() { return !(state & m_state); });
	return true;
}

bool server::wait(async::async_action_status_t state, dword ms)const
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

async::async_action_status_t server::status()const
{
	return m_state;
}

bool server::cancel()
{
	async::scope_locker<async::mutex_t> lock = m_mutex;
	if (m_state & (async::async_action_status::finished))
		return false;

	m_state = async::async_action_status::canceled;
	m_cond.signal();

	return true;
}

void server::result()const
{
	async::scope_locker<async::mutex_t> lock = m_mutex;
	if (!has_thread_access()) m_cond.waitfor(m_mutex, [&]() ->bool
	{
		return m_state <= async::async_action_status::running;//  !(async_action_status::finished | async_action_status::wait_for_then).is_active(state);
	});
}

bool server::has_thread_access()const
{
	return m_async_worker->has_thread_access();
}

bool server::is_connected()const
{
	return m_handle != INVALID_SOCKET;
}

async::iasync<bool> server::connect(astring port)
{
	async::scope_locker<async::mutex_t> lock = m_mutex;
	if (is_connected())
		return null;

	if (m_library.is_empty())
		return null;

	m_state = async::async_action_status::wait_for_start;
	m_cond.signal();

	server_t auto_save = this;
	return m_async_worker->run_async<bool>([=](async::iasync<bool> task)->bool
	{
		async::scope_locker<async::mutex_t> lock = m_mutex;

		if (m_state & async::async_action_status::finished)
		{
			shutdown_sync();
			return false;
		}

		HRESULT nret = 0;
		addrinfo hints, *result = NULL;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		nret = getaddrinfo(NULL, (castr_t)port, &hints, &result);
		if (nret != 0)
		{
			shutdown_sync();
			return false;
		}

		m_handle = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (m_handle == INVALID_SOCKET)
		{
			nret = WSAGetLastError();
			freeaddrinfo(result);
			shutdown_sync();
			return false;
		}

		// Setup the TCP listening socket
		nret = bind(m_handle, result->ai_addr, (int)result->ai_addrlen);
		if (nret == SOCKET_ERROR)
		{
			nret = WSAGetLastError();
			freeaddrinfo(result);
			shutdown_sync();
			return false;
		}

		freeaddrinfo(result);

		nret = listen(m_handle, SOMAXCONN);
		if (nret == SOCKET_ERROR)
		{
			nret = WSAGetLastError();
			shutdown_sync();
			return false;
		}

		auto_save->m_listen_token = m_async_worker->idle_event += bind(this, &server::listening);
		auto_save->m_stream_token = m_async_worker->idle_event += bind(this, &server::streaming);

		return true;
		
	});	
}

async::iasync<bool> server::shutdown()
{
	async::scope_locker<async::mutex_t> lock = m_mutex;
	if (is_connected())
		return null;
	return m_async_worker->run_async<bool>(
		bind(this, &server::shutdown_sync));
}

bool server::shutdown_sync(async::iasync<bool>)
{
	async::scope_locker<async::mutex_t> lock = m_mutex;
	for (client_t & client : m_clients)
	{
		client->disconnect();
		client = null;
	}
	m_clients->clear();

	if(m_listen_token.is_valid())
		m_async_worker->idle_event -= m_listen_token;
	m_listen_token = listen_token<void(void)>();

	if(m_stream_token.is_valid())
		m_async_worker->idle_event -= m_stream_token;
	m_stream_token = listen_token<void(void)>();

	if(m_handle != SOCKET_ERROR)
		closesocket(m_handle);
	m_handle = SOCKET_ERROR;

	m_state = async::async_action_status::completed;
	m_cond.signal();

	return true;
}

client_t server::accept_client()
{
	SOCKET _client = SOCKET_ERROR;
	fd_set readset;
	FD_ZERO(&readset);
	FD_SET(m_handle, &readset);
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;

	if (select((int)m_handle, &readset, NULL, NULL, &timeout) == 1)
	{
		_client = accept(m_handle, NULL, NULL);
		if (_client != INVALID_SOCKET)
			return new client(this, _client);
	}
	return null;
}

void server::listening()
{
	async::scope_locker<async::mutex_t> lock = m_mutex;
	if (!is_connected())
		return;

	if (!!(m_state & async::async_action_status::finished))
	{
		shutdown();
		return;
	}

	client_t new_client = accept_client();
	if (!new_client.is_empty())
	{
		m_clients += new_client;
		new_client->write(array<byte>("Welcome to the server!\r\n"));
	}
}

void server::streaming()
{
	async::scope_locker<async::mutex_t> lock = m_mutex;
	if (!is_connected() || !!(m_state & async::async_action_status::finished))
		return;

	for (client_t client : m_clients)
		client->read_all();
}
