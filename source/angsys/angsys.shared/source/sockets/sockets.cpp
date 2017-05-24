#include "pch.h"
#include "ang/core/sockets.h"
#include "socket_manager.h"

using namespace ang;
using namespace ang::core;
using namespace ang::core::connectivity;


socket_manager::socket_manager()
	: _is_initialized(false)
{
	initialize();
}

socket_manager::~socket_manager()
{
	close();
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::connectivity::socket_manager, ang::object);

bool socket_manager::is_initialized()const
{
	return _is_initialized;
}

bool socket_manager::initialize()
{
	if (!_is_initialized)
		_is_initialized = (WSAStartup(MAKEWORD(2, 2), &_wsa_data) == 0);
	return _is_initialized;
}

bool socket_manager::close()
{
	if (!_is_initialized)
		return false;
	WSACleanup();
	_is_initialized = false;
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////

//
//read_event_args::read_event_args(client_t c, wsize_t l, wsize_t m)
//	: _client(c)
//	, _buffer_length(l)
//	, _max_block_buffer_length(m)
//{
//
//}
//
//read_event_args::~read_event_args()
//{
//
//}
//
////ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::sockets::read_event_args, core::delegates::event_args);
//
//client_t read_event_args::get_client()const
//{
//	return _client;
//}
//
//wsize_t read_event_args::get_buffer_length()const
//{
//	return _buffer_length;
//}
//
//wsize_t read_event_args::get_max_block_buffer_length()const
//{
//	return _max_block_buffer_length;
//}
//
//wsize_t read_event_args::read(array<byte_t>& data)
//{
//	if (data.is_empty() || data->is_empty())
//		return 0;
//
//	wsize_t to_read = min(min(_buffer_length, _max_block_buffer_length), data->size());
//
//	if (to_read == 0)
//		return 0;
//
//	_client->read(data);
//
//
//}
//
//wsize_t read_event_args::read_raw(wsize_t, byte_t*)
//{
//	return 0;
//}

///////////////////////////////////////////////////////////////////////////////////////////////////