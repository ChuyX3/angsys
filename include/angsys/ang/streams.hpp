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
		struct iinput_stream;
		struct ioutput_stream;
		struct ibinary_input_stream;
		struct ibinary_output_stream;
		template<text::encoding_enum ENCODING> struct itext_input_stream;
		template<text::encoding_enum ENCODING> struct itext_output_stream;

		typedef intf_wrapper<iinput_stream> iinput_stream_t;
		typedef intf_wrapper<ioutput_stream> ioutput_stream_t;
		typedef intf_wrapper<ibinary_input_stream> ibinary_input_stream_t;
		typedef intf_wrapper<ibinary_output_stream> ibinary_output_stream_t;
		template<text::encoding_enum ENCODING> using itext_input_stream_t = intf_wrapper<itext_input_stream<ENCODING>>;
		template<text::encoding_enum ENCODING> using itext_output_stream_t = intf_wrapper<itext_output_stream<ENCODING>>;

		struct iserializable;

		ANG_BEGIN_ENUM(LINK, stream_mode, byte)
			unknow,
			in,
			out,
			inout
		ANG_END_ENUM(stream_mode);

		ANG_BEGIN_ENUM(LINK, stream_format, byte)
			binary,
			text
		ANG_END_ENUM(stream_format);

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
			visible vcall wsize read(pointer, wsize)pure;
			visible vcall wsize read(char&)pure;
			visible vcall wsize read(byte&)pure;
			visible vcall wsize read(short&)pure;
			visible vcall wsize read(ushort&)pure;
			visible vcall wsize read(int&)pure;
			visible vcall wsize read(uint&)pure;
			visible vcall wsize read(long&)pure;
			visible vcall wsize read(ulong&)pure;
			visible vcall wsize read(long64&)pure;
			visible vcall wsize read(ulong64&)pure;
			visible vcall wsize read(float&)pure;
			visible vcall wsize read(double&)pure;

			visible vcall stream_format_t format()const pure;
			visible vcall stream_index_t position()const pure;
			visible vcall stream_size_t stream_size()const pure;
			visible vcall bool end_of_stream()const pure;
			visible vcall bool move_to(stream_index_t size, stream_reference_t ref)pure;
			//	visible vcall ibuffer* map(stream_index_t, stream_size_t)const pure;
			//	visible vcall void unmap(ibuffer*)const pure;
		ANG_END_INTERFACE();

		ANG_BEGIN_INTERFACE(LINK, ioutput_stream)
			visible vcall wsize write(pointer, wsize)pure;
			visible vcall wsize write(char)pure;
			visible vcall wsize write(byte)pure;
			visible vcall wsize write(short)pure;
			visible vcall wsize write(ushort)pure;
			visible vcall wsize write(int)pure;
			visible vcall wsize write(uint)pure;
			visible vcall wsize write(long)pure;
			visible vcall wsize write(ulong)pure;
			visible vcall wsize write(long64)pure;
			visible vcall wsize write(ulong64)pure;
			visible vcall wsize write(float)pure;
			visible vcall wsize write(double)pure;

			visible vcall stream_format_t format()const pure;
			visible vcall stream_index_t position()const pure;
			visible vcall stream_size_t stream_size()const pure;
			visible vcall bool move_to(stream_index_t size, stream_reference_t ref)pure;
			//	visible vcall ibuffer* map(stream_index_t, stream_size_t)const pure;
			//	visible vcall void unmap(ibuffer*)const pure;
		ANG_END_INTERFACE();

		//////////////////////////////////////////////////////////////////////////////////

		template<text::encoding_enum ENCODING>
		ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(itext_input_stream, public iinput_stream)
			visible vcall stream_format_t format()const override;
			visible vcall wsize seek(safe_str<text::char_type_by_encoding<ENCODING>::chat_t const> const&)pure; //read a specific sequence of characters
			visible vcall wsize read(strings::string_base<ENCODING>&, wsize max)pure;
			visible vcall wsize read_line(strings::string_base<ENCODING>&, wsize max, array_view<text::char_type_by_encoding<ENCODING>::chat_t> stop)pure;
			using iinput_stream::read;
		ANG_END_INTERFACE();

		template<text::encoding_enum ENCODING>
		ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(itext_output_stream, public ioutput_stream)
			visible vcall stream_format_t format()const override;
			visible vcall void enable_text_format(bool)pure;
			visible vcall bool is_text_format_enabled()const pure;
			visible vcall void text_format(cstr_t)pure;
			visible vcall void text_format(text::text_format_t)pure;
			visible vcall bool command(special_command_t) pure;
			visible vcall wsize write(safe_str<text::char_type_by_encoding<ENCODING>::chat_t const> const&)pure;
			visible vcall wsize write_line(safe_str<text::char_type_by_encoding<ENCODING>::chat_t const> const&)pure;
			using ioutput_stream::write;
		ANG_END_INTERFACE();


		ANG_BEGIN_INTERFACE_WITH_BASE(LINK, ibinary_input_stream, public iinput_stream)
			visible vcall wsize read(string&)pure;
			visible vcall wsize read(wstring&)pure;
			using iinput_stream::read;
			visible vcall stream_format_t format()const override;
		ANG_END_INTERFACE();


		ANG_BEGIN_INTERFACE_WITH_BASE(LINK, ibinary_output_stream, public ioutput_stream)
			visible vcall wsize write(string)pure;
			visible vcall wsize write(wstring)pure;
			using ioutput_stream::write;
			visible vcall stream_format_t format()const override;
		ANG_END_INTERFACE();

	}

	template<text::encoding_enum ENCODING>
	class intf_wrapper<streams::itext_input_stream<ENCODING>>
	{
	public:
		typedef streams::itext_input_stream<ENCODING>	type;
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

	template<text::encoding_enum ENCODING>
	class intf_wrapper<streams::itext_output_stream<ENCODING>>
	{
	public:
		typedef streams::itext_output_stream<ENCODING> type;
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

	template<>
	class LINK intf_wrapper<streams::ibinary_input_stream>
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

	template<>
	class LINK intf_wrapper<streams::ibinary_output_stream>
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
		template<text::encoding_enum ENCODING>inline itext_input_stream_t<ENCODING>& operator >> (itext_input_stream_t<ENCODING>& stream, char& value) { stream->read(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_input_stream_t<ENCODING>& operator >> (itext_input_stream_t<ENCODING>& stream, byte& value) { stream->read(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_input_stream_t<ENCODING>& operator >> (itext_input_stream_t<ENCODING>& stream, short& value) { stream->read(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_input_stream_t<ENCODING>& operator >> (itext_input_stream_t<ENCODING>& stream, ushort& value) { stream->read(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_input_stream_t<ENCODING>& operator >> (itext_input_stream_t<ENCODING>& stream, int& value) { stream->read(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_input_stream_t<ENCODING>& operator >> (itext_input_stream_t<ENCODING>& stream, uint& value) { stream->read(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_input_stream_t<ENCODING>& operator >> (itext_input_stream_t<ENCODING>& stream, long& value) { stream->read(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_input_stream_t<ENCODING>& operator >> (itext_input_stream_t<ENCODING>& stream, ulong& value) { stream->read(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_input_stream_t<ENCODING>& operator >> (itext_input_stream_t<ENCODING>& stream, long64& value) { stream->read(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_input_stream_t<ENCODING>& operator >> (itext_input_stream_t<ENCODING>& stream, ulong64& value) { stream->read(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_input_stream_t<ENCODING>& operator >> (itext_input_stream_t<ENCODING>& stream, float& value) { stream->read(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_input_stream_t<ENCODING>& operator >> (itext_input_stream_t<ENCODING>& stream, double& value) { stream->read(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_input_stream_t<ENCODING>& operator >> (itext_input_stream_t<ENCODING>& stream, strings::string_base<ENCODING>& value) { stream->read(value, -1); return stream; }
		template<text::encoding_enum ENCODING>inline itext_input_stream_t<ENCODING>& operator >> (itext_input_stream_t<ENCODING>& stream, safe_str<typename text::char_type_by_encoding<ENCODING>::char_t const> const& value) { stream->seek(value); return stream; }

		//////////////////////////////////////////////////////////////////////////////////

		template<text::encoding_enum ENCODING>inline itext_output_stream_t<ENCODING>& operator << (itext_output_stream_t<ENCODING>& stream, special_command_t value) { stream->command(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_output_stream_t<ENCODING>& operator << (itext_output_stream_t<ENCODING>& stream, char value) { stream->write(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_output_stream_t<ENCODING>& operator << (itext_output_stream_t<ENCODING>& stream, byte value) { stream->write(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_output_stream_t<ENCODING>& operator << (itext_output_stream_t<ENCODING>& stream, short value) { stream->write(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_output_stream_t<ENCODING>& operator << (itext_output_stream_t<ENCODING>& stream, ushort value) { stream->write(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_output_stream_t<ENCODING>& operator << (itext_output_stream_t<ENCODING>& stream, int value) { stream->write(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_output_stream_t<ENCODING>& operator << (itext_output_stream_t<ENCODING>& stream, uint value) { stream->write(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_output_stream_t<ENCODING>& operator << (itext_output_stream_t<ENCODING>& stream, long value) { stream->write(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_output_stream_t<ENCODING>& operator << (itext_output_stream_t<ENCODING>& stream, ulong value) { stream->write(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_output_stream_t<ENCODING>& operator << (itext_output_stream_t<ENCODING>& stream, long64 value) { stream->write(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_output_stream_t<ENCODING>& operator << (itext_output_stream_t<ENCODING>& stream, ulong64 value) { stream->write(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_output_stream_t<ENCODING>& operator << (itext_output_stream_t<ENCODING>& stream, float value) { stream->write(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_output_stream_t<ENCODING>& operator << (itext_output_stream_t<ENCODING>& stream, double value) { stream->write(value); return stream; }
		template<text::encoding_enum ENCODING>inline itext_output_stream_t<ENCODING>& operator << (itext_output_stream_t<ENCODING>& stream, safe_str<typename text::char_type_by_encoding<ENCODING>::char_t const> const& value) { stream->write(value); return stream; }

		//////////////////////////////////////////////////////////////////////////////////

		inline ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, char& value) { stream->read(value); return stream; }
		inline ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, byte& value) { stream->read(value); return stream; }
		inline ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, short& value) { stream->read(value); return stream; }
		inline ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, ushort& value) { stream->read(value); return stream; }
		inline ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, int& value) { stream->read(value); return stream; }
		inline ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, uint& value) { stream->read(value); return stream; }
		inline ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, long& value) { stream->read(value); return stream; }
		inline ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, ulong& value) { stream->read(value); return stream; }
		inline ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, long64& value) { stream->read(value); return stream; }
		inline ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, ulong64& value) { stream->read(value); return stream; }
		inline ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, float& value) { stream->read(value); return stream; }
		inline ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, double& value) { stream->read(value); return stream; }
		inline ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, string& value) { stream->read(value); return stream; }
		inline ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, wstring& value) { stream->read(value); return stream; }
		template<typename T>
		inline ibinary_input_stream_t& operator >> (ibinary_input_stream_t& stream, T& value) { stream->read(&value, sizeof(value)); return stream; }

		//////////////////////////////////////////////////////////////////////////////////

		inline ibinary_output_stream_t& operator << (ibinary_output_stream_t& stream, char value) { stream->write(value); return stream; }
		inline ibinary_output_stream_t& operator << (ibinary_output_stream_t& stream, byte value) { stream->write(value); return stream; }
		inline ibinary_output_stream_t& operator << (ibinary_output_stream_t& stream, short value) { stream->write(value); return stream; }
		inline ibinary_output_stream_t& operator << (ibinary_output_stream_t& stream, ushort value) { stream->write(value); return stream; }
		inline ibinary_output_stream_t& operator << (ibinary_output_stream_t& stream, int value) { stream->write(value); return stream; }
		inline ibinary_output_stream_t& operator << (ibinary_output_stream_t& stream, uint value) { stream->write(value); return stream; }
		inline ibinary_output_stream_t& operator << (ibinary_output_stream_t& stream, long value) { stream->write(value); return stream; }
		inline ibinary_output_stream_t& operator << (ibinary_output_stream_t& stream, ulong value) { stream->write(value); return stream; }
		inline ibinary_output_stream_t& operator << (ibinary_output_stream_t& stream, long64 value) { stream->write(value); return stream; }
		inline ibinary_output_stream_t& operator << (ibinary_output_stream_t& stream, ulong64 value) { stream->write(value); return stream; }
		inline ibinary_output_stream_t& operator << (ibinary_output_stream_t& stream, float value) { stream->write(value); return stream; }
		inline ibinary_output_stream_t& operator << (ibinary_output_stream_t& stream, double value) { stream->write(value); return stream; }
		inline ibinary_output_stream_t& operator << (ibinary_output_stream_t& stream, string value) { stream->write(value); return stream; }
		inline ibinary_output_stream_t& operator << (ibinary_output_stream_t& stream, wstring value) { stream->write(value); return stream; }
	}
}

//stream buffers

namespace ang
{
	namespace streams
	{

		class binary_buffer_input_stream;
		class binary_buffer_output_stream;
		template<text::encoding_enum ENCODING>class text_buffer_input_stream;
		template<text::encoding_enum ENCODING>class text_buffer_output_stream;

		typedef object_wrapper<binary_buffer_input_stream> binary_buffer_input_stream_t;
		typedef object_wrapper<binary_buffer_output_stream> binary_buffer_output_stream_t;
		template<text::encoding_enum ENCODING>using text_buffer_input_stream_t = object_wrapper<text_buffer_input_stream<ENCODING>>;
		template<text::encoding_enum ENCODING>using text_buffer_output_stream_t = object_wrapper<text_buffer_output_stream<ENCODING>>;

		template<text::encoding_enum ENCODING>
		class text_buffer_input_stream final
			: public object
			, public itext_input_stream<ENCODING>
		{
		private:
			ibuffer_t _buffer;
			stream_index_t _cursor;
			text::encoding_t _format;

		public:
			text_buffer_input_stream();
			text_buffer_input_stream(text_buffer_input_stream*);
			text_buffer_input_stream(text::itext_buffer*);
			text_buffer_input_stream(ibuffer_t, text::encoding_t _format);

		public: //overrides
			ANG_DECLARE_INTERFACE();

			virtual wsize read(pointer, wsize)override;
			virtual wsize read(char&)override;
			virtual wsize read(byte&)override;
			virtual wsize read(short&)override;
			virtual wsize read(ushort&)override;
			virtual wsize read(int&)override;
			virtual wsize read(uint&)override;
			virtual wsize read(long&)override;
			virtual wsize read(ulong&)override;
			virtual wsize read(long64&)override;
			virtual wsize read(ulong64&)override;
			virtual wsize read(float&)override;
			virtual wsize read(double&)override;
			virtual wsize seek(safe_str<text::char_type_by_encoding<ENCODING>::chat_t const> const&)override; //read a specific sequence of characters
			virtual wsize read(strings::string_base<ENCODING>&, wsize max)override;
			virtual wsize read_line(strings::string_base<ENCODING>&, wsize max, array_view<text::char_type_by_encoding<ENCODING>::chat_t> stop)override;

			virtual stream_index_t position()const override;
			virtual stream_size_t stream_size()const override;
			virtual bool end_of_stream()const override;
			virtual bool move_to(stream_index_t size, stream_reference_t ref)override;

			bool is_valid()const;
			ibuffer* buffer()const;
			bool attach(text::itext_buffer*);
			bool attach(ibuffer* buff, text::encoding_t);
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


		template<text::encoding_enum ENCODING>
		class text_buffer_output_stream
			: public object
			, public itext_output_stream<ENCODING>
		{
		private:
			ibuffer_t _buffer;
			stream_index_t _cursor;
			text::encoding_t _format;
			bool _is_formating_enabled;
			text::text_format_t char_format;
			text::text_format_t string_format;
			text::text_format_t signed_format;
			text::text_format_t unsigned_format;
			text::text_format_t floating_format;

		public:
			text_buffer_output_stream();
			text_buffer_output_stream(text_buffer_output_stream*);
			text_buffer_output_stream(text::itext_buffer*);
			text_buffer_output_stream(ibuffer_t, text::encoding_t);

		public: //overrides
			ANG_DECLARE_INTERFACE();

			virtual wsize write(pointer, wsize)override;
			virtual wsize write(char)override;
			virtual wsize write(byte)override;
			virtual wsize write(short)override;
			virtual wsize write(ushort)override;
			virtual wsize write(int)override;
			virtual wsize write(uint)override;
			virtual wsize write(long)override;
			virtual wsize write(ulong)override;
			virtual wsize write(long64)override;
			virtual wsize write(ulong64)override;
			virtual wsize write(float)override;
			virtual wsize write(double)override;
			virtual wsize write(safe_str<text::char_type_by_encoding<ENCODING>::chat_t const> const&)override;
			virtual wsize write_line(safe_str<text::char_type_by_encoding<ENCODING>::chat_t const> const&)override;

			virtual bool command(special_command_t) override;
			virtual stream_index_t position()const override;
			virtual stream_size_t stream_size()const override;
			virtual bool move_to(stream_index_t size, stream_reference_t ref)override;

			virtual void enable_text_format(bool)override;
			virtual bool is_text_format_enabled()const override;
			virtual void text_format(cstr_t)override;
			virtual void text_format(text::text_format_t)override;

			bool is_valid()const;
			ibuffer* buffer()const;
			bool attach(text::itext_buffer*);
			bool attach(ibuffer* buff, text::encoding_t);
			bool can_move_to(stream_index_t size, stream_reference_t ref);

		private:
			pointer pointer_at(stream_index_t);
			bool forward(pointer,stream_index_t size);
			bool backward(pointer,stream_index_t size);
			bool can_forward(stream_index_t size);
			bool can_backward(stream_index_t size);

		private:
			virtual~text_buffer_output_stream();
		};

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
			virtual wsize read(char&)override;
			virtual wsize read(byte&)override;
			virtual wsize read(short&)override;
			virtual wsize read(ushort&)override;
			virtual wsize read(int&)override;
			virtual wsize read(uint&)override;
			virtual wsize read(long&)override;
			virtual wsize read(ulong&)override;
			virtual wsize read(long64&)override;
			virtual wsize read(ulong64&)override;
			virtual wsize read(float&)override;
			virtual wsize read(double&)override;
			virtual wsize read(string&)override;
			virtual wsize read(wstring&)override;

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
			virtual wsize write(char)override;
			virtual wsize write(byte)override;
			virtual wsize write(short)override;
			virtual wsize write(ushort)override;
			virtual wsize write(int)override;
			virtual wsize write(uint)override;
			virtual wsize write(long)override;
			virtual wsize write(ulong)override;
			virtual wsize write(long64)override;
			virtual wsize write(ulong64)override;
			virtual wsize write(float)override;
			virtual wsize write(double)override;
			virtual wsize write(string)override;
			virtual wsize write(wstring)override;

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


		/*inline text_buffer_input_stream_t& operator >> (text_buffer_input_stream_t& stream, char& value) { stream->read(value); return stream; }
		inline text_buffer_input_stream_t& operator >> (text_buffer_input_stream_t& stream, byte& value) { stream->read(value); return stream; }
		inline text_buffer_input_stream_t& operator >> (text_buffer_input_stream_t& stream, short& value) { stream->read(value); return stream; }
		inline text_buffer_input_stream_t& operator >> (text_buffer_input_stream_t& stream, ushort& value) { stream->read(value); return stream; }
		inline text_buffer_input_stream_t& operator >> (text_buffer_input_stream_t& stream, int& value) { stream->read(value); return stream; }
		inline text_buffer_input_stream_t& operator >> (text_buffer_input_stream_t& stream, uint& value) { stream->read(value); return stream; }
		inline text_buffer_input_stream_t& operator >> (text_buffer_input_stream_t& stream, long& value) { stream->read(value); return stream; }
		inline text_buffer_input_stream_t& operator >> (text_buffer_input_stream_t& stream, ulong& value) { stream->read(value); return stream; }
		inline text_buffer_input_stream_t& operator >> (text_buffer_input_stream_t& stream, float& value) { stream->read(value); return stream; }
		inline text_buffer_input_stream_t& operator >> (text_buffer_input_stream_t& stream, double& value) { stream->read(value); return stream; }
		inline text_buffer_input_stream_t& operator >> (text_buffer_input_stream_t& stream, string& value) { stream->read(value, -1); return stream; }
		inline text_buffer_input_stream_t& operator >> (text_buffer_input_stream_t& stream, wstring& value) { stream->read(value, -1); return stream; }
		inline text_buffer_input_stream_t& operator >> (text_buffer_input_stream_t& stream, cstr_t value) { stream->seek(value); return stream; }
		inline text_buffer_input_stream_t& operator >> (text_buffer_input_stream_t& stream, cwstr_t value) { stream->seek(value); return stream; }*/

	}
}


ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::streams::stream_format_t);
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::streams::stream_mode_t);
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::streams::stream_reference_t);

#ifdef LINK
#undef LINK
#endif
#endif//__ANG_STREAMS_HPP__
