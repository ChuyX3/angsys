#ifndef __ANG_CORE_SOCKETS_H__
#define __ANG_CORE_SOCKETS_H__

#include <angsys.h>
#include <ang/streams.h>
#include <ang/core/async.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

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
#else
#define LINK
#endif


namespace ang
{
	namespace core
	{
		namespace connectivity
		{
			ang_object(client);
			ang_object(server);
			using socket_t = wsize;

			class LINK server
				: public smart<server, async::iaction<void>>
			{
			private:
				friend client;
				objptr m_library;
				socket_t m_handle;
				vector<client_t> m_clients;

				mutable async::cond_t m_cond;
				mutable async::mutex_t m_mutex;
				mutable async::async_action_status_t m_state;
				async::thread_t m_async_worker;

				listen_token<void(void)> m_listen_token;
				listen_token<void(void)> m_stream_token;
			
				listener<void(server_t, client_t)> m_client_connected_event;
				static listen_token<void(server_t, client_t)> add_client_connected_event(base_event* prop, function<void(server_t, client_t)> func) {
					return field_to_parent(&server::client_connected_event, prop)->m_client_connected_event += func;
				}
				static bool remove_client_connected_event(base_event* prop, listen_token<void(server_t, client_t)> token) {
					return field_to_parent(&server::client_connected_event, prop)->m_client_connected_event -= token;
				}

			public:
				server();

				ANG_DECLARE_INTERFACE();

			private: //overrides
				virtual void dispose()override;

			public: //overrides
				virtual bool wait(async::async_action_status_t)const override;
				virtual bool wait(async::async_action_status_t, dword)const override;	
				virtual async::async_action_status_t status()const override;
				virtual bool cancel()override;
				virtual void result()const override;

			public:
				bool is_connected()const;
				bool has_thread_access()const;
				async::iasync<bool> connect(astring port);
				async::iasync<bool> shutdown();

				event<void(server_t, client_t), 
					add_client_connected_event,
					remove_client_connected_event> client_connected_event;

			private:
				bool shutdown_sync(async::iasync<bool> = null);
				client_t accept_client();
				void listening();
				void streaming();

				template<typename T> async::iasync<T> create_task(function<T(async::iasync<T>)> action) {
					return m_async_worker->run_async<T>(action);
				}


			private:
				
				virtual~server();
			};

			class LINK client
				: public smart<client, async::iaction<void>>
			{
			private:
				friend server;
				objptr m_library;
				socket_t m_handle;
				weak_ptr<server> m_server;
				mutable async::cond_t m_cond;
				mutable async::mutex_t m_mutex;
				async::thread_t m_async_worker;
				mutable async::async_action_status_t m_state;
				
				listener<void(array<byte>)> m_data_recived_event;
				static listen_token<void(array<byte>)> add_data_recived_event(base_event* prop, function<void(array<byte>)> func) {
					return field_to_parent(&client::data_recived_event, prop)->m_data_recived_event += func;
				}
				static bool remove_data_recived_event(base_event* prop, listen_token<void(array<byte>)> token) {
					return field_to_parent(&client::data_recived_event, prop)->m_data_recived_event -= token;
				}


			public:
				client();
				client(server_t, socket_t);
				client(astring port, astring device);

				ANG_DECLARE_INTERFACE();

			public: //overrides
				virtual void dispose()override;
				virtual bool wait(async::async_action_status_t)const override;
				virtual bool wait(async::async_action_status_t, dword)const override;
				virtual async::async_action_status_t status()const override;
				virtual bool cancel()override;
				virtual void result()const override;
				bool has_thread_access()const;

			public:
				event<void(array<byte>),
					add_data_recived_event,
					remove_data_recived_event> data_recived_event;


				bool is_connected()const;
				bool attach(server_t, socket_t handle);
				async::iasync<bool> connect(astring port, astring device);
				async::iasync<bool> disconnect();

				array<byte> read(bool wait = false);
				wsize write(array_view<byte>);

				async::iasync<array<byte>> read_async(void);
				async::iasync<wsize> write_async(array<byte>);
				async::iasync<wsize> write_async(array_view<byte>);

			
			private:
				bool disconnect_sync(async::iasync<bool> = null);
				void read_all();
				void listening();
				
			
			private:
				virtual~client();
			};
		}
	}
}

#ifdef  LINK
#undef  LINK
#endif//LINK


#endif//__ANG_CORE_SOCKETS_H__