#include "pch.h"
#include "ang/core/sockets.h"
#include "socket_manager.h"

using namespace ang;
using namespace ang::core;
using namespace ang::core::connectivity;



client::client()
	: handle(INVALID_SOCKET)
{

}

client::client(string port, string device)
	: handle(INVALID_SOCKET)
{
	connect(port, device);
}

client::client(socket_handle_t h)
	: handle(INVALID_SOCKET)
{
	attach(h);
}

client::~client()
{
	disconnect();
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::connectivity::client, ang::object);

bool client::connect(string port, string device)
{
	socket_manager_t manager = socket_manager::instance();
	if (!manager->is_initialized())
		return true;

	int nret = 0;
	addrinfo hints, *result = NULL;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	nret = getaddrinfo((cstr_t)device, (cstr_t)port, &hints, &result);
	if (nret != 0)
	{
		return false;
	}

	handle = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (handle == INVALID_SOCKET)
	{
		nret = WSAGetLastError();
		freeaddrinfo(result);
		return false;
	}

	nret = ::connect(handle, result->ai_addr, (int)result->ai_addrlen);
	if (nret == SOCKET_ERROR) {
		closesocket(handle);
		handle = INVALID_SOCKET;
		return false;
	}

	freeaddrinfo(result);
	manager->add_ref();

	array<byte> message;
	if (read(message))
	{
		printf("server: %s\n", message->data());
	}
	

	return true;
}

bool client::attach(socket_handle_t h)
{
	if (h == INVALID_SOCKET)
		return false;
	socket_manager_t manager = socket_manager::instance();
	handle = h;
	manager->add_ref();
	return true;
}

bool client::disconnect()
{
	if (handle == INVALID_SOCKET)
		return false;
	socket_manager_t manager = socket_manager::instance();
	::shutdown(handle, SD_SEND);
	closesocket(handle);
	handle = INVALID_SOCKET;
	manager->release();
	return true;
}


core::async::iasync_t<bool> client::read_async(core::delegates::function<bool(streams::itext_input_stream_t)> callback)
{

}

core::async::iasync_t<bool> client::read_async(core::delegates::function<bool(streams::ibinary_input_stream_t)> callback)
{

}

core::async::iasync_t<bool> client::write_async(core::delegates::function<bool(streams::itext_output_stream_t)> callback)
{
	return core::async::async_task<bool>::create_task([&](async::iasync<bool>*, var_args_t args)->bool
	{
		streams::socket_text_output_stream_t stream = new streams::socket_text_output_stream(handle);
		return callback(stream.get());
	}, { callback });
}

core::async::iasync_t<bool> client::write_async(core::delegates::function<bool(streams::ibinary_output_stream_t)> callback)
{

}



#define MAX_DATA_SIZE 1048576 // 1MB

bool client::write(string data)const
{
	if (handle == INVALID_SOCKET || data.is_empty())
		return false;
	wsize size = data->length();
	if (size > 0)
	{
		wsize res, total_send = 0;
		send(handle, (wchar*)&size, sizeof(size), 0);
		send(handle, (wchar*)data->cstr(), size, 0);
	}
	return  true;
}

bool client::write(array<byte> data)const
{
	if (handle == INVALID_SOCKET || data.is_empty())
		return false;
	wsize size = data->size();
	if (size > 0)
	{
		wsize res = 0, total_send = 0;
		send(handle, (char_t*)&size, sizeof(size), 0);
		while (size > total_send)
		{
			res += send(handle, (char_t*)&data[total_send], min(MAX_DATA_SIZE, (size - total_send)), 0);
			if (res == SOCKET_ERROR)
				return false;
			total_send += res;
		}
	}
	return  true;
}

bool client::read(array<byte>& out, bool wait)const
{
	if (handle == INVALID_SOCKET)
		return false;
	wsize size = 0;
	int len = recv(handle, (char_t*)&size, sizeof(size), (!wait ? MSG_PEEK : 0));
	if (len > 0 && size > 0)
	{
		if (out.is_empty() || out->size() <= size)
			out = new storage::array_data<byte_t>(size + 1);
		if (!wait)recv(handle, (char_t*)&size, sizeof(size), 0);
		recv(handle, (char_t*)&out[0], (int_t)size, 0);
		out[size] = 0;
		return true;
	}
	return false;
}