
#pragma once
#include <ang/core/sockets.h>
 
namespace ang
{
	namespace core
	{
		namespace connectivity
		{
			class win_sock_lib
				: public smart<win_sock_lib>
			{
			private:
				static win_sock_lib* instance_manager(win_sock_lib* ptr, bool write = false);

			public:
				static win_sock_lib* instance();

				static bool is_instanced();

			private:
				WSADATA wsaData;

			private:
				win_sock_lib();
				virtual~win_sock_lib();
			};

		}
	}
 }
