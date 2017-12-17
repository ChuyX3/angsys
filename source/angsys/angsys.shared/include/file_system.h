#ifndef __ANG_FILE_SYSTEM_H__
#define __ANG_FILE_SYSTEM_H__

#include <ang/core/files.hpp>
#include <ang/collections/vector.hpp>

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

			open_flags_t get_format(text::encoding_t encoding);
			text::encoding_t get_format(open_flags_t encoding);

		}
	}

	template<> class object_wrapper<core::files::file_impl>
	{
	public:
		typedef core::files::file_impl type;

	private:
		core::files::file_impl* _ptr;

	public:
		object_wrapper();
		object_wrapper(core::files::file_impl*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		core::files::file_impl* get(void)const;
		void set(core::files::file_impl*);
		core::files::file_impl ** addres_of(void);

	public:
		object_wrapper& operator = (core::files::file_impl*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<core::files::file_impl> operator & (void);
		core::files::file_impl * operator -> (void);
		core::files::file_impl const* operator -> (void)const;
		operator core::files::file_impl * (void);
		operator core::files::file_impl const* (void)const;

	};



	namespace core
	{
		namespace files
		{

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
				core::async::mutex_ptr_t _mutex;
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
				virtual file_size_t stream_size()const override;
				virtual bool stream_size(file_size_t) override;
				virtual void format(text::encoding_t)override;
				virtual text::encoding_t format()const override;
				virtual bool move_to(file_cursor_t, file_reference_t) override;
				virtual file_size_t position()const override;
				virtual wsize read(pointer buffer, wsize size,  text::text_format_t)override;
				virtual wsize write(pointer buffer, wsize size,  text::text_format_t)override;
				virtual wsize read(pointer buffer, wsize size, text::encoding_t)override;
				virtual wsize write(pointer buffer, wsize size, text::encoding_t)override;
				virtual wsize read(ibuffer_view_t buffer, text::encoding_t = text::encoding::binary)override;
				virtual wsize write(ibuffer_view_t buffer, text::encoding_t = text::encoding::binary)override;
				virtual ibuffer_t map(wsize size, file_cursor_t offset)override;
				virtual bool unmap(ibuffer_t, wsize)override;
				virtual bool set_mutex(core::async::mutex_ptr_t)override;

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

	//template<> ang::core::files::file_system* ang::singleton<ang::core::files::file_system_t>::instance();
	//template<> bool ang::singleton<ang::core::files::file_system_t>::release_instance();
}

#endif//__ANG_FILE_SYSTEM_H__