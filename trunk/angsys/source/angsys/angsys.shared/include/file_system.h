#ifndef __ANG_FILE_SYSTEM_H__
#define __ANG_FILE_SYSTEM_H__

#include <ang/core/files.h>

namespace ang
{
	namespace core
	{
		namespace files
		{
			class core_file;
			class file_system;
			class folder_file_system;

			typedef object_wrapper<core_file> system_file_t;
			typedef object_wrapper<file_system> file_system_t;

			open_flags_t get_format(text::encoding_t encoding);
			text::encoding_t get_format(open_flags_t encoding);

		}
	}

	namespace core
	{
		namespace files
		{

			class mapped_file_buffer 
				: public smart<mapped_file_buffer, ibuffer>
			{
			private:
				system_file_t m_original_source;
				pointer m_buffer_ptr;
				wsize m_buffer_size;
				file_size_t m_buffer_offset;
				open_flags_t m_access_flag;

			public:
				mapped_file_buffer(system_file_t file, open_flags_t access, wsize size, ulong64 offset);
				
			private:
				virtual~mapped_file_buffer();

			public:
				ANG_DECLARE_INTERFACE();
				bool map(open_flags_t access, wsize size, ulong64 offset);
				bool unmap();
				system_file_t original_source()const;
				bool is_readonly()const override;
				pointer buffer_ptr() override;
				const_pointer buffer_ptr()const override;
				wsize buffer_size()const override;
				wsize mem_copy(wsize, pointer, text::encoding_t) override;
				ibuffer_view_t map_buffer(windex, wsize) override;
				bool unmap_buffer(ibuffer_view_t&, wsize) override;
				bool can_realloc_buffer()const override;
				bool realloc_buffer(wsize) override;
				text::encoding_t encoding()const override;
			};

			typedef object_wrapper<mapped_file_buffer> mapped_file_buffer_t;

#ifdef WINDOWS_PLATFORM
			typedef pointer file_handle_t;
#elif defined ANDROID_PLATFORM
			typedef uint file_handle_t;
#endif

			class core_file final
				: public smart<core_file, ifile>
			{
			private:
				file_handle_t m_hfile;
				ulong64 m_hmap_size;
				file_handle_t m_hmap;
				path_t m_path;
				open_flags_t m_flags;
				//long64 _size;
				//long64 _cursor;
				core::async::mutex_ptr_t m_mutex;
				wsize m_map_counter;

			public:
				core_file();

			private:
				virtual~core_file();

			public: //Overrides
				ANG_DECLARE_INTERFACE();

				bool create(path_view_t path, open_flags_t flags);
				file_handle_t handle()const;
				file_handle_t map_handle(ulong64 min);
				bool is_created()const;
				void clear()override;

			public: //overrides
				virtual stream_mode_t mode()const override;
				virtual path_t path()const override;
				virtual text::encoding_t format()const override;			
				virtual file_size_t size()const override;
				virtual bool size(file_size_t) override;
				virtual bool is_eof()const override;
				virtual file_offset_t cursor()const override;
				virtual bool cursor(file_offset_t size, stream_reference_t ref)override;
				virtual file_offset_t read(ibuffer_view_t);
				virtual file_offset_t read(wsize, pointer);
				virtual file_offset_t write(ibuffer_view_t);
				virtual file_offset_t write(wsize, pointer);

				void format(text::encoding_t);

				virtual ibuffer_t map(wsize size, file_offset_t offset)override;
				virtual bool unmap(ibuffer_t, wsize used)override;
				virtual bool set_mutex(core::async::mutex_ptr_t)override;

				bool unmap(mapped_file_buffer*);
			private:
				static file_size_t get_file_size(file_handle_t h);
				static text::encoding_t get_file_encoding(file_handle_t h);
				static void set_file_encoding(file_handle_t h, text::encoding_t);

			};


			class file_system
				: public smart<file_system, ifile_system, singleton<file_system_t>>
			{
			private:
				friend  singleton<file_system_t>;
				collections::vector<path_t> m_paths;

				collections::vector<intf_wrapper<ifile_system>> m_highest_priority;
				collections::vector<intf_wrapper<ifile_system>> m_lowest_priority;

			public:
				file_system();
				virtual~file_system();

			public:
				ANG_DECLARE_INTERFACE();

			public:
				bool register_file_system(ifile_system*, file_system_priority_t);
				virtual array_view<path_t> paths(file_system_priority_t)const override;
				virtual void push_path(path_view_t, file_system_priority_t)override;
				virtual bool open_file(path_view_t, open_flags_t, ifile_ptr_t)override;

				virtual bool open(path_view_t, input_text_file_ptr_t)override;
				virtual bool open(path_view_t, output_text_file_ptr_t)override;
				virtual bool open(path_view_t, input_binary_file_ptr_t)override;
				virtual bool open(path_view_t, output_binary_file_ptr_t)override;
			};


			class folder_file_system
				: public smart<folder_file_system, ifile_system>
			{
			private:
				path_t m_root_path;
				collections::vector<path_t> m_paths;

			public:
				folder_file_system(path_t);
				virtual~folder_file_system();

			public:
				ANG_DECLARE_INTERFACE();

			public:
				virtual array_view<path_t> paths(file_system_priority_t)const override;
				virtual void push_path(path_view_t, file_system_priority_t)override;
				virtual bool open_file(path_view_t, open_flags_t, ifile_ptr_t)override;

				virtual bool open(path_view_t, input_text_file_ptr_t)override;
				virtual bool open(path_view_t, output_text_file_ptr_t)override;
				virtual bool open(path_view_t, input_binary_file_ptr_t)override;
				virtual bool open(path_view_t, output_binary_file_ptr_t)override;
			};

		}
	}

}

#endif//__ANG_FILE_SYSTEM_H__