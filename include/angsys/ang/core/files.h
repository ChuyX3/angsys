/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_FILES_H__
#define __ANG_FILES_H__

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

				type_binary = 0x0001,
				type_text = 0x0002,
				type_pack = 0x0003,

				encoding_ascii = 0x0010,
				encoding_mbyte = 0x0020,
				encoding_unicode = 0x0030,

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
				//wstring path;
				file_size_t size;
				file_cursor_t offset;
			}pack_file_info_t;

			//inline bool operator == (pack_file_info_t const& a1, pack_file_info_t const& a2) { return a1.path == a2.path; }
			//inline bool operator != (pack_file_info_t const& a1, pack_file_info_t const& a2) { return a1.path != a2.path; }


			ANG_BEGIN_INTERFACE(LINK, ifile)
				visible vcall streams::stream_mode_t mode()const pure;
				visible vcall wstring file_path()const pure;
				visible vcall file_size_t file_size()const pure;
				visible vcall void encoding(text::encoding_t) pure;
				visible vcall text::encoding_t encoding()const pure;
				visible vcall void cursor(file_reference_t, file_cursor_t) pure;
				visible vcall file_cursor_t cursor()const pure;
				visible vcall wsize read(wsize size, pointer buffer)pure;
				visible vcall wsize write(wsize size, pointer buffer)pure;
				visible vcall wsize read(ibuffer_t buffer)pure;
				visible vcall wsize write(ibuffer_t buffer)pure;
				visible vcall ibuffer_t map(wsize size, file_cursor_t offset)pure;
				visible vcall bool unmap(ibuffer_t)pure;
				visible vcall bool set_mutex(core::async::mutex_t)pure;
				//visible vcall core::async::iasync_t<ibuffer_t> read_async(uint size)pure;
				//visible vcall core::async::iasync_t<uint> write_async(uint size, pointer buffer)pure;
			ANG_END_INTERFACE();


			ANG_BEGIN_INTERFACE(LINK, ifile_system)
				visible static ifile_system* get_file_system();
				visible static bool register_file_system(ifile_system*, file_system_priority_t);
				visible vcall array<wstring> paths()const pure;
				visible vcall bool register_paths(cwstr_t) pure;
				visible vcall bool create_file_handle(cwstr_t, open_flags_t, ifile_ptr_t) pure;

				visible vcall bool open(cwstr_t, input_text_file_t&)pure;
				visible vcall bool open(cwstr_t, output_text_file_t&)pure;
				visible vcall bool open(cwstr_t, input_binary_file_t&)pure;
				visible vcall bool open(cwstr_t, output_binary_file_t&)pure;
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
				bool create(cwstr_t path, open_flags_t flags);
				ibuffer_t map(wsize, file_cursor_t);
				bool unmap(ibuffer_t);

			public: //Overrides
				ANG_DECLARE_INTERFACE();
				bool attach(ifile*);
				bool is_valid()const;
				file_size_t file_size()const;
				bool file_size(file_size_t);
				streams::stream_mode_t mode()const;
				bool set_mutex(core::async::mutex_t);
				virtual bool close();				

				friend pack_file;
			};
		
			class LINK pack_file
				: public file
				, public ifile_system
			{
			public:
				static bool create_pack_from_folder(cwstr_t in_path, cwstr_t out_path);

			private:
				core::async::mutex_t mutex;
				array<collections::pair<wstring, pack_file_info>> files;
						
			public:
				pack_file();

			public: //Overrides
				ANG_DECLARE_INTERFACE();

				bool create(cwstr_t path, open_flags_t flags);

				array<wstring> paths()const override;
				bool register_paths(cwstr_t) override;
				bool create_file_handle(cwstr_t, open_flags_t, ifile_ptr_t) override;

				bool open(cwstr_t path, input_text_file_t&);
				bool open(cwstr_t path, output_text_file_t&);
				bool open(cwstr_t path, input_binary_file_t&);
				bool open(cwstr_t path, output_binary_file_t&);

			protected:
				virtual~pack_file();
			};

			class LINK input_text_file final
				: public core::files::file
			{
			public:
				input_text_file();
				input_text_file(cwstr_t path);

				ANG_DECLARE_INTERFACE();

				bool open(cwstr_t path);
				text::encoding_t format()const;
				file_cursor_t cursor()const;
				void cursor(file_cursor_t offset, file_reference_t ref = file_reference::begin);

				bool read(core::delegates::function<bool(streams::itext_input_stream_t)> , file_cursor_t offset = 0, wsize size = -1);
				template<typename T> inline core::async::iasync_t<T> read_async(core::delegates::function<T(streams::itext_input_stream_t)> callback, file_cursor_t offset = 0, wsize size = -1) {
					input_text_file_t file = this;
					return core::async::async_task<T>::run_async([=](core::async::iasync<T>* async, var_args_t args)->T {
						T result;
						this->read([&](streams::itext_input_stream_t stream) {
							result = ang::move(callback(stream));
							return true;
						}, offset, size);
						return ang::move(result);
					}, { file , callback });
				}

				wsize read(string& out, wsize count);
				wsize read(wstring& out, wsize count);
				wsize read(mstring& out, wsize count);

			private:
				virtual~input_text_file();
			};

			class LINK output_text_file final
				: public core::files::file
			{
			public:
				output_text_file();
				output_text_file(cwstr_t path);

				ANG_DECLARE_INTERFACE();

				bool open(cwstr_t path);
				text::encoding_t format()const;
				void format(text::encoding_t);
				file_cursor_t cursor()const;
				void cursor(file_cursor_t offset, file_reference_t ref = file_reference::begin);

				bool write(core::delegates::function<bool(streams::itext_output_stream_t)> , file_cursor_t offset = 0, wsize size = -1);			
				template<typename T> inline core::async::iasync_t<T> write_async(core::delegates::function<T(streams::itext_output_stream_t)> callback, file_cursor_t offset = 0, wsize size = -1) {
					output_text_file_t file = this;
					return core::async::async_task<T>::run_async([=](core::async::iasync<T>* async, var_args_t args)->T {
						T result;
						this->write([&](streams::itext_output_stream_t stream) {
							result = ang::move(callback(stream));
							return true;
						}, offset, size);
						return ang::move(result);
					}, { file , callback });
				}

				wsize write(cstr_t out);
				wsize write(cwstr_t out);
				wsize write(cmstr_t out);

			private:
				virtual~output_text_file();
			};

			class LINK input_binary_file final
				: public core::files::file
			{
			public:
				input_binary_file();
				input_binary_file(cwstr_t path);

				ANG_DECLARE_INTERFACE();

				bool open(cwstr_t path);
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
				output_binary_file(cwstr_t path);

				ANG_DECLARE_INTERFACE();

				bool open(cwstr_t path);
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
				binary_file(cwstr_t path);

				bool open(cwstr_t path);
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