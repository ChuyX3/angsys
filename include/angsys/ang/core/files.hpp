/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_FILES_H__
#define __ANG_FILES_H__

#include <angsys.h>
#include <ang/streams.hpp>
#include <ang/core/async.hpp>

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
#endif

namespace ang
{
	namespace core
	{
		namespace files
		{
			struct ifile;
			struct ifile_system;
			typedef intf_wrapper<ifile> ifile_t;
			typedef intf_wrapper_ptr<ifile> ifile_ptr_t;
			typedef intf_wrapper<ifile_system> ifile_system_t;

#if defined WINDOWS_PLATFORM
			typedef wstring path;
			typedef cwstr_t path_view;
#elif defined ANDROID_PLATFORM
			typedef string path; 
			typedef cstr_t path_view;
#endif // WINDOWS_PLATFORM


			class file;
			class pack_file;
			typedef object_wrapper<file> file_t;
			typedef object_wrapper<pack_file> pack_file_t;

			typedef streams::stream_size_t file_size_t;
			typedef streams::stream_index_t file_cursor_t;
			typedef streams::stream_reference file_reference;
			typedef streams::stream_reference_t file_reference_t;

			class text_file;
			class binary_file;
			class input_text_file;
			class output_text_file;
			class input_binary_file;
			class output_binary_file;

			typedef object_wrapper<text_file> text_file_t;
			typedef object_wrapper<binary_file> binary_file_t;
			typedef object_wrapper<input_text_file> input_text_file_t;
			typedef object_wrapper<output_text_file> output_text_file_t;
			typedef object_wrapper<input_binary_file> input_binary_file_t;
			typedef object_wrapper<output_binary_file> output_binary_file_t;

		}
	}

	template<> class LINK intf_wrapper<core::files::ifile>
	{
	public:
		typedef core::files::ifile	type;

	protected:
		core::files::ifile* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(core::files::ifile*);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);
		~intf_wrapper();
	public:
		void clean();
		bool is_empty()const;
		core::files::ifile* get(void)const;
		void set(core::files::ifile*);
		core::files::ifile ** addres_of(void);

	public:
		intf_wrapper& operator = (core::files::ifile*);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);

		intf_wrapper_ptr<core::files::ifile> operator & (void);
		core::files::ifile * operator -> (void);
		core::files::ifile const* operator -> (void)const;
		operator core::files::ifile * (void);
		operator core::files::ifile const* (void)const;
	};


	namespace core
	{
		namespace files
		{
			ANG_BEGIN_ENUM(LINK, file_system_priority, uint)
				lowest,
				highest,
			ANG_END_ENUM(file_system_priority)

			ANG_BEGIN_FLAGS(LINK, open_flags, uint)
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
			ANG_END_FLAGS(open_flags);

			typedef open_flags file_flags;
			typedef open_flags_t file_flags_t;

			typedef struct LINK pack_file_info
			{
				file_size_t size;
				file_cursor_t offset;
			}pack_file_info_t;

			//inline bool operator == (pack_file_info_t const& a1, pack_file_info_t const& a2) { return a1.path == a2.path; }
			//inline bool operator != (pack_file_info_t const& a1, pack_file_info_t const& a2) { return a1.path != a2.path; }


			ANG_BEGIN_INTERFACE_WITH_BASE(LINK, ifile, public streams::istream)
				visible vcall streams::stream_mode_t mode()const pure;
				visible vcall path file_path()const pure;
				visible vcall void format(text::encoding_t)pure;
				visible vcall bool stream_size(file_size_t)pure;

				visible vcall ibuffer_t map(wsize size, file_cursor_t offset)pure;
				visible vcall bool unmap(ibuffer_t, wsize)pure;
				visible vcall bool set_mutex(core::async::mutex_ptr_t)pure;

				visible using streams::istream::format;
				visible using streams::istream::stream_size;

				visible using streams::istream::read;
				visible using streams::istream::write;

				//visible vcall core::async::iasync_t<ibuffer_t> read_async(wsize size)pure;
				//visible vcall core::async::iasync_t<wsize> write_async(wsize size, pointer buffer)pure;
			ANG_END_INTERFACE();


			ANG_BEGIN_INTERFACE(LINK, ifile_system)
				visible static ifile_system_t get_file_system();
				visible static ifile_system_t create_file_system(path root);
				visible static bool register_file_system(ifile_system*, file_system_priority_t);
				visible vcall array<path> paths()const pure;
				visible vcall bool register_paths(path_view) pure;
				visible vcall bool create_file_handle(path_view, open_flags_t, ifile_ptr_t) pure;

				visible vcall bool open(path_view, input_text_file_t&)pure;
				visible vcall bool open(path_view, output_text_file_t&)pure;
				visible vcall bool open(path_view, input_binary_file_t&)pure;
				visible vcall bool open(path_view, output_binary_file_t&)pure;
			ANG_END_INTERFACE();

		}
	}
}

ANG_REGISTER_RUNTIME_TYPENAME(ang::core::files::pack_file_info_t);
//ANG_DECLARE_ARRAY_DATA_SPECIALIZATION(LINK, ang::core::files::pack_file_info);

namespace ang
{
	namespace core
	{
		namespace files
		{

			class LINK file :
				public object
			{
			protected:
				ifile_t hfile;

				file();
				virtual~file();
				bool create(path_view path, open_flags_t flags);
				ibuffer_t map(wsize, file_cursor_t);
				bool unmap(ibuffer_t, wsize);

			public: //Overrides
				ANG_DECLARE_INTERFACE();
				bool attach(ifile*);
				bool is_valid()const;
				file_size_t file_size()const;
				bool file_size(file_size_t);
				streams::stream_mode_t mode()const;
				bool set_mutex(core::async::mutex_ptr_t);
				virtual bool close();		


				friend pack_file;
			};
		
			class LINK pack_file
				: public file
				, public ifile_system
			{
			public:
				static bool create_pack_from_folder(path_view in_path, path_view out_path);

			private:
				core::async::mutex_t mutex;
				array<collections::pair<path, pack_file_info>> files;
						
			public:
				pack_file();

			public: //Overrides
				ANG_DECLARE_INTERFACE();

				bool create(path_view path, open_flags_t flags);

				array<path> paths()const override;
				bool register_paths(path_view) override;
				bool create_file_handle(path_view, open_flags_t, ifile_ptr_t) override;

				bool open(path_view path, input_text_file_t&)override;
				bool open(path_view path, output_text_file_t&)override;
				bool open(path_view path, input_binary_file_t&)override;
				bool open(path_view path, output_binary_file_t&)override;

			protected:
				virtual~pack_file();
			};

			class LINK input_text_file final
				: public core::files::file
			{
			public:
				input_text_file();
				input_text_file(path_view path);

				ANG_DECLARE_INTERFACE();

				bool open(path_view path);
				text::encoding_t format()const;
				file_cursor_t cursor()const;
				void cursor(file_cursor_t offset, file_reference_t ref = file_reference::begin);

				bool read(core::delegates::function<bool(streams::itext_input_stream_t)> , file_cursor_t offset = 0, wsize size = -1);
				template<typename T> inline core::async::iasync_t<T> read_async(core::delegates::function<T(streams::itext_input_stream_t)> callback, file_cursor_t offset = 0, wsize size = -1) {
					input_text_file_t file = this;
					return core::async::task::template run_async<T>([=](core::async::iasync<T>*, var_args_t)->T {
						T result;
						this->read([&](streams::itext_input_stream_t stream) {
							result = ang::move(callback(stream));
							return true;
						}, offset, size);
						return ang::move(result);
					}, var_args_t{ file , callback });
				}

				//wsize read(string& out, wsize count);
				//wsize read(wstring& out, wsize count);

				template<typename T>
				wsize read(T& value, text::text_format_t f = text::default_text_format<T>::format()) {
					return streams::text_deserializer<T>::deserialize(hfile.get(), value, f);
				}

				template<typename T>
				wsize read(safe_str<T>& value) {
					return streams::text_deserializer<safe_str<T>>::deserialize(hfile.get(), value);
				}

				template<text::encoding_enum ENCODING>
				wsize read(strings::string_base<ENCODING>& value, wsize max) {
					return streams::text_deserializer<strings::string_base<ENCODING>>::deserialize(hfile.get(), value, max);
				}

				wsize read(raw_str_t& out);

			private:
				virtual~input_text_file();
			};

			class LINK output_text_file final
				: public core::files::file
			{
			public:
				output_text_file();
				output_text_file(path_view path, text::encoding_t = text::encoding::ascii);

				ANG_DECLARE_INTERFACE();

				bool open(path_view path, text::encoding_t = text::encoding::ascii);
				text::encoding_t format()const;
				void format(text::encoding_t);
				file_cursor_t cursor()const;
				void cursor(file_cursor_t offset, file_reference_t ref = file_reference::begin);

				bool write(core::delegates::function<bool(streams::itext_output_stream_t)> , file_cursor_t offset = 0, wsize size = -1);			
				template<typename T> inline core::async::iasync_t<T> write_async(core::delegates::function<T(streams::itext_output_stream_t)> callback, file_cursor_t offset = 0, wsize size = -1) {
					output_text_file_t file = this;
					
					return core::async::task::template run_async<T>([=](core::async::iasync<T>*, var_args_t)->T {
						T result;
						this->write([&](streams::itext_output_stream_t stream) {
							result = ang::move(callback(stream));
							return true;
						}, offset, size);
						return ang::move(result);
					}, var_args_t{ file , callback });
				}

				template<typename T>
				wsize write(T const& value, text::text_format_t f = text::default_text_format<T>::format()) {
					return streams::text_serializer<T>::serialize(hfile.get(), value, f);
				}

				wsize write(raw_str_t out);

			private:
				virtual~output_text_file();
			};

			class LINK input_binary_file final
				: public core::files::file
			{
			public:
				input_binary_file();
				input_binary_file(path_view path);

				ANG_DECLARE_INTERFACE();

				bool open(path_view path);
				file_cursor_t cursor()const;
				void cursor(file_cursor_t offset, file_reference_t ref = file_reference::begin);

				bool read(core::delegates::function<bool(streams::ibinary_input_stream_t)> , file_cursor_t offset = 0, wsize size = -1);
				template<typename T> inline core::async::iasync_t<T> read_async(core::delegates::function<T(streams::ibinary_input_stream_t)> callback, file_cursor_t offset = 0, wsize size = -1) {
					input_binary_file_t file = this;
					return core::async::async_task<T>::run_async([=](core::async::iasync<T>* async, var_args_t args)->T {
						T result;
						this->read([&](streams::ibinary_input_stream_t stream) {
							result = ang::move(callback(stream));
							return true;
						}, offset, size);
						return ang::move(result);
					}, { file , callback });
				}

				wsize read(pointer out, wsize count);

			private:
				virtual~input_binary_file();
			};

			class LINK output_binary_file final
				: public core::files::file
			{
			public:
				output_binary_file();
				output_binary_file(path_view path);

				ANG_DECLARE_INTERFACE();

				bool open(path_view path);
				file_cursor_t cursor()const;
				void cursor(file_cursor_t offset, file_reference_t ref = file_reference::begin);

				bool write(core::delegates::function<bool(streams::ibinary_output_stream_t)> , file_cursor_t offset = 0, wsize size = -1);
				template<typename T> inline core::async::iasync_t<T> write_async(core::delegates::function<T(streams::ibinary_output_stream_t)> callback, file_cursor_t offset = 0, wsize size = -1) {
					output_binary_file_t file = this;
					return core::async::async_task<T>::run_async([=](core::async::iasync<T>* async, var_args_t args)->T {
						T result;
						this->write([&](streams::ibinary_output_stream_t stream) {
							result = ang::move(callback(stream));
							return true;
						}, offset, size);
						return ang::move(result);
					}, { file , callback });
				}

				wsize write(pointer out, wsize count);

			private:
				virtual~output_binary_file();
			};


			class LINK binary_file final
				: public core::files::file
			{
			public:
				binary_file();
				binary_file(path_view path);

				bool open(path_view path);
				file_cursor_t cursor()const;
				void cursor(file_cursor_t offset, file_reference_t ref = file_reference::begin);


				bool write(core::delegates::function<bool(streams::ibinary_output_stream_t)>, file_cursor_t offset = 0, wsize size = -1);
				bool read(core::delegates::function<bool(streams::ibinary_input_stream_t)>, file_cursor_t offset = 0, wsize size = -1);

				template<typename T> inline core::async::iasync_t<T> write_async(core::delegates::function<T(streams::ibinary_output_stream_t)> callback, file_cursor_t offset = 0, wsize size = -1) {
					binary_file_t file = this;
					return core::async::async_task<T>::run_async([=](core::async::iasync<T>* async, var_args_t args)->T {
						T result;
						this->write([&](streams::ibinary_output_stream_t stream) {
							result = ang::move(callback(stream));
							return true;
						}, offset, size);
						return ang::move(result);
					}, { file , callback });
				}

				template<typename T> inline core::async::iasync_t<T> read_async(core::delegates::function<T(streams::ibinary_input_stream_t)> callback, file_cursor_t offset = 0, wsize size = -1) {
					binary_file_t file = this;
					return core::async::async_task<T>::run_async([=](core::async::iasync<T>* async, var_args_t args)->T {
						T result;
						this->read([&](streams::ibinary_input_stream_t stream) {
							result = ang::move(callback(stream));
							return true;
						}, offset, size);
						return ang::move(result);
					},{ file , callback });
				}

				wsize write(pointer out, wsize count);
				wsize read(pointer out, wsize count);
			};


		}
	}
}



#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANG_FILES_H__