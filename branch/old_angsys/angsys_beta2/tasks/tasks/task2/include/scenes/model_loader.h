#pragma once
#include <ang/graphics/angraph.hpp>

namespace ang
{
	namespace graphics
	{
		namespace scenes
		{

			class ordered_model;
			class indexed_model;
			typedef object_wrapper<ordered_model> ordered_model_t;
			typedef object_wrapper<indexed_model> indexed_model_t;

			class ordered_model
				: public object
			{
			public:
				struct vertex
				{
					maths::float4 position;
					maths::float4 normal;
					maths::float2 texcoord;

					bool operator == (const vertex&)const { return false; }//dummy
					bool operator != (const vertex&)const { return false; }//dummy
				};

				/*See more: http://paulbourke.net/dataformats/mtl/ */
				struct material
				{
					/*"illum" statement specifies the illumination model to use in the material.
					    It can be a number from 0 to 10:
					 0	Color on and Ambient off
					 1	Color on and Ambient on
					 2	Highlight on
					 3	Reflection on and Ray trace on
					 4	Transparency: Glass on
 						Reflection: Ray trace on
					 5	Reflection: Fresnel on and Ray trace on
					 6	Transparency: Refraction on
 						Reflection: Fresnel off and Ray trace on
					 7	Transparency: Refraction on
 						Reflection: Fresnel on and Ray trace on
					 8	Reflection on and Ray trace off
					 9	Transparency: Glass on
 						Reflection: Ray trace off
					 10	Casts shadows onto invisible surfaces  */
					uint illum;

					/*"d factor" specifies the dissolve for the current material
					"factor" is the amount this material dissolves into the background.  A 
					factor of 1.0 is fully opaque.  This is the default when a new material 
					is created.  A factor of 0.0 is fully dissolved (completely 
					transparent)
					"d -halo factor" specifies that a dissolve is dependent on the surface orientation 
					relative to the viewer.  For example, a sphere with the following dissolve,
					d -halo 0.0, will be fully dissolved at its center and will 
					appear gradually more opaque toward its edge.*/
					float d;

					/*"Ns" exponent specifies the specular exponent for the current material.  This defines 
					the focus of the specular highlight.*/
					float Ns;

					/*"Ka" statement specifies the ambient reflectivity of the current material*/
					maths::unaligned_float3 Ka;

					/*"Kd" statement specifies the diffuse reflectivity of the current material*/
					maths::unaligned_float3 Kd;

					/*"Kd" statement specifies the specular reflectivity of the current material*/
					maths::unaligned_float3 Ks;

					/*"Tf" statement specifies the transmission filter of the current material.
					Any light passing through the object is filtered by the transmission
					filter, which only allows the specifiec colors to pass through*/
					maths::unaligned_float3 Tf;

					string map_Ka;
					string map_Kd;
					string map_Ks;
					string map_d; //alpha channel texture
				};

				struct model_element
				{
					string material;
					collections::vector<vertex, memory::aligned16_allocator> vertices;

					bool operator == (const model_element&)const { return false; }//dummy
					bool operator != (const model_element&)const { return false; }//dummy
				};

			private:
				collections::vector<model_element> _elements;

			public:
				ordered_model();

				ANG_DECLARE_INTERFACE();

				collections::vector<model_element> load(core::files::input_text_file_t);
				collections::vector<model_element> load(core::files::input_binary_file_t);
				core::async::iasync_t<collections::vector<model_element>> load_async(core::files::input_text_file_t);
				core::async::iasync_t<collections::vector<model_element>> load_async(core::files::input_binary_file_t);

				bool save(core::files::output_binary_file_t);

			private:
				virtual~ordered_model();
			};

			class indexed_model
				: public object
			{
			public:

				struct model_element
				{
					string technique_name;
					collections::vector<string> textures;
					ibuffer_t index_data;
					ibuffer_t vertex_data;
					reflect::var_type_t index_type;
					collections::vector<reflect::attribute_desc> vertex_desc;

					bool operator == (const model_element&)const { return false; }//dummy
					bool operator != (const model_element&)const { return false; }//dummy
				};

			private:
				collections::vector<model_element> _elements;

			public:
				indexed_model();

				ANG_DECLARE_INTERFACE();

				collections::vector<model_element> load(xml::ixml_node_t);
				core::async::iasync_t<collections::vector<model_element>> load_async(xml::ixml_node_t);

			private:
				bool load_element(xml::ixml_node_t, indexed_model::model_element& out);
				bool load_material(xml::ixml_node_t, indexed_model::model_element& out);
				bool load_index_buffer(xml::ixml_node_t, indexed_model::model_element& out);
				bool load_vertex_buffer(xml::ixml_node_t, indexed_model::model_element& out);


			private:
				virtual~indexed_model();
			};

		}
	}

	ANG_OBJECT_WRAPPER_DECLARATION(, graphics::scenes::ordered_model);
	ANG_OBJECT_WRAPPER_DECLARATION(, graphics::scenes::indexed_model);
}

ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::scenes::ordered_model::vertex);
ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::scenes::ordered_model::material);
ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::scenes::ordered_model::model_element);
ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::scenes::indexed_model::model_element);