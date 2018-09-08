#ifndef __ANG_STREAMS_H__
#define __ANG_STREAMS_H__

#include <angsys.h>

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
	namespace streams
	{
		typedef long64 stream_size_t;
		typedef long64 stream_index_t;

		ang_interface(istream);
		ang_interface(iinput_stream);
		ang_interface(ioutput_stream);

		ang_interface(itext_input_stream);
		ang_interface(itext_output_stream);
		ang_interface(ibinary_input_stream);
		ang_interface(ibinary_output_stream);

		ang_object(text_buffer_input_stream);
		ang_object(text_buffer_output_stream);
		ang_object(binary_buffer_input_stream);
		ang_object(binary_buffer_output_stream);

		safe_enum(LINK, stream_mode, byte)
		{
			unknow,
			in,
			out,
			inout
		};

		safe_enum(LINK, stream_reference, byte)
		{
			begin,
			current,
			end,
		};

		safe_enum(LINK, special_command, byte)
		{
			none,
			end_line,
			space,
			tab,
			back_space,
			insert,
			del,
			home,
			end,
			log_none,
			log_info,
			log_warning,
			log_error
		};

		typedef special_command_t  ssc, ssc_t;


		ang_begin_interface(LINK iinput_stream)
			visible vcall text::encoding_t format()const pure;
			visible vcall stream_index_t cursor()const pure;
			visible vcall stream_size_t size()const pure;
			visible vcall bool is_eos()const pure;
			visible vcall bool cursor(stream_index_t size, stream_reference_t ref)pure;
		ang_end_interface();

		ang_begin_interface(LINK ioutput_stream)
			visible vcall text::encoding_t format()const pure;
			visible vcall stream_index_t cursor()const pure;
			visible vcall stream_size_t size()const pure;
			visible vcall bool cursor(stream_index_t size, stream_reference_t ref)pure;
		ang_end_interface();
	
		ang_begin_interface(LINK istream)
			visible vcall text::encoding_t format()const pure;
			visible vcall stream_index_t position()const pure;
			visible vcall stream_size_t size()const pure;
			visible vcall bool is_eos()const pure;
			visible vcall bool cursor(stream_index_t size, stream_reference_t ref)pure;
		ang_end_interface();

		
		ang_begin_interface(LINK itext_input_stream, public iinput_stream)
			visible vcall wsize read_format(raw_cstr_t format, var_args_t&)pure;
			visible vcall wsize read(ibuffer_view_t, text::encoding_t, wsize max)pure;
			visible vcall wsize read_line(ibuffer_view_t, text::encoding_t, wsize max, array_view<const char32_t> = U"\n\r")pure;
			template<typename C, text::encoding E> wsize read_format(str_view<C, E> format, var_args_t& va) {
				return read_format(raw_cstr(format), ang::forward<var_args_t&>(va));
			}
			template<typename C, text::encoding E, typename...Ts> wsize read_format(str_view<C, E> format, Ts&...);
		ang_end_interface();


		ang_begin_interface(LINK itext_output_stream, public ioutput_stream)
			visible vcall bool command(special_command_t) pure;
			visible vcall wsize write(raw_cstr_t)pure;
			visible vcall wsize write_line(raw_cstr_t)pure;
			visible vcall wsize write_format(raw_cstr_t, var_args_t)pure;
			template<typename T> wsize write(T const&);
			template<typename C, text::encoding E, typename...Ts> wsize write_format(str_view<C, E> format, Ts... args) {
				return write_format(raw_cstr(format), var_args_t{ ang::forward<Ts>(args)... });
			}
		ang_end_interface();


		ang_begin_interface(LINK ibinary_input_stream, public iinput_stream)
			visible vcall wsize read(pointer, wsize)pure;
			template<typename T> inline wsize read(T& value);
		ang_end_interface();


		ang_begin_interface(LINK ibinary_output_stream, public ioutput_stream)
			visible vcall wsize write(pointer, wsize)pure;
			template<typename T> inline wsize write(T const& value);
		ang_end_interface();
	}
}


namespace ang
{
	namespace streams
	{
		template<typename C, text::encoding E, typename...Ts> struct read_format_helper;

		template<typename C, text::encoding E, typename T, typename...Ts>
		struct read_format_helper<C,E,T,Ts...>
		{
			static wsize read_format(itext_input_stream_t stream, str_view<C, E> format, var_args_t& va, T& a, Ts&... as) {
				auto readed = read_format_helper<C, E, Ts...>::read_format(format, va, ang::forward<Ts&>(as)...);
				objptr obj;
				va->pop(obj, true);
				 try { a = obj.as<T>(); }
				catch(...){ a = T(); 	}
				return readed;
			}
		};

		template<typename C, text::encoding E>
		struct read_format_helper<C,E>
		{
			static wsize read_format(itext_input_stream_t stream, str_view<C, E> format, var_args_t& va) {
				return stream->read_format(format, va);
			}
		};

		template<typename C, text::encoding E, typename...Ts> 
		wsize itext_input_stream::read_format(str_view<C, E> format, Ts&... args) {
			var_args_t va = new var_args();
			return read_format_helper<C, E, Ts...>::read_format(this, format, va, args...);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename T> struct write_text_helper
		{
			static wsize write_text(itext_output_stream_t stream, T const& val) {
				return stream->write(var(val));
			}
		};

		template<typename T, text::encoding E>
		struct write_text_helper<str_view<T,E>>
		{
			static wsize write_text(itext_output_stream_t stream, str_view<T, E> const& val) {
				stream->write(raw_cstr(val));
			}
		};

		template<text::encoding E, template<typename> class A>
		struct write_text_helper<strings::basic_string<E,A>>
		{
			static wsize write_text(itext_output_stream_t stream, strings::basic_string<E, A> const& val) {
				return val.is_empty() ? 0 : stream->write(raw_cstr(val->cstr()));
			}
		};

		template<typename T>
		struct write_text_helper<object_wrapper<T>>
		{
			static wsize write_text(itext_output_stream_t stream, object_wrapper<T> const& val) {
				return val.is_empty() ? 0 : stream->write(val->to_string());
			}
		};

		template<>
		struct write_text_helper<var>
		{
			static wsize write_text(itext_output_stream_t stream, var const& val) {
				return val.is_empty() ? 0 : stream->write(val->to_string());
			}
		};
		
		template<typename T>
		wsize itext_output_stream::write(T const& val) {
			return write_text_helper<T>::write_text(this, val);
		}


	}
}



#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANG_STREAMS_H__