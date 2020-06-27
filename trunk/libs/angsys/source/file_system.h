/*********************************************************************************************************************/
/*   File Name: file_systm.h                                                                                         */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

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

#ifdef WINDOWS_PLATFORM
			static constexpr integer_constant<text::encoding, text::encoding::unicode> path_encoding;
#else
			static constexpr integer_constant<text::encoding, text::encoding::ascii> path_encoding;
#endif
			using path_t = fast_string<path_encoding>;
		}
	}

	namespace core
	{
		namespace files
		{
			class mapped_file_buffer 
				: public implement<mapped_file_buffer
				, iid("ang::core::files::mapped_file_buffer")
				, ibuffer
				, ibuffer_view>
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
				: public implement<core_file
				, iid("ang::core::files::core_file")
				, ifile>
			{
			private:
				file_handle_t m_hfile;
				ulong64 m_hmap_size;
				file_handle_t m_hmap;
				path_t m_path;
				open_flags_t m_flags;
				//long64 _cursor;
				core::async::mutex_ptr_t m_mutex;
				wsize m_map_counter;
				friend mapped_file_buffer;

			public:
				core_file();

			private:
				virtual~core_file();

			protected: //overrides
				void dispose()override;

			public: //Overrides
				bool create(path_t&& path, open_flags_t flags);
				file_handle_t handle()const;
				file_handle_t map_handle(ulong64 min);
				bool is_created()const;

			public: //overrides
				virtual stream_mode_t mode()const override;
				virtual string path()const override;
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
				: public implement<file_system
				, iid("ang::core::files::file_system")
				, ifile_system>
			{
			private:
				async::mutex_t m_mutex;
				async::idispatcher_t m_async_worker;
				collections::hash_map<path_t, path_access_type_t> m_paths;
				collections::hash_map<string, vector<path_t>> m_macros;
				vector<intf_wrapper<ifile_system>> m_highest_priority;
				vector<intf_wrapper<ifile_system>> m_lowest_priority;

			public:
				file_system();
				virtual~file_system();

			public:
				virtual void dispose() override;

			public:
				bool register_file_system(ifile_system*, file_system_priority_t);
				virtual vector<string> paths(path_access_type_t)const override;
				virtual void push_path(cstr_t, path_access_type_t, cstr_t macro = null)override;
				virtual vector<string> find_paths(cstr_t)const override;
				virtual path_access_type_t path_access_type(cstr_t path)const override;
				virtual bool create_handle(cstr_t, open_flags_t, ifile_ptr_t, cstr_t macro = null)override;
				virtual async::iasync_op<ifile> create_handle_async(cstr_t path, open_flags_t flags, cstr_t macro = null) override;

				virtual bool open(cstr_t, input_text_file_ptr_t, cstr_t macro = null)override;
				virtual bool open(cstr_t, output_text_file_ptr_t, cstr_t macro = null)override;
				virtual bool open(cstr_t, input_binary_file_ptr_t, cstr_t macro = null)override;
				virtual bool open(cstr_t, output_binary_file_ptr_t, cstr_t macro = null)override;
			};


			class folder_file_system
				: public implement<folder_file_system
				, iid("ang::core::files::folder_file_system")
				, ifile_system>
			{
			private:
				path_t m_root_path;
				smart_vector<collections::tuple<path_access_type_t, path_t>> m_paths;
				collections::hash_map<path_t, path_t> m_macros;

			public:
				folder_file_system(path_t);
				virtual~folder_file_system();

			public:
				virtual vector<string> paths(path_access_type_t)const override;
				virtual void push_path(cstr_t, path_access_type_t, cstr_t macro = null)override;
				virtual vector<string> find_paths(cstr_t)const override;
				virtual bool create_handle(cstr_t, open_flags_t, ifile_ptr_t, cstr_t macro = null)override;

				virtual bool open(cstr_t, input_text_file_ptr_t, cstr_t macro = null)override;
				virtual bool open(cstr_t, output_text_file_ptr_t, cstr_t macro = null)override;
				virtual bool open(cstr_t, input_binary_file_ptr_t, cstr_t macro = null)override;
				virtual bool open(cstr_t, output_binary_file_ptr_t, cstr_t macro = null)override;
			};

		}
	}

}

#endif//__ANG_FILE_SYSTEM_H__
