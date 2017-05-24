#pragma once
#include "angsys.h"
#include "ang/core/sockets.h"

namespace ang
{
	namespace core
	{
		namespace connectivity
		{
			class socket_manager;
			typedef object_wrapper<socket_manager> socket_manager_t;

			class socket_manager
				: public object
				, public singleton<socket_manager_t>
			{
			private:
				friend singleton<socket_manager_t>;
#ifdef WINDOWS_PLATFORM
			
				bool _is_initialized;
				WSADATA _wsa_data;
#endif
				socket_manager();
				virtual~socket_manager();
			public:
				ANG_DECLARE_INTERFACE();

				bool is_initialized()const;
				bool initialize();
				bool close();		
			};

		}
	}



	namespace streams
	{
		class socket_text_output_stream;
		typedef object_wrapper<socket_text_output_stream> socket_text_output_stream_t;

		class socket_text_output_stream
			: public object
			, public itext_output_stream
		{
		private:
			core::connectivity::socket_handle_t _socket;
			text::encoding_t _format;
			bool _is_formating_enabled;
			text::text_format_t char_format;
			text::text_format_t string_format;
			text::text_format_t signed_format;
			text::text_format_t unsigned_format;
			text::text_format_t floating_format;

		public:
			socket_text_output_stream(core::connectivity::socket_handle_t);

		public: //overrides
			ANG_DECLARE_INTERFACE();

			virtual bool write(pointer, wsize)override;
			virtual bool write(wchar)override;
			virtual bool write(char)override;
			virtual bool write(byte)override;
			virtual bool write(short)override;
			virtual bool write(ushort)override;
			virtual bool write(int)override;
			virtual bool write(uint)override;
			virtual bool write(long)override;
			virtual bool write(ulong)override;
			virtual bool write(long64)override;
			virtual bool write(ulong64)override;
			virtual bool write(float)override;
			virtual bool write(double)override;
			virtual bool write(cstr_t)override;
			virtual bool write(cwstr_t)override;
			virtual bool write(cmstr_t)override;
			virtual bool write_line(cstr_t)override;
			virtual bool write_line(cwstr_t)override;
			virtual bool write_line(cmstr_t)override;

			virtual bool command(special_command_t) override;
			virtual stream_index_t position()const override;
			virtual stream_size_t stream_size()const override;
			virtual bool move_to(stream_index_t size, stream_reference_t ref)override;

			virtual text::encoding_t text_encoding()const override;
			virtual void enable_text_format(bool)override;
			virtual bool is_text_format_enabled()const override;
			virtual void text_format(cstr_t)override;
			virtual void text_format(text::text_format_t)override;

		private:
			virtual~socket_text_output_stream();
		};
	}
}