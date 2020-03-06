/*********************************************************************************************************************/
/*   File Name: ang/streams.h                                                                                        */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_STREAMS_H__
#define __ANG_STREAMS_H__

#include <angsys.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANG_DYNAMIC_LIBRARY

#ifdef ANG_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANG_EXPORTS
#else//#elif defined ANG_STATIC_LIBRARY
#define LINK
#endif//ANG_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif

#ifdef __cplusplus

namespace ang
{
	namespace streams
	{
		typedef long64 stream_size_t;
		typedef long64 stream_index_t;

		ang_declare_interface(istream);
		ang_declare_interface(iinput_stream);
		ang_declare_interface(ioutput_stream);

		ang_declare_interface(itext_input_stream);
		ang_declare_interface(itext_output_stream);
		ang_declare_interface(ibinary_input_stream);
		ang_declare_interface(ibinary_output_stream);

		ang_declare_object(string_input_stream);

		ang_declare_object(text_buffer_input_stream);
		ang_declare_object(text_buffer_output_stream);
		ang_declare_object(binary_buffer_input_stream);
		ang_declare_object(binary_buffer_output_stream);

		declare_enum(LINK, stream_mode, byte)
		{
			unknow,
			in,
			out,
			inout
		};

		declare_enum(LINK, stream_reference, byte)
		{
			begin,
			current,
			end,
		};

		declare_enum(LINK, special_command, byte)
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


		struct nvt LINK iinput_stream
			: intf<iinput_stream
			, iid("ang::streams::iinput_stream")>
		{
			virtual text::encoding_t format()const = 0;
			virtual stream_index_t cursor()const = 0;
			virtual stream_size_t size()const = 0;
			virtual bool is_eos()const = 0;
			virtual bool cursor(stream_index_t size, stream_reference_t ref)= 0;
		};

		struct nvt LINK ioutput_stream
			: intf<ioutput_stream
			, iid("ang::streams::ioutput_stream")>
		{
			virtual text::encoding_t format()const = 0;
			virtual stream_index_t cursor()const = 0;
			virtual stream_size_t size()const = 0;
			virtual bool cursor(stream_index_t size, stream_reference_t ref)= 0;
		};

		struct nvt LINK istream
			: intf<istream
			, iid("ang::streams::istream")>
		{
			virtual text::encoding_t format()const = 0;
			virtual stream_index_t position()const = 0;
			virtual stream_size_t size()const = 0;
			virtual bool is_eos()const = 0;
			virtual bool cursor(stream_index_t size, stream_reference_t ref)= 0;
		};

		struct nvt LINK itext_input_stream
			: intf<itext_input_stream
			, iid("ang::streams::itext_input_stream")
			, iinput_stream>
		{
			virtual wsize seek(cstr_t format)= 0;
			virtual wsize read(pointer, ang::rtti_t const&)= 0;
			virtual wsize read_format(cstr_t format, var_args_t&)= 0;
			virtual wsize read(string, wsize, wsize*written = null)= 0;
			virtual wsize read(pointer, wsize sz, text::encoding_t, wsize*written = null)= 0;
			virtual wsize read_line(string, array_view<const char32_t> = U"\n\r", wsize*written = null)= 0;
			virtual wsize read_line(pointer, wsize, text::encoding_t, array_view<const char32_t> = U"\n\r", wsize*written = null)= 0;
			template<typename T> wsize read(T&&);
			template<typename...Ts> wsize read_format(raw_cstr_t format, Ts&...);
		};
		
		struct nvt LINK itext_output_stream
			: intf<itext_output_stream
			, iid("ang::streams::itext_output_stream")
			, ioutput_stream>
		{
			virtual bool command(special_command_t) = 0;
			virtual wsize write(cstr_t)= 0;
			virtual wsize write_line(cstr_t)= 0;
			virtual wsize write_format(cstr_t, var_args_t)= 0;
			template<typename T> wsize write(T const&);
			template<typename...Ts> wsize write_format(raw_cstr_t format, Ts... args) {
				return write_format(format, var_args_t{ ang::forward<Ts>(args)... });
			}
		};

		struct nvt LINK ibinary_input_stream
			: intf<ibinary_input_stream
			, iid("ang::streams::ibinary_input_stream")
			, iinput_stream> {
			virtual wsize read(pointer, wsize)= 0;
			virtual wsize read(ibuffer_t)= 0;
			virtual wsize read(pointer, const rtti_t&)= 0;
			template<typename T> inline wsize read(T& value);
		};

		struct nvt LINK ibinary_output_stream
			: intf<ibinary_output_stream
			, iid("ang::streams::ibinary_output_stream")
			, ioutput_stream> {
			virtual wsize write(pointer, wsize)= 0;
			virtual wsize write(ibuffer_view_t)= 0;
			template<typename T> inline wsize write(T const& value);
		};

		struct nvt LINK itext_stream
			: intf<itext_stream
			, iid("ang::streams::itext_stream")
			, istream>
		{
			virtual wsize seek(cstr_t format)= 0;
			virtual wsize read(pointer, ang::rtti_t const&)= 0;
			virtual wsize read_format(cstr_t format, var_args_t&)= 0;
			virtual wsize read(string, wsize, wsize*written = null)= 0;
			virtual wsize read(pointer, wsize sz, text::encoding_t, wsize*written = null)= 0;
			virtual wsize read_line(string, array_view<const char32_t> = U"\n\r", wsize*written = null)= 0;
			virtual wsize read_line(pointer, wsize, text::encoding_t, array_view<const char32_t> = U"\n\r", wsize*written = null)= 0;

			virtual bool command(special_command_t) = 0;
			virtual wsize write(cstr_t)= 0;
			virtual wsize write_line(cstr_t)= 0;
			virtual wsize write_format(cstr_t, var_args_t)= 0;

			template<typename T> wsize write(T const&);
			template<typename...Ts> wsize write_format(raw_cstr_t format, Ts... args) {
				return write_format(format, var_args_t{ ang::forward<Ts>(args)... });
			}
			template<typename T> wsize read(T&&);
			template<typename...Ts> wsize read_format(raw_cstr_t format, Ts&...);
		};

		struct nvt LINK ibinary_stream
			: intf<ibinary_stream
			, iid("ang::streams::ibinary_stream")
			, istream>
		{
			virtual wsize write(pointer, wsize)= 0;
			virtual wsize write(ibuffer_view_t)= 0;
			virtual wsize read(pointer, wsize)= 0;
			virtual wsize read(ibuffer_t)= 0;
			virtual wsize read(pointer, const rtti_t&)= 0;
		};

	}
}

ANG_ENUM_DECLARATION(LINK, ang::streams::stream_mode);
ANG_ENUM_DECLARATION(LINK, ang::streams::stream_reference);
ANG_ENUM_DECLARATION(LINK, ang::streams::special_command);

namespace ang
{
	namespace streams
	{
		class LINK string_input_stream final
			: public implement<string_input_stream
			, iid("string_input_stream")
			, itext_input_stream
			, iinput_stream>
		{
		private:
			stream_index_t m_cursor;
			string m_string;

		public:
			string_input_stream(string);

			void attach(string);
			string data()const;

		public: //overrides	
			virtual void dispose()override;
			text::encoding_t format()const override;
			stream_index_t cursor()const override;
			stream_size_t size()const override;
			bool is_eos()const override;
			bool cursor(stream_index_t size, stream_reference_t ref)override;
			wsize seek(cstr_t format)override;
			wsize read(pointer ptr, ang::rtti_t const&)override;
			wsize read_format(cstr_t format, var_args_t&)override;
			wsize read(string, wsize, wsize*written = null)override;
			wsize read(pointer, wsize sz, text::encoding_t, wsize*written = null)override;
			wsize read_line(string, array_view<const char32_t> = U"\n\r", wsize*written = null)override;
			wsize read_line(pointer, wsize, text::encoding_t, array_view<const char32_t> = U"\n\r", wsize*written = null)override;

		private:
			virtual~string_input_stream();
		};



		class LINK string_output_stream final
			: public implement<string_output_stream
			, iid("string_output_stream")
			, itext_output_stream
			, ioutput_stream>
		{
		private:
			stream_index_t m_cursor;
			string m_string;

		public:
			string_output_stream(string);

			void attach(string);
			string data()const;

		public: //overrides	
			virtual void dispose()override;
			text::encoding_t format()const override;
			stream_index_t cursor()const override;
			stream_size_t size()const override;
			bool cursor(stream_index_t size, stream_reference_t ref)override;
			bool command(special_command_t) override;
			wsize write(cstr_t) override;
			wsize write_line(cstr_t) override;
			wsize write_format(cstr_t, var_args_t) override;

		private:
			virtual~string_output_stream();
		};
	}
}

namespace ang
{
	namespace streams
	{
		template<typename S, typename...Ts> struct read_format_helper;

		template<typename S, typename T, typename...Ts>
		struct read_format_helper<S, T, Ts...>
		{
			static wsize read_format(S stream, raw_cstr_t format, var_args_t& va, T& a, Ts&... as) {
				wsize readed = 0;
				va += default_value<T>::value;
				readed = read_format_helper<S, Ts...>::read_format(stream, format, va, as...);
				var v;
				va.pop_back(v);
				a = ang::move(v.get<T>());
				return readed;
			}
		};

		template<typename S>
		struct read_format_helper<S>
		{
			static wsize read_format(S stream, raw_cstr_t format, var_args_t& va) {
				return stream->read_format(format, va);
			}
		};

		template<typename S, typename T> struct read_text_helper {
			static wsize read(S, T const&) {
				static_assert(ang::is_lvalue_reference<T>::value, "can't writte to rvalue reference");
				return 0;
			}
		};

		template<typename S, typename T> struct read_text_helper<S, T&> {
			static wsize read(S stream, T& val) {
				return stream->read(pointer(&val), ang::type_of<T>());
			}
		};

		template<typename S, typename T, wsize N> struct read_text_helper<S, const T(&)[N]> {
			static wsize read(S stream, const T(&cstr)[N]) {
				return stream->seek(cstr);
			}
		};

		template<typename S> struct read_text_helper<S, str_t> {
			static wsize read(S stream, str_t const&  cstr) {
				return stream->seek(cstr);
			}
		};

		template<typename S> struct read_text_helper<S, str_t&> {
			static wsize read(S stream, str_t const& cstr) {
				return stream->seek(cstr);
			}
		};

		template<typename S> struct read_text_helper<S, cstr_t> {
			static wsize read(S stream, cstr_t const& cstr) {
				return stream->seek(cstr);
			}
		};

		template<typename S> struct read_text_helper<S, cstr_t&> {
			static wsize read(S stream, cstr_t const& cstr) {
				return stream->seek(cstr);
			}
		};

		template<typename S, typename T, text::encoding E> struct read_text_helper<S, str_view<T, E>> {
			static wsize read(S stream, str_view<T, E> const& cstr) {
				return stream->seek(cstr);
			}
		};

		template<typename S, typename T, text::encoding E> struct read_text_helper<S, str_view<T, E>&> {
			static wsize read(S stream, str_view<T, E> const& cstr) {
				return stream->seek(cstr);
			}
		};

		template<typename T>
		wsize itext_input_stream::read(T&& val) {
			return read_text_helper<itext_input_stream*, T>::read(this, ang::forward<T>(val));
		}

		template<typename...Ts>
		wsize itext_input_stream::read_format(raw_cstr_t format, Ts&... args) {
			var_args_t va = new var_args();
			return read_format_helper<itext_input_stream*, Ts...>::read_format(this, format, va, args...);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename S, typename T>
		struct write_text_helper
		{
			static wsize write_text(S stream, T const& val) {
				return stream->write(to_string(val));
			}
		};

		template<typename S, typename T, text::encoding E>
		struct write_text_helper<S, str_view<T, E>>
		{
			static wsize write_text(S stream, str_view<T, E> const& val) {
				return stream->write(cstr_t(val));
			}
		};

		template<typename S, text::encoding E, template<typename> class A>
		struct write_text_helper<S, text::fast_string<E, A>>
		{
			static wsize write_text(S stream, text::fast_string<E, A> const& val) {
				return val.is_empty() ? 0 : stream->write(cstr_t(val.cstr()));
			}
		};

		template<typename S, text::encoding E, template<typename> class A>
		struct write_text_helper<S, text::basic_string<E, A>>
		{
			static wsize write_text(S stream, text::basic_string<E, A> const& val) {
				return val.is_empty() ? 0 : stream->write(cstr_t(val->cstr()));
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

		template<typename S>
		struct write_text_helper<S, string>
		{
			static wsize write_text(S stream, string const& val) {
				return val.is_empty() ? 0 : stream->write((cstr_t)val);
			}
		};

		template<typename T>
		wsize itext_output_stream::write(T const& val) {
			return write_text_helper<itext_output_stream*, T>::write_text(this, val);
		}


		/////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename S, typename T>
		struct read_binary_helper
		{
			static wsize read(S stream, T& val) {
				return stream->read(&val, size_of<T>());
			}
		};

		template<typename T>
		wsize ibinary_input_stream::read(T& val) {
			return read_binary_helper<ibinary_input_stream_t, T>::read(this, val);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename T>
		itext_output_stream_t& operator << (itext_output_stream_t& stream, T const& val) {
			stream->write(val);
			return stream;
		}

		template<typename T>
		ibinary_output_stream_t& operator << (ibinary_output_stream_t& stream, T const& val) {
			stream->write(val);
			return stream;
		}

		template<typename T>
		itext_input_stream_t& operator >> (itext_input_stream_t& stream, T&& val) {
			stream->read(ang::forward<T>(val));
			return stream;
		}

		template<typename T>
		ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, T& val) {
			stream->read(val);
			return stream;
		}
	}
}


#endif // __cplusplus

#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANG_STREAMS_H__
