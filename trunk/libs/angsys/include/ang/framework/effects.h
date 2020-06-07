#ifndef __ANG_FRAMEWORK_H__
#elif !defined __ANG_FRAMEWORK_EFFECTS_H__
#define __ANG_FRAMEWORK_EFFECTS_H__

namespace ang
{
	namespace graphics
	{
		namespace meshes
		{
			ang_declare_object(quad_geometry);
			ang_declare_object(quad_material);


			class LINK quad_material
				: public implement<quad_material
				, iid("ang::graphics::meshes::quad_material")
				, meshes::imaterial>
			{
			private:
				vector<textures::itexture_t> m_textures;

			public:
				quad_material();

			public: //overrides
				string technique()const override;
				array_view<reflect::varying_desc> fields_layout()const override;
				reflect::varying fields() override;
				reflect::varying field(windex) override;
				reflect::varying field(cstr_t) override;
				array_view<textures::itexture_t> textures() const override;

				void push_texture(textures::itexture_t tex);

				void clear();

			private: //overrides
				void dispose()override;

			private:
				virtual~quad_material();
			};

			class LINK quad_geometry
				: public implement<quad_geometry
				, iid("ang::graphics::meshes::quad_geometry")
				, framework::aligned
				, meshes::igeometry>
			{
			private:

				maths::mat4 m_transform;
				quad_material_t m_material;
				buffers::ivertex_buffer_t m_vertex_buffer;

			public:
				quad_geometry();

			public: //overrides
				string technique() const override;
				buffers::iindex_buffer_t index_buffer() const override;
				buffers::ivertex_buffer_t vertex_buffer() const override;
				meshes::imaterial_t material() const override;
				void transform(maths::mat4 const&) override;
				maths::mat4 const& transform() const override;

				void push_texture(textures::itexture_t tex) {
					m_material->push_texture(tex);
				}

				void clear() {
					m_material->clear();
				}

			private: //overrides
				void dispose()override;

			private:
				virtual~quad_geometry();
			};

		}

		namespace effects
		{
			using namespace resources;

			typedef struct frame_buffer_data : frame_buffer_desc
			{
				mutable iframe_buffer_t fbo;

				frame_buffer_data() {
					static textures::tex_format_t s_color_format[] = { textures::tex_format::R32G32B32A32 };
					color_format = to_array(s_color_format);
					depth_stencil_format = textures::tex_format::D24S8;
					dimentions = { 512,512 };
					is_stereo = false;
					fbo = null;
				}
				frame_buffer_data(frame_buffer_desc&& desc) {
					color_format = ang::move(desc.color_format);
					depth_stencil_format = ang::move(desc.depth_stencil_format);
					dimentions = ang::move(desc.dimentions);
					is_stereo = desc.is_stereo;
					fbo = null;
				}
				frame_buffer_data(frame_buffer_data const& other) {
					color_format = other.color_format;
					depth_stencil_format = other.depth_stencil_format;
					dimentions = other.dimentions;
					is_stereo = other.is_stereo;
					fbo = other.fbo;
				}
			}frame_buffer_data_t, * frame_buffer_data_ptr_t;

			class LINK pass
				: public implement<pass
				, iid("ang::graphics::effects::pass")
				, ipass>
			{
			private:
				vector<string> m_resources;
				string m_frame_buffer;
				string m_geometry;
				ishaders_t m_shaders;

			public:
				pass();

			protected: //override
				void dispose()override;

			public: //overrides	
				virtual bool load(ilibrary_t lib, dom::xml::ixml_node_t);
				virtual bool save(ilibrary_t lib, dom::xml::ixml_document_t)const;

				void draw(idriver_t, scenes::iscene_t, meshes::igeometry_t)override;
				effects::ishaders_t shaders()const override;
				string frame_buffer()const override;
				string geometry()const override;
				array_view<string> resources()const override;
			private:
				virtual~pass();
			};

			class LINK effect
				: public implement<effect
				, iid("ang::graphics::effects::effect")
				, ieffect
				, iresource>
			{
				string m_resource_sid;
				//core::async::mutex_t m_mutex;
				vector<pass_t> m_passes;
				collections::hash_map<string, frame_buffer_data_t> m_fbo_map;
				meshes::quad_geometry_t m_quad_model;
				weak_ptr<ilibrary> m_library;

			public:
				effect();

			public: //overrides
				virtual bool load(ilibrary_t lib, dom::xml::ixml_node_t);
				virtual bool save(ilibrary_t lib, dom::xml::ixml_document_t)const;

				resources::iresource_t resource()const override;
				void draw(idriver_t, iframe_buffer_t, scenes::iscene_t, scenes::inode_t)override;
				wsize pass_count()const override;
				ipass_t pass(windex)const override;
				iframe_buffer_t frame_buffer(cstr_t)const override;
				iframe_buffer_t frame_buffer(cstr_t, size<uint> resize)const override;

				resource_type_t resource_type()const override;
				string resource_sid()const override;
				void resource_sid(cstr_t) override;

			private: //overrides
				void dispose()override;
				intfptr fast_cast(resources::resource_type_t) override;

			private:
				bool load_frame_buffer(dom::xml::ixml_node_t);
				iframe_buffer_t default_fbo()const;
				void default_fbo(iframe_buffer_t);

			private:
				virtual~effect();

			};

			class LINK effect_library
				: public implement<effect_library
				, iid("ang::graphics::effects::effect_library")
				, ieffect_library>
			{
				weak_ptr<ilibrary> m_parent;
				mutable core::async::mutex_t m_mutex;
				collections::hash_map<string, ieffect_t> m_effect_map;
				collections::hash_map<string, ishaders_t> m_technique_map;

			public:
				effect_library(ilibrary* parent);

			private: //overrides
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

				optional<ieffect> load_effect(dom::xml::ixml_node_t) override;
				core::async::iasync_op<ieffect> load_effect_async(dom::xml::ixml_node_t) override;
				optional<ishaders> load_technique(dom::xml::ixml_node_t) override;
				core::async::iasync_op<ishaders> load_technique_async(dom::xml::ixml_node_t) override;
				ieffect_t find_effect(cstr_t)const override;
				ishaders_t find_technique(cstr_t)const override;

			public: //internal
				inline ilibrary_t parent()const {
					return m_parent.lock();
				}

			private:
				bool load_shader_info(dom::xml::ixml_node_t data, shader_info_t& info);
				template<typename T>
				inline core::async::iasync<T> create_task(function<T(core::async::iasync<T>)> func)const {
					return dispatcher()->run_async<T>(ang::move(func));
				}

			private:
				virtual~effect_library();
			};
		}
	}
}

#endif/*__ANG_FRAMEWORK_EFFECTS_H__*/