#pragma once

namespace ang
{
	namespace graphics
	{
		namespace effects
		{
			using namespace resources;

			class technique
				: public resource
			{
			public:
				inline technique()
					: resource() {
				}

				inline itechnique_t handle()const {
					return const_cast<technique*>(this)->to_shaders();
				}

				bool load(ilibrary_t lib, dom::xml::xml_node_t);
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
				: public smart<effect, ieffect, iresource>
			{
				string m_resource_sid;
				vector<pass_t> m_passes;
				collections::hash_map<string, iframe_buffer_t> m_fbo_map;

			public:
				effect();

				ANG_DECLARE_INTERFACE();

			public: //overrides
				virtual bool load(ilibrary_t lib, dom::xml::xml_node_t);
				virtual bool save(ilibrary_t lib, dom::xml::xml_document_t)const;
				
				iresource_t resource()const override;
				wsize pass_count()const override;
				ipass_t pass(windex)const override;
				iframe_buffer_t frame_buffer(cstr_t)const override;

				resource_type_t resource_type()const override;
				string resource_sid()const override;
				void resource_sid(cstr_t) override;

			private: //overrides
				//void async_worker(core::async::idispatcher_t)override { }
				//core::async::iasync<bool> load_async(ilibrary_t lib, dom::xml::xml_node_t);
				//core::async::iasync<bool> save_async(ilibrary_t lib, dom::xml::xml_document_t)const;

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
	}
}