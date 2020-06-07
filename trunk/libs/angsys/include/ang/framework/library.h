#ifndef __ANG_FRAMEWORK_H__
#elif !defined __ANG_FRAMEWORK_LIBRARY_H__
#define __ANG_FRAMEWORK_LIBRARY_H__

namespace ang
{
	namespace graphics
	{
		namespace resources
		{
			class library
				: public implement<library
				, iid("ang::graphics::resources::library")
				, ilibrary>
			{
			protected:
				weak_ptr<ilibrary> m_parent;
				mutable core::async::mutex_t m_mutex;
				core::async::thread_t m_async_worker;
				effects::effect_library_t m_fx_library;
				textures::texture_loader_t m_tex_loader;

			public:
				library(ilibrary* parent);

			protected: //overrides
				void dispose()override;

			public: //overrides
				void clear()override;
				ifactory_t factory()const override;
				core::files::ifile_system_t file_system()const override;
				core::async::idispatcher_t dispatcher()const override;
				bool load(dom::xml::ixml_node_t)override;
				bool save(dom::xml::ixml_document_t)const override;
				core::async::iasync<bool> load_async(dom::xml::ixml_node_t)override;
				core::async::iasync<bool> save_async(dom::xml::ixml_document_t)const override;

				bool load_sources(dom::xml::ixml_node_t) override;
				optional<ilibrary> load_library(dom::xml::ixml_node_t) override;
				optional<iresource> load_resource(dom::xml::ixml_node_t) override;
				core::async::iasync_op<ilibrary> load_library_async(dom::xml::ixml_node_t) override;
				core::async::iasync_op<iresource> load_resource_async(dom::xml::ixml_node_t) override;
				string find_source(cstr_t)const override;
				iresource_t find_resource(cstr_t)const override;
				core::async::iasync<iresource_t> find_resource_async(cstr_t) override;

				effects::ieffect_library_t fx_library()const { return m_fx_library.get(); }
				textures::itexture_loader_t tex_loader()const { return m_tex_loader.get(); }

			public:
				inline ilibrary_t parent()const {
					return m_parent.lock();
				}

			protected:
				template<typename T>
				inline core::async::iasync<T> create_task(function<T(core::async::iasync<T>)> func)const {
					return dispatcher()->run_async<T>(ang::move(func));
				}

			protected:
				virtual~library();
			};


			class resource_manager
				: public implement<resource_manager
				, iid("ang::graphics::resources::resource_manager")
				, library
				, core::files::ifile_system>
			{
			private:
				ifactory_t m_factory;
				core::async::mutex_t m_fs_mutex;
				core::files::ifile_system_t m_file_system;
				collections::hash_map<string, string> m_file_map;

			public:
				resource_manager();

			private:
				void dispose()override;

			public: //overrides
				void clear()override;

				vector<string> paths(core::files::path_access_type_t)const override;
				void push_path(cstr_t, core::files::path_access_type_t, cstr_t macro = null) override;
				vector<string> find_paths(cstr_t macro)const override;
				core::files::path_access_type_t path_access_type(cstr_t)const override;
				bool create_handle(cstr_t path, core::files::open_flags_t flags, core::files::ifile_ptr_t out, cstr_t macro = null) override;
				core::async::iasync_op<core::files::ifile_t> create_handle_async(cstr_t path, core::files::open_flags_t flags, cstr_t macro = null) override;
				bool open(cstr_t, core::files::input_text_file_ptr_t, cstr_t macro = null)override;
				bool open(cstr_t, core::files::output_text_file_ptr_t, cstr_t macro = null)override;
				bool open(cstr_t, core::files::input_binary_file_ptr_t, cstr_t macro = null)override;
				bool open(cstr_t, core::files::output_binary_file_ptr_t, cstr_t macro = null)override;

				ifactory_t factory()const override;
				core::files::ifile_system_t file_system()const override;
				string find_source(cstr_t)const override;
				bool load_sources(dom::xml::ixml_node_t sources)override;

				using library::load;
				using library::load_async;
				using library::load_library;
				using library::load_library_async;

				bool load(cstr_t);
				core::async::iasync<bool> load_async(cstr_t);
				optional<ilibrary> load_library(cstr_t);
				core::async::iasync_op<ilibrary> load_library_async(cstr_t);


			public: //internal
				inline void factory(ifactory_t factory) {
					if (!factory.is_empty()) {
						m_factory = factory.get();
						factory->set_file_system(this);
						//m_worker_tread = factory->driver()->dispatcher();
					}
					else
					{
						m_factory = null;
						//m_worker_tread = null;
					}
				}

			private:
				using library::parent;

			private:
				virtual~resource_manager();
			};
		}
	}
}

#endif/*__ANG_FRAMEWORK_LIBRARY_H__*/