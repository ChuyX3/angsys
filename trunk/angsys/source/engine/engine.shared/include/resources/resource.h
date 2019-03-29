#pragma once
namespace ang
{
	namespace graphics
	{
		namespace resources
		{
			class resource
				: public smart<resource, iresource>
			{
			protected:
				iresource_t m_resource;

			public:
				resource();
				ANG_DECLARE_INTERFACE();

			protected: //override
				void dispose()override;

			public: //override
				virtual void clear();
				virtual bool load(ilibrary_t, dom::xml::xml_node_t);
				virtual bool save(ilibrary_t, dom::xml::xml_document_t)const ;
				virtual void async_worker(ilibrary_t, core::async::idispatcher_t);
				virtual core::async::iasync<bool> load_async(ilibrary_t, dom::xml::xml_node_t);
				virtual core::async::iasync<bool> save_async(ilibrary_t, dom::xml::xml_document_t)const;

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
			
			protected:
				virtual~resource();
			};
		}
	}
}