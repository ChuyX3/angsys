#ifndef __ANG_CORE_FILES_H__
#define __ANG_CORE_FILES_H__

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
		namespace files
		{
			ang_interface(ifile);
			ang_interface(ifile_system);

			ang_object(input_text_file);
			ang_object(output_text_file);
			ang_object(input_binary_file);
			ang_object(output_binary_file);

#ifdef WINDOWS_PLATFORM
			typedef wstring path_t;
			typedef cwstr_t path_view_t;
			inline str_view<const wchar> operator "" _path(const wchar* str, wsize sz) { return str_view<const wchar>(str, sz); }
#else
			typedef string path_t;
			typedef cstr_t path_view_t;
			inline str_view<const char> operator "" _path(const char* str, wsize sz) { return str_view<const char>(str, sz); }
#endif // PLATFORM_WINDOWS

			using streams::stream_mode;
			using streams::stream_mode_t;

			using streams::stream_reference;
			using streams::stream_reference_t;

			using file_size_t = streams::stream_size_t;
			using file_offset_t = streams::stream_index_t;


			safe_enum(LINK, file_system_priority, uint)
			{
				lowest,
				highest,
			};

			safe_flags(LINK, open_flags, uint)
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

				open_exist = 0x1000, //Open the file only if it exists, if it does not exist the Create function return false
				open_alway = 0x2000, //Open if it exist, if it don´t exist and it is for writing create a new file
				create_new = 0x3000, // Create a new file if it don´t exist, if the file exist it is not created and return false
				create_alway = 0x4000,// Create a new file if it don´t exist, if the file exist the existing file will be rewriting 
			};


			ang_begin_interface(LINK ifile)
				visible vcall stream_mode_t mode()const pure;
				visible vcall path_t path()const pure;
				visible vcall text::encoding_t format()const pure;
				visible vcall file_size_t size()const pure;
				visible vcall bool size(file_size_t) pure;//write mode only
				visible vcall bool is_eof()const pure;
				visible vcall file_offset_t cursor()const pure;
				visible vcall bool cursor(file_offset_t size, stream_reference_t ref)pure;
				visible vcall file_offset_t read(ibuffer_view_t)pure;
				visible vcall file_offset_t read(wsize, pointer)pure;
				visible vcall file_offset_t write(ibuffer_view_t)pure;
				visible vcall file_offset_t write(wsize, pointer)pure;

				visible vcall ibuffer_t map(wsize size, file_offset_t offset)pure;
				visible vcall bool unmap(ibuffer_t, wsize used)pure;
				visible vcall bool set_mutex(core::async::mutex_ptr_t)pure;
			ang_end_interface();

			ang_begin_interface(LINK ifile_system)
				visible scall ifile_system_t fs_instance();
				visible vcall array_view<path_t> paths(file_system_priority_t)const pure;
				visible vcall void push_path(path_view_t, file_system_priority_t) pure;
				visible vcall bool open_file(path_view_t, open_flags_t, ifile_ptr_t)pure;
				
				visible vcall bool open(path_view_t, input_text_file_ptr_t)pure;
				visible vcall bool open(path_view_t, output_text_file_ptr_t)pure;
				visible vcall bool open(path_view_t, input_binary_file_ptr_t)pure;
				visible vcall bool open(path_view_t, output_binary_file_ptr_t)pure;
			ang_end_interface();

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
				: public object
			{
			protected:
				ifile_t m_hfile;

				file();
				virtual~file();
				bool create(path_view_t path, open_flags_t flags);
				ibuffer_t map(wsize, file_offset_t);
				bool unmap(ibuffer_t, wsize);

			public: //Overrides
				ANG_DECLARE_INTERFACE();
				void clear()override;
				bool attach(ifile*);
				bool is_valid()const;
				virtual stream_mode_t mode()const = 0;
				bool set_mutex(core::async::mutex_ptr_t);
			
			};

			class LINK input_text_file final
				: public core::files::file
				, public streams::itext_input_stream
			{
			public:
				input_text_file();
				input_text_file(path_view_t path);

				ANG_DECLARE_INTERFACE();

				bool open(path_view_t path);
				
				stream_mode_t mode()const override;
				text::encoding_t format()const override;
				file_offset_t cursor()const override;
				file_size_t size()const override;
				bool is_eos()const override;
				bool cursor(file_offset_t size, stream_reference_t ref)override;

				wsize read_format(raw_cstr_t format, var_args_t&)override;
				wsize read(ibuffer_view_t, text::encoding_t, wsize*written = null)override;
				wsize read_line(ibuffer_view_t, text::encoding_t, array_view<const char32_t> = U"\n\r", wsize* written = null)override;

				template<text::encoding E, template<typename> class A>
				wsize read(strings::basic_string<E, A>& str, wsize max) {
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
				wsize read_line(strings::basic_string<E, A>& str, wsize max, array_view<const char32_t> end = U"\n\r") {
					if (str.is_empty()) str = "";
					max = ang::min(max, 1 << 10);
					str->realloc(max, false);
					wsize written = 0;
					auto view = str->map_buffer(0, max);
					auto readed = read_line(view, E, end, &written);
					auto cs = text::iencoder::get_encoder(format())->class_info().size();
					str->unmap_buffer(view, written);
					return readed;
				}

				template<typename C, text::encoding E> wsize read_format(str_view<C, E> format, var_args_t& va) {
					return read_format(raw_cstr(format), ang::forward<var_args_t&>(va));
				}
				template<typename C, text::encoding E, typename...Ts> wsize read_format(str_view<C, E> format, Ts&...) {
					var_args_t va = new var_args();
					return streams::read_format_helper<input_text_file_t, C, E, Ts...>::read_format(this, format, va, args...);
				}

			private:
				virtual~input_text_file();
			};

			class LINK output_text_file final
				: public core::files::file
				, public streams::itext_output_stream
			{
			public:
				output_text_file();
				output_text_file(path_view_t path, text::encoding_t = text::encoding::ascii);

				ANG_DECLARE_INTERFACE();

				bool open(path_view_t path, text::encoding_t = text::encoding::ascii);
				stream_mode_t mode()const override;
				text::encoding_t format()const override;
				file_offset_t cursor()const override;
				file_size_t size()const override;
				bool cursor(file_offset_t size, stream_reference_t ref)override;

				bool command(streams::special_command_t) override;
				wsize write(raw_cstr_t)override;
				wsize write_line(raw_cstr_t)override;
				wsize write_format(raw_cstr_t, var_args_t)override;
				template<typename T> wsize write(T const& val) {
					streams::write_text_helper<output_text_file_t, T>::write_text(this, val);
				}
				template<typename C, text::encoding E, typename...Ts> wsize write_format(str_view<C, E> format, Ts... args) {
					return write_format(raw_cstr(format), var_args_t{ ang::forward<Ts>(args)... });
				}
			private:
				virtual~output_text_file();
			};

		}
	}
}

#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANG_CORE_FILES_H__