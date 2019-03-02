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

		
		ang_begin_interface(LINK itext_input_stream, iinput_stream)
			visible vcall uint seek(text::raw_cstr_t format)pure;
			visible vcall uint read_format(text::raw_cstr_t format, var_args_t&)pure;
			visible vcall wsize read(text::istring_t, wsize, wsize*written = null)pure;
			visible vcall wsize read(text::unknown_str_t, wsize sz, text::encoding_t, wsize*written = null)pure;
			visible vcall wsize read_line(text::istring_t, array_view<const char32_t> = U"\n\r", wsize*written = null)pure;
			visible vcall wsize read_line(text::unknown_str_t, wsize, text::encoding_t, array_view<const char32_t> = U"\n\r", wsize*written = null)pure;
			template<typename C, text::encoding E> uint read_format(str_view<C, E> format, var_args_t& va) {
				return read_format(raw_cstr(format), ang::forward<var_args_t&>(va));
			}
			template<typename C, text::encoding E, typename...Ts> uint read_format(str_view<C, E> format, Ts&...);
		ang_end_interface();


		ang_begin_interface(LINK itext_output_stream, ioutput_stream)
			visible vcall bool command(special_command_t) pure;
			visible vcall wsize write(text::raw_cstr_t)pure;
			visible vcall wsize write_line(text::raw_cstr_t)pure;
			visible vcall wsize write_format(text::raw_cstr_t, var_args_t)pure;
			template<typename T> wsize write(T const&);
			template<typename C, text::encoding E, typename...Ts> wsize write_format(str_view<C, E> format, Ts... args) {
				return write_format(raw_cstr(format), var_args_t{ ang::forward<Ts>(args)... });
			}
		ang_end_interface();


		ang_begin_interface(LINK ibinary_input_stream, iinput_stream)
			visible vcall wsize read(pointer, wsize)pure;
			template<typename T> inline wsize read(T& value);
		ang_end_interface();


		ang_begin_interface(LINK ibinary_output_stream, ioutput_stream)
			visible vcall wsize write(pointer, wsize)pure;
			template<typename T> inline wsize write(T const& value);
		ang_end_interface();
	}
}


namespace ang
{
	namespace streams
	{
		template<typename S, typename C, text::encoding E, typename...Ts> struct read_format_helper;

		template<typename S, typename C, text::encoding E, typename T, typename...Ts>
		struct read_format_helper<S,C,E,T,Ts...>
		{
			static uint read_format(S stream, str_view<C, E> format, var_args_t& va, T& a, Ts&... as) {
				uint readed = 0;
				readed = read_format_helper<S, C, E, Ts...>::read_format(stream, format, va, ang::forward<Ts&>(as)...);
				var obj;
				va->pop(obj, true);
				 try { a = obj.as<T>(); }
				catch(...){ a = T(); 	}
				return readed;
			}
		};

		template<typename S, typename C, text::encoding E>
		struct read_format_helper<S,C,E>
		{
			static uint read_format(S stream, str_view<C, E> format, var_args_t& va) {
				return stream->read_format(format, va);
			}
		};

		template<typename C, text::encoding E, typename...Ts> 
		uint itext_input_stream::read_format(str_view<C, E> format, Ts&... args) {
			var_args_t va = new var_args();
			return read_format_helper<itext_input_stream_t, C, E, Ts...>::read_format(this, format, va, args...);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename S, typename T>
		struct write_text_helper
		{
			static wsize write_text(S stream, T const& val) {
				return stream->write(var(val));
			}
		};

		template<typename S, typename T, text::encoding E>
		struct write_text_helper<S, str_view<T,E>>
		{
			static wsize write_text(S stream, str_view<T, E> const& val) {
				return stream->write(text::raw_cstr(val));
			}
		};

		template<typename S, text::encoding E, template<typename> class A>
		struct write_text_helper<S, text::basic_string<E,A>>
		{
			static wsize write_text(S stream, text::basic_string<E, A> const& val) {
				return val.is_empty() ? 0 : stream->write(text::raw_cstr(val->cstr()));
			}
		};

		template<typename S, typename T>
		struct write_text_helper<S, object_wrapper<T>>
		{
			static wsize write_text(S stream, object_wrapper<T> const& val) {
				return val.is_empty() ? 0 : stream->write(val->to_string());
			}
		};

		template<typename S>
		struct write_text_helper<S, var>
		{
			static wsize write_text(S stream, var const& val) {
				return val.is_empty() ? 0 : stream->write(val->to_string());
			}
		};
		
		template<typename T>
		wsize itext_output_stream::write(T const& val) {
			return write_text_helper<itext_output_stream_t, T>::write_text(this, val);
		}

		template<typename T>
		itext_output_stream_t& operator << (itext_output_stream_t& stream, T const& val) {
			stream->write(val);
			return stream;
		}

	}
}



#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANG_STREAMS_H__