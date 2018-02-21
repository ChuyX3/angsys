#ifndef __ANG_FILE_SYSTEM_H__
#define __ANG_FILE_SYSTEM_H__

#include <ang/core/files.h>
#include <ang/collections/vector.h>

namespace ang
{
	namespace core
	{
		namespace files
		{
			class file_impl;
			class file_system;
			class folder_file_system;
			
			typedef object_wrapper<file_impl> system_file_t;
			typedef object_wrapper<file_system> file_system_t;

			class mapped_file_buffer 
				: public object
				, public ibuffer
			{
			private:
				system_file_t _original_source;
				pointer _buffer_ptr;
				wsize _buffer_size;
				file_size_t _buffer_offset;
				file_flags_t _access_flag;

			public:
				mapped_file_buffer(system_file_t file, file_flags_t access, uint size, ulong64 offset);
				
			private:
				virtual~mapped_file_buffer();

			public:
				ANG_DECLARE_INTERFACE();
				bool map(file_flags_t access, wsize size, ulong64 offset);
				bool unmap();
				system_file_t original_source()const;
				pointer buffer_ptr()const override;
				wsize buffer_size()const override;
				wsize mem_copy(wsize, pointer) override;
				pointer map_buffer(windex, wsize) override;
				void unmap_buffer(pointer, wsize) override;
				bool can_realloc_buffer()const override;
				bool realloc_buffer(wsize) override;
			};

			typedef object_wrapper<mapped_file_buffer> mapped_file_buffer_t;

#ifdef WINDOWS_PLATFORM
			typedef pointer file_handle_t;
#elif defined ANDROID_PLATFORM
			typedef int file_handle_t;
#endif

			class file_impl :
				public object,
				public ifile
			{
			private:
				file_handle_t _hfile;
				ulong64 _hmap_size;
				file_handle_t _hmap;
				path _path;
				file_flags_t _flags;
				long64 _size;
				long64 _cursor;
				core::async::mutex_t _mutex;
				wsize _map_counter;

			public:
				file_impl();

			private:
				virtual~file_impl();

			public: //Overrides
				ANG_DECLARE_INTERFACE();

				bool create(path_view path, open_flags_t flags);
				file_handle_t handle()const;
				file_handle_t map_handle(ulong64 min);
				bool is_created()const;
				bool close();

			public: //Methos
				virtual streams::stream_mode_t mode()const override;
				virtual path file_path()const override;
				virtual file_size_t file_size()const override;
				virtual bool file_size(file_size_t) override;
				virtual void encoding(text::encoding_t)override;
				virtual text::encoding_t encoding()const override;
				virtual void cursor(file_reference_t, file_cursor_t) override;
				virtual file_size_t cursor()const override;
				virtual wsize read(wsize size, pointer buffer)override;
				virtual wsize write(wsize size, pointer buffer)override;
				virtual wsize read(ibuffer_t buffer)override;
				virtual wsize write(ibuffer_t buffer)override;
				virtual ibuffer_t map(wsize size, file_size_t offset)override;
				virtual bool unmap(ibuffer_t)override;

				virtual bool set_mutex(core::async::mutex_t)override;
			//	virtual core::async::iasync_t<ibuffer_t> read_async(uint size)override;
			//	virtual core::async::iasync_t<uint> write_async(uint size, pointer buffer)override;
				bool unmap(mapped_file_buffer*);
			private:
				static file_size_t get_file_size(file_handle_t h);
				static text::encoding_t get_file_encoding(file_handle_t h);
				static void set_file_encoding(file_handle_t h, text::encoding_t);

			};


			class file_system
				: public object
				, public ifile_system
				, public singleton<file_system_t>
			{
			private:
				friend  singleton<file_system_t>;
				collections::vector<path> _paths;

				collections::vector<intf_wrapper<ifile_system>> highest_priority;
				collections::vector<intf_wrapper<ifile_system>> lowest_priority;

			public:
				file_system();
				virtual~file_system();

			public:
				ANG_DECLARE_INTERFACE();

			public:
				bool register_file_system(ifile_system*, file_system_priority_t);
				virtual array<path> paths()const override;
				virtual bool register_paths(path_view)override;
				virtual bool create_file_handle(path_view, open_flags_t, ifile_ptr_t)override;

				virtual bool open(path_view, input_text_file_t&)override;
				virtual bool open(path_view, output_text_file_t&)override;
				virtual bool open(path_view, input_binary_file_t&)override;
				virtual bool open(path_view, output_binary_file_t&)override;
			};


			class folder_file_system
				: public object
				, public ifile_system
			{
			private:
				path _root_path;
				collections::vector<path> _paths;

			public:
				folder_file_system(path);
				virtual~folder_file_system();

			public:
				ANG_DECLARE_INTERFACE();

			public:
				virtual array<path> paths()const override;
				virtual bool register_paths(path_view)override;
				virtual bool create_file_handle(path_view, open_flags_t, ifile_ptr_t)override;

				virtual bool open(path_view, input_text_file_t&)override;
				virtual bool open(path_view, output_text_file_t&)override;
				virtual bool open(path_view, input_binary_file_t&)override;
				virtual bool open(path_view, output_binary_file_t&)override;
			};

		}
	}

	template<> ang::core::files::file_system* ang::singleton<ang::core::files::file_system_t>::instance();
	template<> bool ang::singleton<ang::core::files::file_system_t>::release_instance();
}

#endif//__ANG_FILE_SYSTEM_H__