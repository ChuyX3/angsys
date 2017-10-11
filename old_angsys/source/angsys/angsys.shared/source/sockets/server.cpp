#include "pch.h"
#include "ang/core/sockets.h"
#include "socket_manager.h"

using namespace ang;
using namespace ang::core;
using namespace ang::core::connectivity;


ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::connectivity::server, ang::object);


server::server()
	: handle(0)
	, listening_task(nullptr)
	, streaming_task(nullptr)
{
	cond = new async::cond();
	mutex = new async::mutex();
}

server::~server()
{
	listening_task = nullptr;
	streaming_task = nullptr;
}

bool server::connect(string port, wsize max_clients)
{
	if (listening_task != nullptr)
		return false;
	
	(listening_task = async::async_task<int>::create_task(delegates::function<int(async::iasync<int>*, var_args_t)>(this, &server::on_listening_task),
	{ port }))->wait(ang::core::async::async_action_status::running, -1);

	return true;
}

int server::initialize(string port)
{
	socket_manager_t manager = socket_manager::instance();

	if (!manager->is_initialized())
		return -1;

	HRESULT nret = 0;
	addrinfo hints, *result = NULL;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	nret = getaddrinfo(NULL, (cstr_t)port, &hints, &result);
	if (nret != 0)
	{
		return nret;
	}

	handle = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (handle == INVALID_SOCKET)
	{
		nret = WSAGetLastError();
		freeaddrinfo(result);
		return nret;
	}

	// Setup the TCP listening socket
	nret = bind(handle, result->ai_addr, (int)result->ai_addrlen);
	if (nret == SOCKET_ERROR)
	{
		nret = WSAGetLastError();
		freeaddrinfo(result);
		closesocket(handle);
		return nret;
	}

	freeaddrinfo(result);

	nret = listen(handle, SOMAXCONN);
	if (nret == SOCKET_ERROR)
	{
		nret = WSAGetLastError();
		closesocket(handle);
		WSACleanup();
		return nret;
	}

	(streaming_task = async::async_task<int>::create_task(delegates::function<int(async::iasync<int>*, var_args_t)>(this, &server::on_streaming_task),
	{ null }))->wait(ang::core::async::async_action_status::running, -1);

	return 0;
}

int server::shutdown()
{
	if (listening_task == nullptr)
		return 0;
	listening_task->cancel();
	listening_task->wait(async::async_action_status::completed, -1);
	listening_task = nullptr;

	return 0;
}

client_t server::accept_client()
{
	SOCKET _client = SOCKET_ERROR;
	fd_set readset;
	FD_ZERO(&readset);
	FD_SET(handle, &readset);
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;

	if (select(handle, &readset, NULL, NULL, &timeout) == 1)
	{
		_client = accept(handle, NULL, NULL);
		if (_client != INVALID_SOCKET)
			return new client(_client);
	}
	return nullptr;
}

int server::on_listening_task(async::iasync<int>* _async, var_args_t args)
{
	int nret = initialize(args[0]->as<strings::string_buffer>());
	if (nret != 0)
		return nret;
	do
	{
		if (_async->status() == async::async_action_status::suspended)
			_async->wait(async::async_action_status::running, -1);
		async::scope_locker lock = mutex;
		client_t new_client = accept_client();
		if (!new_client.is_empty())
		{
			clients += new_client;
			new_client->write("Welcome to the server!\r\n");
		}

	} while (_async->status() != async::async_action_status::canceled);

	streaming_task->cancel();
	streaming_task->wait(async::async_action_status::completed, -1);
	streaming_task = null;

	if (!clients.is_empty() && !clients->is_empty()) {//scope
		async::scope_locker lock = mutex;
		clients = nullptr;
	}

	closesocket(handle);
	WSACleanup();
	return 0;
}

int server::on_streaming_task(async::iasync<int>* _async, var_args_t)
{
	while (_async->status() != async::async_action_status::canceled)
	{
		if (_async->status() == async::async_action_status::suspended)
			_async->wait(async::async_action_status::running, -1);

		if (clients.is_empty() || clients->size() == 0)
		{
			async::scope_locker lock = mutex;
			if (!cond->waitfor(mutex, 100, [&]()->bool { return(clients.is_empty() || clients->size() == 0); }))
				continue;
		}
		else
		{
			async::scope_locker lock = mutex;
			array<byte> data;
			for (auto it = clients->begin(); it.is_valid(); it++)
			{
				client_t client = *it;
				if (client->read(data, false))
					data_recieved_event(client, data);
			}
		}
	}
	return 0;
}


client_t server::get_client(windex idx)const
{
	async::scope_locker lock = mutex;
	if (clients.is_empty() || clients->size() <= idx)
		return nullptr;
	return clients[idx];
}