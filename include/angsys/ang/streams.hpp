/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_STREAMS_HPP__
#define __ANG_STREAMS_HPP__

#include <angsys.h>

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
	namespace streams
	{
		typedef long64 stream_size_t;
		typedef long64 stream_index_t;

		class text_buffer_input_stream;
		class text_buffer_output_stream;
		class binary_buffer_input_stream;
		class binary_buffer_output_stream;

		typedef object_wrapper<text_buffer_input_stream> text_buffer_input_stream_t;
		typedef object_wrapper<text_buffer_output_stream> text_buffer_output_stream_t;
		typedef object_wrapper<binary_buffer_input_stream> binary_buffer_input_stream_t;
		typedef object_wrapper<binary_buffer_output_stream> binary_buffer_output_stream_t;

		ANG_INTERFACE(istream);

		//struct iserializable;

		ANG_BEGIN_ENUM(LINK, stream_mode, byte)
			unknow,
			in,
			out,
			inout
		ANG_END_ENUM(stream_mode);

		ANG_BEGIN_ENUM(LINK, stream_reference, byte)
			begin,
			current,
			end,
		ANG_END_ENUM(stream_reference);

		ANG_BEGIN_ENUM(LINK, special_command, byte)
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
		ANG_END_ENUM(special_command);

		typedef special_command_t  ssc, ssc_t;

		ANG_BEGIN_INTERFACE(LINK, iinput_stream)
			visible vcall wsize read(pointer, wsize, text::text_format_t)pure;
			visible vcall text::encoding_t format()const pure;
			visible vcall stream_index_t position()const pure;
			visible vcall stream_size_t stream_size()const pure;
			visible vcall bool end_of_stream()const pure;
			visible vcall bool move_to(stream_index_t size, stream_reference_t ref)pure;
		ANG_END_INTERFACE();

		ANG_BEGIN_INTERFACE(LINK, ioutput_stream)
			visible vcall wsize write(pointer, wsize, text::text_format_t)pure;
			visible vcall text::encoding_t format()const pure;
			visible vcall stream_index_t position()const pure;
			visible vcall stream_size_t stream_size()const pure;
			visible vcall bool move_to(stream_index_t size, stream_reference_t ref)pure;
		ANG_END_INTERFACE();

		ANG_BEGIN_INTERFACE(LINK, istream)
			visible vcall wsize read(pointer, wsize, text::encoding_t = text::encoding::binary)pure;
			visible vcall wsize read(ibuffer_view_t, text::encoding_t = text::encoding::binary)pure;
			visible vcall wsize write(pointer, wsize, text::encoding_t = text::encoding::binary)pure;
			visible vcall wsize write(ibuffer_view_t, text::encoding_t = text::encoding::binary)pure;
			visible vcall wsize read(pointer, wsize, text::text_format_t)pure;
			visible vcall wsize write(pointer, wsize, text::text_format_t)pure;
			visible vcall text::encoding_t format()const pure;
			visible vcall stream_index_t position()const pure;
			visible vcall stream_size_t stream_size()const pure;
			visible vcall bool move_to(stream_index_t size, stream_reference_t ref)pure;
		ANG_END_INTERFACE();

		//////////////////////////////////////////////////////////////////////////////////

		
		ANG_BEGIN_INTERFACE_WITH_BASE(LINK, itext_input_stream, public iinput_stream)
			visible using iinput_stream::read;
			visible vcall wsize seek(raw_str_t)pure;
			visible vcall wsize read(ibuffer_view_t, text::encoding_t, wsize max)pure;
			visible vcall wsize read_line(ibuffer_view_t, text::encoding_t, wsize max, array_view<const char32_t>)pure;

			template<typename T> inline wsize read(T& value);

			template<text::encoding_enum ENCODING>
			inline wsize read(strings::string_base<ENCODING>& str, wsize max);	
			template<text::encoding_enum ENCODING>
			inline wsize read_line(strings::string_base<ENCODING>& str, wsize max, array_view<const char32_t> endline);
		ANG_END_INTERFACE();


		ANG_BEGIN_INTERFACE_WITH_BASE(LINK, itext_output_stream, public ioutput_stream)
			visible using ioutput_stream::write;
			visible vcall void enable_text_formating(bool)pure;
			visible vcall bool is_text_formating_enabled()const pure;
			visible vcall void text_formating(cstr_t)pure;
			visible vcall void text_formating(text::text_format_t)pure;
			visible vcall bool command(special_command_t) pure;
			visible vcall wsize write(raw_str_t)pure;
			visible vcall wsize write_line(raw_str_t)pure;

			template<typename T> inline wsize write(T const& value);
			template<typename T> inline wsize write(T const& value, text::text_format_t);

		ANG_END_INTERFACE();


		ANG_BEGIN_INTERFACE_WITH_BASE(LINK, ibinary_input_stream, public iinput_stream)
			using iinput_stream::read;
			visible vcall wsize read(pointer, wsize)pure;
			template<typename T> inline wsize read(T& value);
		ANG_END_INTERFACE();


		ANG_BEGIN_INTERFACE_WITH_BASE(LINK, ibinary_output_stream, public ioutput_stream)
			using ioutput_stream::write;
			visible vcall wsize write(pointer, wsize)pure;
			template<typename T> inline wsize write(T const& value);
		ANG_END_INTERFACE();


	}

	template<> class LINK intf_wrapper<streams::itext_input_stream>
	{
	public:
		typedef streams::itext_input_stream	type;
	protected:
		type* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(type*);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);
		~intf_wrapper();
	public:
		void clean();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

	public:
		intf_wrapper& operator = (type*);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);

		intf_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		operator type * (void);
		operator type const* (void)const;
	};

	template<> class LINK intf_wrapper<streams::itext_output_stream>
	{
	public:
		typedef streams::itext_output_stream type;
	protected:
		type* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(type*);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);
		~intf_wrapper();

	public:
		void clean();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

	public:
		intf_wrapper& operator = (type*);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);

		intf_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		operator type * (void);
		operator type const* (void)const;
	};

	template<> class LINK intf_wrapper<streams::ibinary_input_stream>
	{
	public:
		typedef streams::ibinary_input_stream type;
	protected:
		type* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(type*);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);
		~intf_wrapper();

	public:
		void clean();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

	public:
		intf_wrapper& operator = (type*);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);

		intf_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		operator type * (void);
		operator type const* (void)const;
	};

	template<> class LINK intf_wrapper<streams::ibinary_output_stream>
	{
	public:
		typedef streams::ibinary_output_stream type;
	protected:
		type* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(type*);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);
		~intf_wrapper();

	public:
		void clean();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

	public:
		intf_wrapper& operator = (type*);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);

		intf_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		operator type * (void);
		operator type const* (void)const;
	};

	namespace streams
	{
		template<typename T>
		struct text_serializer {
			static wsize serialize(itext_output_stream_t stream, T const& value, text::text_format_t format = text::default_text_format<T>::format()) {
				return stream->write((pointer)&value, sizeof(T), format);
			}
			static wsize serialize(istream_t stream, T const& value, text::text_format_t format = text::default_text_format<T>::format()) {
				return stream->write((pointer)&value, sizeof(T), format);
			}
		};

		template<typename T>
		struct text_deserializer {
			static wsize deserialize(itext_input_stream_t stream, T& value) {
				return stream->read((pointer)&value, sizeof(T), text::default_text_format<T>::format());
			}
			static wsize deserialize(istream_t stream, T& value) {
				return stream->read((pointer)&value, sizeof(T), text::default_text_format<T>::format());
			}
		};

		template<typename T>
		struct text_serializer<safe_str<T>> {
			static wsize serialize(itext_output_stream_t stream, safe_str<T> const& value) {
				return stream->write((pointer)&value, sizeof(T), text::default_text_format<T>::format());
			}
			static wsize serialize(istream_t stream, safe_str<T> const& value) {
				return stream->write((pointer)&value, sizeof(T), text::default_text_format<T>::format());
			}
		};

		template<typename T>
		struct text_serializer<object_wrapper<T>> {
			static wsize serialize(itext_output_stream_t stream, object_wrapper<T> const& value) {
				return value.is_empty() ? 0 : value->serialize(stream);
			}
			static wsize serialize(istream_t stream, object_wrapper<T> const& value) {
				return value.is_empty() ? 0 : value->serialize(stream);
			}
		};

		template<typename T>
		struct text_deserializer<safe_str<T>> {
			static wsize deserialize(itext_input_stream_t stream, safe_str<T>& str) {
				wsize max = min(str.size(), stream->stream_size() - stream->position());
				dummy_buffer buff(str.get(), str.size() * sizeof(T));
				auto c = stream->read(&buff, text::encoding_by_type<T>::encoding(), max);
				str.set(str.get(), c);
				return c;
			}
			static wsize deserialize(istream_t stream, safe_str<T>& str) {
				wsize max = min(str.size(), stream->stream_size() - stream->position());
				auto c = stream->read(str.get(), str.size(), text::encoding_by_type<T>::encoding());
				str.set(str.get(), c);
				return c;
			}
			static wsize deserialize_until(itext_input_stream_t stream, safe_str<T>& str, array_view<const char32_t> endline) {
				wsize max = min(str.size(), stream->stream_size() - stream->position());
				dummy_buffer buff(str.get(), str.size() * sizeof(T));
				auto c = stream->read_line(&buff, text::encoding_by_type<T>::encoding(), max, endline);
				str.set(str.get(), c);
				return c;
			}
		};

		template<text::encoding_enum ENCODING>
		struct text_deserializer<strings::string_base<ENCODING>> {
			static wsize deserialize(itext_input_stream_t stream, strings::string_base<ENCODING>& str, wsize max) {
				max = min(max, stream->stream_size() - stream->position());
				ibuffer_view_t buff = str->map_buffer(0, max);
				auto c = stream->read(buff, ENCODING, max);
				str->unmap_buffer(buff, c);
				return c;
			}
			static wsize deserialize(istream_t stream, strings::string_base<ENCODING>& str, wsize max) {
				max = min(max, stream->stream_size() - stream->position());
				ibuffer_view_t buff = str->map_buffer(0, max);
				auto c = stream->read(buff, max, ENCODING);
				str->unmap_buffer(buff, c);
				return c;
			}
			static wsize deserialize_until(itext_input_stream_t stream, strings::string_base<ENCODING>& str, wsize max, array_view<const char32_t> endline) {
				max = min(max, stream->stream_size() - stream->position());
				ibuffer_view_t buff = str->map_buffer(0, max);
				auto c = stream->read_line(buff, ENCODING, max, endline);
				str->unmap_buffer(buff, c);
				return c;
			}
		};


		template<typename T>
		struct binary_serializer {
			static wsize serialize(ibinary_output_stream_t stream, T const& value) {
				return stream->write((pointer)&value, sizeof(T));
			}
		};

		template<typename T>
		struct binary_deserializer {
			static wsize deserialize(ibinary_input_stream_t stream, T& value) {
				return stream->read(&value, sizeof(T));
			}
		};

		template<typename T>
		struct binary_serializer<object_wrapper<T>> {
			static wsize serialize(ibinary_output_stream_t stream, object_wrapper<T> const& value) {
				return value.is_empty() ? 0u : value->serialize(stream);
			}
		};

		template<typename T>
		inline wsize itext_input_stream::read(T& value) {
			return text_deserializer<T>::deserialize(this, value);
		}

		template<text::encoding_enum ENCODING>
		inline wsize itext_input_stream::read(strings::string_base<ENCODING>& str, wsize max) {
			return text_deserializer<strings::string_base<ENCODING>>::deserialize(this, str, max);
		}

		template<text::encoding_enum ENCODING>
		inline wsize itext_input_stream::read_line(strings::string_base<ENCODING>& str, wsize max, array_view<const char32_t> endline) {
			return text_deserializer<strings::string_base<ENCODING>>::deserialize(this, str, max, endline);
		}


		template<typename T>
		inline wsize itext_output_stream::write(T const& value) {
			return text_serializer<T>::serialize(this, value);
		}

		template<typename T>
		inline wsize itext_output_stream::write(T const& value, text::text_format_t format) {
			return text_serializer<T>::serialize(this, value, format);
		}

		template<typename T> inline wsize ibinary_input_stream::read(T& value) {
			return binary_deserializer<T>::deserialize(this, value);
		}

		template<typename T> inline wsize ibinary_output_stream::write(T const& value) {
			return binary_serializer<T>::serialize(this, value);
		}
	}


	namespace streams
	{
		template <typename T> inline itext_input_stream_t& operator >> (itext_input_stream_t& stream, T& value) { stream->read(value); return stream; }
		template <typename T> inline itext_input_stream_t& operator >> (itext_input_stream_t& stream, safe_str<T> value) { stream->seek(value); return stream; }
		template<text::encoding_enum ENCODING> inline itext_input_stream_t& operator >> (itext_input_stream_t& stream, strings::string_base<ENCODING>& str) { stream->read(str, -1); return stream; }
	
		//////////////////////////////////////////////////////////////////////////////////

		template <typename T> inline itext_output_stream_t& operator << (itext_output_stream_t& stream, T const& value) { stream->write(value); return stream; }
		inline itext_output_stream_t& operator << (itext_output_stream_t& stream, special_command_t value) { stream->command(value); return stream; }

		//////////////////////////////////////////////////////////////////////////////////

		template <typename T> inline ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, T& value) { stream->read(value); return stream; }

		//////////////////////////////////////////////////////////////////////////////////

		template <typename T> inline ibinary_output_stream_t& operator << (ibinary_output_stream_t& stream, T const& value) { stream->write(value); return stream; }

		//////////////////////////////////////////////////////////////////////////////////
	}
}

namespace ang
{
	namespace streams
	{

		class LINK binary_buffer_input_stream final
			: public object
			, public ibinary_input_stream
		{
		private:
			ibuffer_t _buffer;
			stream_index_t _cursor;

		public:
			binary_buffer_input_stream();
			binary_buffer_input_stream(binary_buffer_input_stream*);
			binary_buffer_input_stream(ibuffer_t);

		public: //overrides
			ANG_DECLARE_INTERFACE();

			virtual wsize read(pointer, wsize)override;
			virtual wsize read(pointer, wsize, text::text_format_t /*unused*/)override;
			//virtual wsize read(objptr&)override;

			virtual text::encoding_t format()const override;
			virtual stream_index_t position()const override;
			virtual stream_size_t stream_size()const override;
			virtual bool end_of_stream()const override;
			virtual bool move_to(stream_index_t size, stream_reference_t ref)override;

			bool is_valid()const;
			ibuffer* buffer()const;
			bool attach(ibuffer* buff);
			bool can_move_to(stream_index_t size, stream_reference_t ref);
			pointer pointer_at(stream_index_t);

		private:		
			bool forward(stream_index_t size);
			bool backward(stream_index_t size);
			bool can_forward(stream_index_t size);
			bool can_backward(stream_index_t size);

		private:
			virtual~binary_buffer_input_stream();
		};


		class LINK binary_buffer_output_stream final
			: public object
			, public ibinary_output_stream
		{
		private:
			ibuffer_t _buffer;
			stream_index_t _cursor;

		public:
			binary_buffer_output_stream();
			binary_buffer_output_stream(binary_buffer_output_stream*);
			binary_buffer_output_stream(ibuffer_t);

		public: //overrides
			ANG_DECLARE_INTERFACE();

			virtual wsize write(pointer, wsize)override;
			virtual wsize write(pointer, wsize, text::text_format_t /*unused*/)override;
			//virtual wsize write(objptr)override;

			virtual text::encoding_t format()const override;
			virtual stream_index_t position()const override;
			virtual stream_size_t stream_size()const override;
			virtual bool move_to(stream_index_t size, stream_reference_t ref)override;

			bool is_valid()const;
			ibuffer* buffer()const;
			bool attach(ibuffer* buff);
			bool can_move_to(stream_index_t size, stream_reference_t ref);

		private:

			pointer pointer_at(stream_index_t);
			bool forward(stream_index_t size);
			bool backward(stream_index_t size);
			bool can_forward(stream_index_t size);
			bool can_backward(stream_index_t size);

		private:
			virtual~binary_buffer_output_stream();
		};


		class LINK text_buffer_input_stream final
			: public object
			, public itext_input_stream
		{
		private:
			text::itext_buffer_t _buffer;
			stream_index_t _cursor;
			text::encoder_interface encoder;

		public:
			text_buffer_input_stream();
			text_buffer_input_stream(text_buffer_input_stream const*);
			text_buffer_input_stream(text::itext_buffer_t);

		public: //overrides
			ANG_DECLARE_INTERFACE();

			using itext_input_stream::read;
			using itext_input_stream::read_line;


			virtual wsize seek(raw_str_t)override;
			virtual wsize read(pointer, wsize, text::text_format_t)override;
			virtual wsize read(ibuffer_view_t, text::encoding_t, wsize max)override;
			virtual wsize read_line(ibuffer_view_t, text::encoding_t, wsize max, array_view<const char32_t>)override;

			virtual text::encoding_t format()const override;
			virtual stream_index_t position()const override;
			virtual stream_size_t stream_size()const override;
			virtual bool end_of_stream()const override;
			virtual bool move_to(stream_index_t size, stream_reference_t ref)override;

			bool is_valid()const;
			text::itext_buffer* buffer()const;
			bool attach(text::itext_buffer_t);
			template<text::encoding_enum ENCODING> bool attach(ibuffer_t buff) {
				return attach(new text::text_buffer_wrapper<ENCODING>(buff));
			}
			bool can_move_to(stream_index_t size, stream_reference_t ref);
			pointer pointer_at(stream_index_t);

		private:
			bool forward(stream_index_t size);
			bool backward(stream_index_t size);
			bool can_forward(stream_index_t size);
			bool can_backward(stream_index_t size);

		private:
			virtual~text_buffer_input_stream();
		};

		class LINK text_buffer_output_stream final
			: public object
			, public itext_output_stream
		{
		private:
			text::itext_buffer_t _buffer;
			stream_index_t _cursor;
			text::encoder_interface encoder;
			bool _is_formating_enabled;
			text::text_format_t char_format;
			text::text_format_t string_format;
			text::text_format_t signed_format;
			text::text_format_t unsigned_format;
			text::text_format_t floating_format;

		public:
			text_buffer_output_stream();
			text_buffer_output_stream(text_buffer_output_stream*);
			text_buffer_output_stream(text::itext_buffer_t);

		public: //overrides
			ANG_DECLARE_INTERFACE();

			virtual void enable_text_formating(bool)override;
			virtual bool is_text_formating_enabled()const override;
			virtual void text_formating(cstr_t)override;
			virtual void text_formating(text::text_format_t)override;
			virtual bool command(special_command_t) override;
			virtual wsize write(pointer, wsize, text::text_format_t)override;
			virtual wsize write(raw_str_t)override;
			virtual wsize write_line(raw_str_t)override;
			using itext_output_stream::write;
			using itext_output_stream::write_line;

			virtual text::encoding_t format()const override;
			virtual stream_index_t position()const override;
			virtual stream_size_t stream_size()const override;
			virtual bool move_to(stream_index_t size, stream_reference_t ref)override;

			bool is_valid()const;
			text::itext_buffer* buffer()const;
			bool attach(text::itext_buffer_t);
			template<text::encoding_enum ENCODING> bool attach(ibuffer_t buff) {
				return attach(new text::text_buffer_wrapper<ENCODING>(buff));
			}
			bool can_move_to(stream_index_t size, stream_reference_t ref);

		private:
			pointer pointer_at(stream_index_t);
			bool forward(ibuffer_view_t, stream_index_t size);
			bool backward(ibuffer_view_t, stream_index_t size);
			bool can_forward(stream_index_t size);
			bool can_backward(stream_index_t size);

		private:
			virtual~text_buffer_output_stream();
		};

	}
}


ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::streams::stream_mode_t);
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::streams::stream_reference_t);

//#include <ang/inline/streams.inl>

#ifdef LINK
#undef LINK
#endif
#endif//__ANG_STREAMS_HPP__
