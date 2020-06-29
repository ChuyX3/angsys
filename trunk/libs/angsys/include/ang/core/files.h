/*********************************************************************************************************************/
/*   File Name: ang/core/files.h                                                                                     */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_CORE_FILES_H__
#define __ANG_CORE_FILES_H__

#include <ang/streams.h>
#include <ang/core/async.h>

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


namespace ang
{
	namespace core
	{
		namespace files
		{
			ang_declare_interface(ifile);
			//ang_declare_interface(ifolder);
			ang_declare_interface(ifile_system);

			//ang_declare_object(path);
			//typedef struct path path_t;

			ang_declare_object(input_text_file);
			ang_declare_object(output_text_file);
			ang_declare_object(input_binary_file);
			ang_declare_object(output_binary_file);

			using streams::stream_mode;
			using streams::stream_mode_t;

			using streams::stream_reference;
			using streams::stream_reference_t;

			using file_size_t = streams::stream_size_t;
			using file_offset_t = streams::stream_index_t;

			declare_enum(LINK, element_type, uint)
			{
				folder,
				file,
			};

			declare_enum(LINK, file_system_priority, uint)
			{
				lowest,
				highest
			};

			declare_flags(LINK, path_access_type, uint)
			{
				none,
				read,
				write,
				all,
			};

			declare_flags(LINK, open_flags, uint)
			{
				null = 0x0000,

				format_binary = 1,
				format_packfile,

				format_text = 0X10, //autodetect format
				format_ascii = 0X11,
				format_utf8,
				format_utf16_le,
				format_utf16_be,
				format_utf32_le,
				format_utf32_be,

				access_in = 0x0100,
				access_out = 0x0200,
				access_inout = 0x0300,

				open_exist = 0x1000, //Open the file only if it exists, if it does not exist the open function return false
				open_alway = 0x2000, //Open if it exist, if it don´t exist and it is for writing create a new file
				create_new = 0x3000, // Create a new file if it don´t exist, if the file exist it is not created and return false
				create_alway = 0x4000,// Create a new file if it don´t exist, if the file exist the existing file will be rewriting 
			};

			declare_enum(LINK, file_mode, uint)
			{
				input_binary = 0X00,
				output_binary = 0X01,
				input_text = 0X10,
				output_text = 0X11
			};

			template<file_mode FM> struct type_by_mode;

			template<> struct type_by_mode<file_mode::input_binary> {
				using type = input_binary_file;
				using type_ptr = input_binary_file_t;
				static inline open_flags_t flags() {
					return open_flags::access_in + open_flags::open_exist;
				}
			};

			template<> struct type_by_mode<file_mode::output_binary> {
				using type = output_binary_file;
				using type_ptr = output_binary_file_t;
				static inline open_flags_t flags() {
					return open_flags::access_out + open_flags::open_alway;
				}
			};

			template<> struct type_by_mode<file_mode::input_text> {
				using type = input_text_file;
				using type_ptr = input_text_file_t;
				static inline open_flags_t flags() {
					return open_flags::access_in + open_flags::open_exist + open_flags::format_text;
				}
			};

			template<> struct type_by_mode<file_mode::output_text> {
				using type = output_text_file;
				using type_ptr = output_text_file_t;
				static inline open_flags_t flags() {
					return open_flags::access_out + open_flags::open_alway + open_flags::format_text;
				}
			};


			struct nvt LINK ifile
				: intf<ifile
				, iid("ang::core::files::ifile")>
			{
				virtual stream_mode_t mode()const = 0;
				virtual string path()const = 0;
				virtual text::encoding_t format()const = 0;
				virtual file_size_t size()const = 0;
				virtual bool size(file_size_t) = 0;//write mode only
				virtual bool is_eof()const = 0;
				virtual file_offset_t cursor()const = 0;
				virtual bool cursor(file_offset_t size, stream_reference_t ref) = 0;
				virtual file_offset_t read(ibuffer_view_t) = 0;
				virtual file_offset_t read(wsize, pointer) = 0;
				virtual file_offset_t write(ibuffer_view_t) = 0;
				virtual file_offset_t write(wsize, pointer) = 0;
				virtual ibuffer_t map(wsize size, file_offset_t offset) = 0;
				virtual bool unmap(ibuffer_t, wsize used) = 0;
				virtual bool set_mutex(core::async::mutex_ptr_t) = 0;
			};

			struct nvt LINK ifile_system
				: intf<ifile_system
				, iid("ang::core::files::ifile_system")>
			{
				static ifile_system_t instance();
				virtual vector<string> paths(path_access_type_t access = path_access_type::read)const = 0;
				virtual void push_path(cstr_t path, path_access_type_t access = path_access_type::read, cstr_t macro = null) = 0;
				virtual vector<string> find_paths(cstr_t macro)const = 0;
				virtual path_access_type_t path_access_type(cstr_t path)const = 0;
				virtual bool create_handle(cstr_t path, open_flags_t flags, ifile_ptr_t out, cstr_t macro = null) = 0;
				virtual async::iasync_op<ifile> create_handle_async(cstr_t path, open_flags_t flags, cstr_t macro = null) = 0;
				virtual bool open(cstr_t path, input_text_file_ptr_t out, cstr_t macro = null) = 0;
				virtual bool open(cstr_t path, output_text_file_ptr_t out, cstr_t macro = null) = 0;
				virtual bool open(cstr_t path, input_binary_file_ptr_t out, cstr_t macro = null) = 0;
				virtual bool open(cstr_t path, output_binary_file_ptr_t out, cstr_t macro = null) = 0;
				template<file_mode FM> inline async::iasync_op<typename type_by_mode<FM>::type_ptr> open_async(cstr_t path, cstr_t macro = null) {
					return create_handle_async(path, type_by_mode<FM>::flags(), macro)->then<optional<typename type_by_mode<FM>::type_ptr>>(
						[](async::iasync_op<ifile> task)->optional<typename type_by_mode<FM>::type_ptr> {
						try {
							auto op = task->result();
							if (op.failed())
								return op.error();
							typename type_by_mode<FM>::type_ptr file = new typename type_by_mode<FM>::type();
							file->attach(op.get());
							return file;
						}
						catch (exception const& ex) {
							return error(ex);
						}
					});
				}
			};

		}
	}
}

namespace ang
{
	namespace core
	{
		namespace files
		{
			
			class LINK file
				: public implement<file
				, iid("ang::core::files::file")>
			{
			protected:
				ifile_t m_hfile;

				file();
				virtual~file();
				bool create(cstr_t path, open_flags_t flags);
				ibuffer_t map(wsize, file_offset_t);
				bool unmap(ibuffer_t, wsize);

			public: //Overrides
				void dispose()override;
				bool attach(ifile*);
				bool is_valid()const;
				virtual stream_mode_t mode()const = 0;
				bool set_mutex(core::async::mutex_ptr_t);

			};

			class LINK input_text_file final
				: public implement<input_text_file
				, iid("ang::core::files::input_text_file")
				, core::files::file
				, streams::itext_input_stream
				, streams::iinput_stream>
			{
			public:
				input_text_file();
				input_text_file(cstr_t path);

				bool open(cstr_t path);

				stream_mode_t mode()const override;
				text::encoding_t format()const override;
				file_offset_t cursor()const override;
				file_size_t size()const override;
				bool is_eos()const override;
				bool cursor(file_offset_t size, stream_reference_t ref)override;

				wsize seek(cstr_t format)override;
				wsize read(pointer, ang::rtti_t const&);
				wsize read_format(cstr_t format, var_args_t&)override;
				wsize read(string, wsize, wsize*written = null)override;
				wsize read(pointer, wsize, text::encoding_t, wsize*written = null)override;
				wsize read_line(string, array_view<const char32_t> = U"\r\n", wsize* written = null)override;
				wsize read_line(pointer, wsize, text::encoding_t, array_view<const char32_t> = U"\r\n", wsize* written = null)override;

				bool map(function<bool(ibuffer_view_t)> func, wsize = -1, file_offset_t = 0);
				bool map2(function<bool(string)> func, wsize = -1, file_offset_t = 0);

				bool read(function<bool(streams::itext_input_stream_t)>);

				template<text::encoding E, template<typename> class A>
				wsize read(text::basic_string<E, A>& str, wsize max) {
					if (str.is_empty()) str = "";
					max = ang::min(max, 1 << 10);
					str->realloc(max, false);
					wsize written = 0;
					auto view = str->map_buffer(0, max);
					auto readed = read(view, E, &written);
					str->unmap_buffer(view, written);
					return readed;
				}

				template<text::encoding E, template<typename> class A>
				wsize read_line(text::basic_string<E, A>& str, wsize max, array_view<const char32_t> end = U"\n\r") {
					if (str.is_empty()) str = "";
					max = ang::min(max, 1 << 10);
					str->realloc(max, false);
					wsize written = 0;
					auto view = str->map_buffer(0, max);
					auto readed = read_line(view, E, end, &written);
					auto cs = text::encoder<text::encoding::auto_detect>::char_size_by_encoding(format());
					str->unmap_buffer(view, written);
					return readed;
				}


				template<typename...Ts> uint read_format(cstr_t format, Ts&...args) {
					var_args_t va;
					return streams::read_format_helper<input_text_file_t, Ts...>::read_format(this, format, va, args...);
				}



			private:
				virtual~input_text_file();
			};

			class LINK output_text_file final
				: public implement<output_text_file
				, iid("ang::core::files::output_text_file")
				, core::files::file
				, streams::itext_output_stream
				, streams::ioutput_stream>
			{
			public:
				output_text_file();
				output_text_file(cstr_t path, text::encoding_t = text::encoding::ascii);

				bool open(cstr_t path, text::encoding_t = text::encoding::ascii);
				stream_mode_t mode()const override;
				text::encoding_t format()const override;
				file_offset_t cursor()const override;
				file_size_t size()const override;
				bool cursor(file_offset_t size, stream_reference_t ref)override;

				bool command(streams::special_command_t) override;
				wsize write(cstr_t)override;
				wsize write_line(cstr_t)override;
				wsize write_format(cstr_t, var_args_t)override;
				template<typename T> wsize write(T const& val) {
					streams::write_text_helper<output_text_file_t, T>::write_text(this, val);
				}
				template<typename C, text::encoding E, typename...Ts> wsize write_format(str_view<C, E> format, Ts... args) {
					return write_format(raw_cstr(format), var_args_t{ ang::forward<Ts>(args)... });
				}
			private:
				virtual~output_text_file();
			};

			class LINK input_binary_file final
				: public implement<input_binary_file
				, iid("ang::core::files::input_binary_file")
				, core::files::file
				, streams::ibinary_input_stream
				, streams::iinput_stream>
			{
			public:
				input_binary_file();
				input_binary_file(cstr_t path);

				bool open(cstr_t path);

				stream_mode_t mode()const override;
				text::encoding_t format()const override;
				file_offset_t cursor()const override;
				file_size_t size()const override;
				bool is_eos()const override;
				bool cursor(file_offset_t size, stream_reference_t ref)override;

				wsize read(pointer, wsize)override;
				wsize read(ibuffer_t)override;
				wsize read(pointer, const rtti_t&)override;

				bool map(function<bool(ibuffer_view_t)> func, wsize = -1, file_offset_t = 0);
				//bool map(function<bool(text::istring_view_t)> func, wsize = -1, file_offset_t = 0);

				error read(function<error(streams::ibinary_input_stream_t)>);

			private:
				virtual~input_binary_file();
			};

			class LINK output_binary_file final
				: public implement<output_binary_file
				, iid("ang::core::files::file")
				, core::files::file
				, streams::ibinary_output_stream
				, streams::ioutput_stream>
			{
			public:
				output_binary_file();
				output_binary_file(cstr_t path);

				bool open(cstr_t path);
				stream_mode_t mode()const override;
				text::encoding_t format()const override;
				file_offset_t cursor()const override;
				file_size_t size()const override;
				bool cursor(file_offset_t size, stream_reference_t ref)override;

				wsize write(pointer, wsize)override;
				wsize write(ibuffer_view_t)override;

			private:
				virtual~output_binary_file();
			};
		}
	}
}

ANG_ENUM_DECLARATION(LINK, ang::core::files::file_system_priority);
ANG_FLAGS_DECLARATION(LINK, ang::core::files::path_access_type);
ANG_FLAGS_DECLARATION(LINK, ang::core::files::open_flags);

#ifdef  LINK
#undef  LINK
#endif//LINK

//#include <ang/core/inline/path.inl>

#endif//__ANG_CORE_FILES_H__
