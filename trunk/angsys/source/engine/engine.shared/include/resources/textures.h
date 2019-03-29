#pragma once

namespace ang
{
	namespace graphics
	{
		namespace textures
		{
			using namespace resources;
			
			class texture : public resource
			{
			public:
				inline texture()
					: resource() {
				}

				inline itexture_t handle()const {
					return const_cast<texture*>(this)->to_texture();
				}

				bool load(ilibrary_t lib, dom::xml::xml_node_t);

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
	}
}