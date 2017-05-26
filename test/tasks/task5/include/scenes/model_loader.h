#pragma once
#include <ang/graphics/angraph.h>

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

					bool operator == (const vertex&) { return false; }//dummy
					bool operator != (const vertex&) { return false; }//dummy
				};

				struct model_element
				{
					string material;
					collections::vector<vertex> vertices;

					bool operator == (const model_element&) { return false; }//dummy
					bool operator != (const model_element&) { return false; }//dummy
				};

			private:
				collections::vector<model_element> _elements;

			public:
				ordered_model();

				ANG_DECLARE_INTERFACE();

				collections::vector<model_element> load(core::files::input_text_file_t);
				core::async::iasync_t<collections::vector<model_element>> load_async(core::files::input_text_file_t);

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

					bool operator == (const model_element&) { return false; }//dummy
					bool operator != (const model_element&) { return false; }//dummy
				};

			private:
				collections::vector<model_element> _elements;

			public:
				indexed_model();

				ANG_DECLARE_INTERFACE();

				collections::vector<model_element> load(xml::xml_node_t);
				core::async::iasync_t<collections::vector<model_element>> load_async(xml::xml_node_t);

			private:
				bool load_element(xml::xml_node_t, indexed_model::model_element& out);
				bool load_material(xml::xml_node_t, indexed_model::model_element& out);
				bool load_index_buffer(xml::xml_node_t, indexed_model::model_element& out);
				bool load_vertex_buffer(xml::xml_node_t, indexed_model::model_element& out);


			private:
				virtual~indexed_model();
			};

		}
	}
}

ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::scenes::ordered_model::vertex);
ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::scenes::ordered_model::model_element);
ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::scenes::indexed_model::model_element);