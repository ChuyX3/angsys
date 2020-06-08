#ifndef __ANG_FRAMEWORK_H__
#elif !defined __ANG_FRAMEWORK_MESHES_H__
#define __ANG_FRAMEWORK_MESHES_H__

namespace ang
{
	namespace graphics
	{

		namespace meshes
		{
			using namespace resources;

			class LINK material
				: public implement<material
				, iid("ang::graphics::meshes::material")
				, imaterial>
			{
			private:
				string m_tectnique_name;
				array<byte> m_data;
				reflect::varying m_fields;
				vector<textures::itexture_t> m_textures;

			public:
				material();

			protected:
				void dispose() override;

			public:
				string technique()const override;
				array<reflect::varying_desc> fields_layout()const override;
				array<reflect::varying> fields() override; //only one per time
				reflect::varying field(windex) override; //only one per time
				reflect::varying field(cstr_t) override; //only one per time
				array<textures::itexture_t> textures() const override;

			private:
				virtual~material();
			};

			class LINK geometry
				: public implement<geometry
				, iid("ang::graphics::meshes::geometry")
				, framework::aligned
				, igeometry>
			{
			private:
				maths::mat4 m_transform;
				buffers::iindex_buffer_t m_index_buffer;
				buffers::ivertex_buffer_t m_vertex_buffer;
				imaterial_t m_material;

			public:
				geometry();

			protected:
				void dispose() override;

			public:
				bool load(ifactory_t lib, geometry_data_t const& data);
				string technique() const override;
				buffers::iindex_buffer_t index_buffer() const override;
				buffers::ivertex_buffer_t vertex_buffer() const override;
				void transform(maths::mat4 const&) override;
				maths::mat4 const& transform() const override;
				imaterial_t material() const override;

			private:
				virtual~geometry();
			};

			class LINK mesh
				: public implement<mesh
				, iid("ang::graphics::meshes::mesh")
				, imesh
				, iresource>
			{
			private:
				string m_resource_id;
				vector<igeometry_t> m_geometries;

			public:
				mesh();

			protected: //override
				void dispose()override;

			public:
				bool load(ilibrary_t, dom::xml::ixml_node_t);
				core::async::iasync_op<bool> load_async(ilibrary_t, dom::xml::ixml_node_t);
				core::async::iasync_op<bool> load_async(ilibrary_t, core::files::input_text_file_t);
				core::async::iasync_op<bool> load_async(ilibrary_t, core::files::input_binary_file_t);

				string resource_sid()const override;
				void resource_sid(cstr_t) override;
				array_view<igeometry_t> elements()const override;

			private:
				resources::iresource_t resource()const override;
				resource_type_t resource_type()const override;
				intfptr fast_cast(resource_type_t) override;


			private:
				virtual~mesh();
			};

			class LINK mesh_loader
				: public implement<mesh_loader
				, iid("ang::graphics::meshes::mesh")
				, imesh_loader>
			{
			public:
				static vector<geometry_data> load_data(ilibrary_t, dom::xml::ixml_node_t, string_ptr = null);
				static vector<geometry_data> load_data(ilibrary_t, core::files::input_text_file_t, string_ptr = null);
				static vector<geometry_data> load_data(ilibrary_t, core::files::input_binary_file_t, string_ptr = null);
				static void load_element(ilibrary_t, dom::xml::ixml_node_t, vector<geometry_data>&);
				static void load_material_data(ilibrary_t, core::files::input_text_file_t, collections::hash_map<string, material_data>&);

			private:
				weak_ptr<ilibrary> m_parent;
				mutable core::async::mutex_t m_mutex;
				collections::hash_map<string, imesh_t> m_mesh_map;

			public:
				mesh_loader(ilibrary* parent);

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

				optional<imesh> load_mesh(dom::xml::ixml_node_t) override;
				core::async::iasync_op<imesh> load_mesh_async(dom::xml::ixml_node_t) override;
				imesh_t find_mesh(cstr_t)const override;

			public: //internal
				inline ilibrary_t parent()const {
					return m_parent.lock();
				}

			private:
				template<typename T>
				inline core::async::iasync<T> create_task(function<T(core::async::iasync<T>)> func)const {
					return dispatcher()->run_async<T>(ang::move(func));
				}

			private:
				virtual~mesh_loader();
			};
		}
	}
}

#endif/*__ANG_FRAMEWORK_MESHES_H__*/
