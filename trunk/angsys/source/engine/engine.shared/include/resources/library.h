#pragma once
namespace ang
{
	namespace graphics
	{
		namespace resources
		{
			class library
				: public smart<library, ilibrary>
			{
			protected:
				weak_ptr<ilibrary> m_parent;
				mutable core::async::mutex_t m_mutex;
				core::async::idispatcher_t m_async_worker;
				collections::hash_map<string, weak_ptr<iresource>> m_resource_map;
				effects::effect_library_t m_fx_library;
				textures::texture_loader_t m_tex_loader;

			public:
				library(ilibrary* parent);

				ANG_DECLARE_INTERFACE();

			public: //overrides
				void clear()override;

				ifactory_t factory()const override;
				bool load(dom::xml::xml_node_t)override;
				bool save(dom::xml::xml_document_t)const override;
				core::async::iasync<bool> load_async(dom::xml::xml_node_t)override;
				core::async::iasync<bool> save_async(dom::xml::xml_document_t)const override;

				bool load_sources(dom::xml::xml_node_t) override;
				ilibrary_t load_library(dom::xml::xml_node_t) override;
				iresource_t load_resource(dom::xml::xml_node_t) override;
				core::async::iasync<ilibrary_t> load_library_async(dom::xml::xml_node_t) override;
				core::async::iasync<iresource_t> load_resource_async(dom::xml::xml_node_t) override;
				core::async::idispatcher_t dispatcher()const override;
				string find_source(cstr_t)const override;
				iresource_t find_resource(cstr_t)const override;



			public: //internal
				inline ilibrary_t parent()const {
					return m_parent.lock();
				}


			protected:
				virtual~library();
			};

			class resource_manager
				: public smart<resource_manager
				, library
				, core::files::ifile_system
				, singleton<resource_manager_t>>
			{
				friend singleton<resource_manager_t>;
				weak_ptr<ifactory> m_factory;
				core::async::thread_t m_worker_tread;
				collections::hash_map<string, string> m_file_map;

			public:
				using singleton<resource_manager_t>::instance;

			public:
				resource_manager();

				ANG_DECLARE_INTERFACE();

			public: //overrides
				void clear()override;

				collections::ienum_ptr<string> paths(core::files::path_access_type_t)const override;
				void push_path(cstr_t, core::files::path_access_type_t, cstr_t macro = null) override;
				collections::ienum_ptr<string> find_paths(cstr_t macro)const override;
				core::files::path_access_type_t path_access_type(cstr_t)const override;
				bool open_file(cstr_t, core::files::open_flags_t, core::files::ifile_ptr_t, cstr_t macro = null)override;
				bool open(cstr_t, core::files::input_text_file_ptr_t, cstr_t macro = null)override;
				bool open(cstr_t, core::files::output_text_file_ptr_t, cstr_t macro = null)override;
				bool open(cstr_t, core::files::input_binary_file_ptr_t, cstr_t macro = null)override;
				bool open(cstr_t, core::files::output_binary_file_ptr_t, cstr_t macro = null)override;

				ifactory_t factory()const override;
				string find_source(cstr_t)const override;
				iresource_t find_resource(cstr_t)const override;
				bool load_sources(dom::xml::xml_node_t sources)override;

			public: //internal
				inline void factory(ifactory_t factory) {
					m_factory = factory.get();
					if (!factory.is_empty()) {
						factory->set_file_system(this);
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