#pragma once
#include <angsys.h>
#include <ang/core/files.h>
#include <ang/graphics/graphics.h>
#include <ang/collections/hash_map.h>

namespace ang
{
	namespace graphics
	{
		namespace resources
		{
			ang_object(resource);
			ang_object(library);	
			ang_object(resource_manager);
		}
		namespace buffers
		{
			ang_object(index_buffer);
			ang_object(vertex_buffer);
			ang_object(frame_buffer);
		}
		namespace textures
		{
			ang_object(texture);
			ang_object(texture_loader);	
		}
		namespace effects
		{
			using itechnique = ishaders;
			using itechnique_t = ishaders_t;

			ang_object(technique);
			ang_object(pass);
			ang_object(effect);
			ang_object(effect_library);			
		}
	}

	template<> struct is_inherited_from<graphics::resources::resource_manager, intf> : public true_type { };
	template<> struct is_inherited_from<graphics::resources::resource_manager, object> : public true_type { };

	namespace graphics
	{
		namespace resources
		{
			class resource
				: public smart<resource, dom::xml::ixml_serializable, iresource>
			{
			protected:
				weak_ptr<ilibrary> m_library;
				iresource_t m_resource;

			public:
				resource(ilibrary*);
				ANG_DECLARE_INTERFACE();

			public: //override
				void clear()override;
				bool load(dom::xml::xml_node_t)override;
				bool save(dom::xml::xml_document_t)const override;
				core::async::iasync<bool> load_async(dom::xml::xml_node_t)override;
				core::async::iasync<bool> save_async(dom::xml::xml_document_t)const override;

				resource_type_t resource_type()const override;
				string resource_sid()const override;
				void resource_sid(cstr_t) override;
				effects::ieffect_t to_effect() override;
				effects::ishaders_t to_shaders() override;
				textures::itexture_t to_texture() override;
				iframe_buffer_t to_frame_buffer() override;
				buffers::iindex_buffer_t to_index_buffer() override;
				buffers::ivertex_buffer_t to_vertex_buffer() override;

				inline void dettach() { m_resource = null; }
				inline void attach(iresource_t res) { m_resource = res; }
				inline iresource_t handle()const { return m_resource.get(); }
				inline void library(ilibrary* lib) { m_library = lib; }
				inline ilibrary_t library()const { return m_library.lock(); }

			protected:
				virtual~resource();
			};
		}
	
		namespace textures
		{
			using namespace resources;
			
			class texture : public resource
			{
			public:
				inline texture(ilibrary* lib)
					: resource(lib) {
				}

				inline itexture_t handle()const {
					return const_cast<texture*>(this)->to_texture();
				}

				bool load(dom::xml::xml_node_t);

			private: //override
				using resource::resource_type;
				using resource::resource_sid;
				using resource::to_shaders;
				using resource::to_texture;
				using resource::to_frame_buffer;
				using resource::to_index_buffer;
				using resource::to_vertex_buffer;
			};

			class texture_loader
				: public smart<texture_loader, itexture_loader>
			{
				weak_ptr<ilibrary> m_parent;
			public:
				texture_loader(ilibrary* parent);
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

				itexture_t load_texture(cstr_t sid) override;
				itexture_t load_texture(dom::xml::xml_node_t) override;
				core::async::iasync<itexture_t> load_texture_async(cstr_t sid) override;
				core::async::iasync<itexture_t> load_texture_async(dom::xml::xml_node_t) override;
				itexture_t find_texture(cstr_t)const override;

			private:
				virtual~texture_loader();
			};
		}
		namespace buffers
		{
			using namespace resources;

			class index_buffer
				: public resource
			{
			public:
				inline index_buffer(ilibrary* lib)
					: resource(lib) {
				}

				inline iindex_buffer_t handle()const {
					return const_cast<index_buffer*>(this)->to_index_buffer();
				}

				bool load(dom::xml::xml_node_t);

			private: //override
				using resource::resource_type;
				using resource::resource_sid;
				using resource::to_effect;
				using resource::to_shaders;
				using resource::to_texture;
				using resource::to_frame_buffer;
				using resource::to_index_buffer;
				using resource::to_vertex_buffer;
			};

			class vertex_buffer
				: public resource
			{
			public:
				inline vertex_buffer(ilibrary* lib)
					: resource(lib) {
				}

				inline ivertex_buffer_t handle()const {
					return const_cast<vertex_buffer*>(this)->to_vertex_buffer();
				}

				bool load(dom::xml::xml_node_t);

			private: //override
				using resource::resource_type;
				using resource::resource_sid;
				using resource::to_effect;
				using resource::to_shaders;
				using resource::to_texture;
				using resource::to_frame_buffer;
				using resource::to_index_buffer;
				using resource::to_vertex_buffer;
			};

			class frame_buffer
				: public resource
			{
			public:
				inline frame_buffer(ilibrary* lib)
					: resource(lib) {
				}

				inline iframe_buffer_t handle()const {
					return const_cast<frame_buffer*>(this)->to_frame_buffer();
				}

				bool load(dom::xml::xml_node_t);

			private: //override
				using resource::resource_type;
				using resource::resource_sid;
				using resource::to_effect;
				using resource::to_shaders;
				using resource::to_texture;
				using resource::to_frame_buffer;
				using resource::to_index_buffer;
				using resource::to_vertex_buffer;
			};
		}
		namespace effects
		{
			using namespace resources;

			class technique
				: public resource
			{
			public:
				inline technique(ilibrary* lib)
					: resource(lib) {
				}

				inline itechnique_t handle()const {
					return const_cast<technique*>(this)->to_shaders();
				}

				bool load(dom::xml::xml_node_t);
				bool load_shader(dom::xml::xml_node_t, shader_info_t& out);

			private: //override
				using resource::resource_type;
				using resource::resource_sid;
				using resource::to_effect;
				using resource::to_shaders;
				using resource::to_texture;
				using resource::to_frame_buffer;
				using resource::to_index_buffer;
				using resource::to_vertex_buffer;
			};

			class pass
				: public smart<pass, ipass>
			{
				weak_ptr<ilibrary> m_library;
				itechnique_t m_technique;

			public:
				pass(ilibrary* parent);
				ANG_DECLARE_INTERFACE();

			private:
				virtual~pass();
			};

			class effect
				: public smart<effect, ieffect, iresource, dom::xml::ixml_serializable>
			{
				weak_ptr<ilibrary> m_parent;
				string m_resource_sid;
				vector<pass_t> m_passes;
				collections::hash_map<string, iframe_buffer_t> m_fbo_map;

			public:
				effect(ilibrary* parent);

				ANG_DECLARE_INTERFACE();

			public: //overrides
				bool load(dom::xml::xml_node_t) override;
				bool save(dom::xml::xml_document_t)const override;
				
				iresource_t resource()const override;
				wsize pass_count()const override;
				ipass_t pass(windex)const override;
				iframe_buffer_t frame_buffer(cstr_t)const override;

				resource_type_t resource_type()const override;
				string resource_sid()const override;
				void resource_sid(cstr_t) override;

			private: //overrides
				core::async::iasync<bool> load_async(dom::xml::xml_node_t) override;
				core::async::iasync<bool> save_async(dom::xml::xml_document_t)const override;

				effects::ieffect_t to_effect() override;
				effects::ishaders_t to_shaders() override;
				textures::itexture_t to_texture() override;
				iframe_buffer_t to_frame_buffer() override;
				buffers::iindex_buffer_t to_index_buffer() override;
				buffers::ivertex_buffer_t to_vertex_buffer() override;

			private:
				virtual~effect();
				
			};

			class effect_library
				: public smart<effect_library, ieffect_library>
			{
				weak_ptr<ilibrary> m_parent;
				mutable core::async::mutex_t m_mutex;
				core::async::idispatcher_t m_async_worker;
				collections::hash_map<string, ieffect_t> m_effect_map;
				collections::hash_map<string, itechnique_t> m_technique_map;

			public:
				effect_library(ilibrary* parent);
				ANG_DECLARE_INTERFACE();

			public: //overrides
				void clear()override;

				ifactory_t factory()const override;
				bool load(dom::xml::xml_node_t)override;
				bool save(dom::xml::xml_document_t)const override;
				core::async::iasync<bool> load_async(dom::xml::xml_node_t)override;
				core::async::iasync<bool> save_async(dom::xml::xml_document_t)const override;
				core::async::idispatcher_t dispatcher()const override;

				bool load_sources(dom::xml::xml_node_t) override;
				ilibrary_t load_library(dom::xml::xml_node_t) override;
				iresource_t load_resource(dom::xml::xml_node_t) override;
				core::async::iasync<ilibrary_t> load_library_async(dom::xml::xml_node_t) override;
				core::async::iasync<iresource_t> load_resource_async(dom::xml::xml_node_t) override;

				string find_source(cstr_t)const override;
				iresource_t find_resource(cstr_t)const override;

				ieffect_t load_effect(dom::xml::xml_node_t) override;
				core::async::iasync<ieffect_t> load_effect_async(dom::xml::xml_node_t) override;
				itechnique_t load_technique(dom::xml::xml_node_t) override;
				core::async::iasync<itechnique_t> load_technique_async(dom::xml::xml_node_t) override;
				ieffect_t find_effect(cstr_t)const override;
				itechnique_t find_technique(cstr_t)const override;

			public: //internal
				inline ilibrary_t parent()const {
					return m_parent.lock();
				}

			private:
				virtual~effect_library();
			};
		}

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