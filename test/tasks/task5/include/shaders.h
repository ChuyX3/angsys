#pragma once
namespace d3d11
{
	namespace resources
	{
		
		class shaders
			: public resource
		{
		private:
			collections::vector<graphics::reflect::attribute_desc> input_layout;
			collections::vector<graphics::reflect::variable_desc> vs_uniforms;
			collections::vector<graphics::reflect::variable_desc> ps_uniforms;

			com_wrapper<ID3D11PixelShader> d3d_pixel_shader;
			com_wrapper<ID3D11VertexShader> d3d_vertex_shader;
			com_wrapper<ID3D11InputLayout> d3d_input_layout;
			collections::vector<com_wrapper<ID3D11Buffer>> d3d_vs_const_buffers;
			collections::vector<com_wrapper<ID3D11Buffer>> d3d_ps_const_buffers;

		public:
			shaders();

			ANG_DECLARE_INTERFACE();

			bool load(ID3D11Device2*, xml::xml_node_t);

			bool use_shaders(ID3D11DeviceContext2*);
			bool update_vs_uniforms(ID3D11DeviceContext2*, index, void*);
			bool update_ps_uniforms(ID3D11DeviceContext2*, index, void*);

		private:
			bool load_vertex_shader(ID3D11Device2*, xml::xml_node_t);
			bool load_pixel_shader(ID3D11Device2*, xml::xml_node_t);
			bool load_vs_const_buffer(ID3D11Device2*, xml::xml_node_t);
			bool load_ps_const_buffer(ID3D11Device2*, xml::xml_node_t);
			bool load_input_layout(xml::xml_node_t);

		public:
			inline ID3D11PixelShader* D3D11PixelShader()const { return d3d_pixel_shader.get(); }
			inline ID3D11VertexShader* D3D11VertexShader()const { return d3d_vertex_shader.get(); }
			inline ID3D11InputLayout* D3D11InputLayout()const { return d3d_input_layout.get(); }
			inline collections::vector<com_wrapper<ID3D11Buffer>> const& D3D11VSConstBuffer()const { return d3d_vs_const_buffers; }
			inline collections::vector<com_wrapper<ID3D11Buffer>> const& D3D11PSConstBuffer()const { return d3d_ps_const_buffers; }

		private:
			virtual~shaders();
		};


		class fx_library 
			: public object
			, public singleton<fx_library_t>
		{
		private:
			friend singleton<fx_library_t>;

			collections::map<string, wstring> _source_map;
			collections::map<string, shaders_t> _shaders;

			fx_library();
			virtual~fx_library();

		public:
			ANG_DECLARE_INTERFACE();

			bool load(ID3D11Device2*, xml::xml_node_t);

		private:
			bool load_sources(xml::xml_node_t);
			bool load_fx(ID3D11Device2*, xml::xml_node_t);

		public:
			inline wstring find_file(cstr_t sid)const {
				if (_source_map.is_empty())
					return "";
				auto it = _source_map->find(sid);
				return it.is_valid() ? it->value() : L"";
			}
			inline wstring find_file(cwstr_t sid)const {
				if (_source_map.is_empty())
					return "";
				auto it = _source_map->find(sid);
				return it.is_valid() ? it->value() : L"";
			}
			inline shaders_t find_shaders(cstr_t sid)const {
				if (_shaders.is_empty())
					return null;
				auto it = _shaders->find(sid);
				return it.is_valid() ? it->value() : null;
			}
			inline shaders_t find_shaders(cwstr_t sid)const {
				if (_shaders.is_empty())
					return null;
				auto it = _shaders->find(sid);
				return it.is_valid() ? it->value() : null;
			}
		};
	}

}

