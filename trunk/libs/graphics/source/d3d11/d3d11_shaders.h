#pragma once

#if DIRECTX11_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace d3d11
		{
			class d3d11_shaders
				: public graphic<d3d11_shaders
				, iid("ang::graphics::d3d11::d3d11_shaders")
				, effects::ishaders
				, resources::iresource>
			{
			private:
				string m_resource_sid;
				vector<reflect::attribute_desc> m_input_layout;
				vector<reflect::varying_desc> m_vs_uniforms;
				vector<reflect::varying_desc> m_ps_uniforms;
				vector<string> m_d3d_textures;

				com_wrapper<ID3D11PixelShader> m_d3d_pixel_shader;
				com_wrapper<ID3D11VertexShader> m_d3d_vertex_shader;
				com_wrapper<ID3D11InputLayout> m_d3d_input_layout;

				vector<com_wrapper<ID3D11Buffer>> m_d3d_vs_const_buffers;
				vector<com_wrapper<ID3D11Buffer>> m_d3d_ps_const_buffers;
				vector<com_wrapper<ID3D11SamplerState>> m_d3d_ps_samplers;

			public:
				d3d11_shaders();

			protected: //override
				void dispose()override;

			public: //overrides		
				resources::iresource_t resource()const override;
				array_view<reflect::attribute_desc> input_layout()const override;
				array_view<reflect::varying_desc> vs_uniforms_layouts()const override;
				array_view<reflect::varying_desc> ps_uniforms_layouts()const override;
				bool bind_vertex_buffer(idriver_t, buffers::ivertex_buffer_t) override;
				bool bind_texture(idriver_t, windex, windex) override;
				bool bind_texture(idriver_t, cstr_t, windex) override;
				reflect::varying map_vs_uniform(idriver_t, windex) override; //only one per time
				reflect::varying map_vs_uniform(idriver_t, cstr_t) override; //only one per time
				reflect::varying map_ps_uniform(idriver_t, windex) override; //only one per time
				reflect::varying map_ps_uniform(idriver_t, cstr_t) override; //only one per time
				void unmap_vs_uniform(idriver_t driver, reflect::varying&) override;
				void unmap_ps_uniform(idriver_t driver, reflect::varying&) override;

				resources::resource_type_t resource_type()const override;
				string resource_sid()const override;
				void resource_sid(cstr_t) override;
				intfptr fast_cast(resources::resource_type_t) override;

			public:
				bool load(d3d11_driver_t, string, string, string, astring* log = null);
				bool load(d3d11_driver_t, effects::shader_info_t const&, effects::shader_info_t const&, string, astring* log = null);
				bool use_shaders(d3d11_driver_t);
				bool close();

			private:
				astring load_vertex_shader(d3d11_driver_t, effects::shader_info_t const&);
				astring load_vertex_shader(d3d11_driver_t, string);
				astring load_pixel_shader(d3d11_driver_t, effects::shader_info_t const&);
				astring load_pixel_shader(d3d11_driver_t, string);
				bool load_vs_const_buffer(d3d11_driver_t, reflect::varying_desc&);
				bool load_ps_const_buffer(d3d11_driver_t, reflect::varying_desc&);
				//bool load_ps_samplers(d3d11_driver_t, dom::xml::xml_node_t);

			public: //internal
				inline ID3D11PixelShader* D3D11PixelShader()const { return m_d3d_pixel_shader.get(); }
				inline ID3D11VertexShader* D3D11VertexShader()const { return m_d3d_vertex_shader.get(); }
				inline ID3D11InputLayout* D3D11VSInputLayout()const { return m_d3d_input_layout.get(); }
				inline collections::vector<com_wrapper<ID3D11Buffer>> const& D3D11VSConstBuffer()const { return m_d3d_vs_const_buffers; }
				inline collections::vector<com_wrapper<ID3D11Buffer>> const& D3D11PSConstBuffer()const { return m_d3d_ps_const_buffers; }
				inline collections::vector<com_wrapper<ID3D11SamplerState>> const& D3D11PSSamplers()const { return m_d3d_ps_samplers; }
			};


			bool d3d_load_shader_input(collections::vector<graphics::reflect::attribute_desc> const& attributes, collections::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout_desc);
			bool d3d_reflect_shader_input(collections::vector<graphics::reflect::attribute_desc>& attributes, ID3D11ShaderReflection* vertexShaderReflection);
			bool d3d_reflect_shader_variable(reflect::varying_desc_t& field, ID3D11ShaderReflectionVariable* varInfo);
			bool d3d_reflect_shader_uniforms(reflect::uniform_fields_t& uniforms, ID3D11ShaderReflection* vertexShaderReflection);
		}
	}
}


//inline bool operator == (D3D_SHADER_MACRO const&, D3D_SHADER_MACRO const&) { return false; } //dummy
//inline bool operator == (D3D11_INPUT_ELEMENT_DESC const&, D3D11_INPUT_ELEMENT_DESC const&) { return false; } //dummy

//COFFE_REGISTER_RUNTIME_TYPENAME(D3D_SHADER_MACRO);
//COFFE_REGISTER_RUNTIME_TYPENAME(D3D11_INPUT_ELEMENT_DESC);

#endif
