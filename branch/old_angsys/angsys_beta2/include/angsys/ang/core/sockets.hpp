/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_SOCKETS_H__
#define __ANG_SOCKETS_H__

#include <angsys.h>
#include <ang/streams.h>
#include <ang/core/async.h>

#ifdef LINK
#undef LINK
#endif

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGSYS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

namespace ang
{
	namespace core
	{
		namespace connectivity
		{
			class client;
			class server;
			typedef wsize socket_handle_t;
			typedef object_wrapper<client> client_t;
			typedef object_wrapper<server> server_t;
		}
	}

	namespace core
	{
		namespace connectivity
		{
			class LINK client
				: public object
			{
				friend server;

			protected:
				socket_handle_t handle;

			public:
				client();
				client(string port, string device);
				client(socket_handle_t);

				ANG_DECLARE_INTERFACE();

			public:
				bool connect(string port, string device);
				bool disconnect();

				core::async::iasync_t<bool> read_async(core::delegates::function<bool(streams::itext_input_stream_t)>);
				core::async::iasync_t<bool> read_async(core::delegates::function<bool(streams::ibinary_input_stream_t)>);

				core::async::iasync_t<bool> write_async(core::delegates::function<bool(streams::itext_output_stream_t)>);
				core::async::iasync_t<bool> write_async(core::delegates::function<bool(streams::ibinary_output_stream_t)>);

			protected:
				bool attach(socket_handle_t handle);
				bool write(string)const;
				bool write(array<byte>)const;
				bool read(array<byte>&, bool wait = false)const;

			private:

				virtual~client();
			};

		}
	}

	namespace core
	{
		namespace connectivity
		{
			class LINK server
				: public object
			{
			protected:
				socket_handle_t handle;
				async::cond_t cond;
				async::mutex_t mutex;
				wsize max_client_counter;
				collections::vector<client_t> clients;
				async::thread_t streaming_thread;
				async::thread_t listening_thread;
				async::iasync_t<int> streaming_task;
				async::iasync_t<int> listening_task;

			public:
				server();
				ANG_DECLARE_INTERFACE();

			public:
				delegates::listener<void(client_t, array<byte>)> data_recieved_event;
				bool connect(string port, wsize max_clients);
				int shutdown();

			private:
				int initialize(string port);
				client_t accept_client();
				client_t get_client(windex)const;

			private:
				int on_listening_task(async::iasync<int>*, var_args_t);
				int on_streaming_task(async::iasync<int>*, var_args_t);

			private:
				virtual~server();
			};

		}
	}
}

#ifdef LINK
#undef LINK
#endif

#endif//__ANG_SOCKETS_H__
