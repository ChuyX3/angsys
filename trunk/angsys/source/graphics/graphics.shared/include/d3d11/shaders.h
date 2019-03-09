#pragma once

#if DIRECTX11_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace d3d11
		{
			class d3d11_effect_library;
			class d3d11_effect;
			class d3d11_shaders;

			typedef object_wrapper<d3d11_shaders> d3d11_shaders_t;
			typedef object_wrapper<d3d11_effect> d3d11_effect_t;
			typedef object_wrapper<d3d11_effect_library> d3d11_effect_library_t;

			class d3d11_shaders
				: public smart<d3d11_shaders, effects::ishaders>
			{
			private:
				string m_technique_name;
				collections::vector<reflect::attribute_desc> m_input_layout;
				collections::vector<reflect::varying_desc> m_vs_uniforms;
				collections::vector<reflect::varying_desc> m_ps_uniforms;

				com_wrapper<ID3D11PixelShader> m_d3d_pixel_shader;
				com_wrapper<ID3D11VertexShader> m_d3d_vertex_shader;
				com_wrapper<ID3D11InputLayout> m_d3d_input_layout;
				collections::vector<com_wrapper<ID3D11Buffer>> m_d3d_vs_const_buffers;
				collections::vector<com_wrapper<ID3D11Buffer>> m_d3d_ps_const_buffers;
				collections::vector<com_wrapper<ID3D11SamplerState>> m_d3d_ps_samplers;

			public:
				d3d11_shaders();

				ANG_DECLARE_INTERFACE();

			public:
				bool load(d3d11_effect_library_t, dom::xml::xml_node_t);
				bool use_shaders(d3d11_driver_t);

			public: //overrides
				text::istring_view_t resource_name()const override;
				array_view<reflect::attribute_desc> input_layout()const override;
				array_view<reflect::varying_desc> vs_uniforms_layouts()const override;
				array_view<reflect::varying_desc> ps_uniforms_layouts()const override;
				reflect::varying map_vs_uniform(idriver_t, index) override; //only one per time
				reflect::varying map_vs_uniform(idriver_t, text::raw_cstr_t) override; //only one per time
				reflect::varying map_ps_uniform(idriver_t, index) override; //only one per time
				reflect::varying map_ps_uniform(idriver_t, text::raw_cstr_t) override; //only one per time
				void unmap_vs_uniform(idriver_t driver, reflect::varying&) override;
				void unmap_ps_uniform(idriver_t driver, reflect::varying&) override;

			private:
				bool load_vertex_shader(d3d11_effect_library_t, dom::xml::xml_node_t);
				bool load_pixel_shader(d3d11_effect_library_t, dom::xml::xml_node_t);
				bool load_vs_const_buffer(d3d11_effect_library_t, dom::xml::xml_node_t);
				bool load_ps_const_buffer(d3d11_effect_library_t, dom::xml::xml_node_t);
				bool load_ps_samplers(d3d11_effect_library_t, dom::xml::xml_node_t);

			public: //internal
				inline ID3D11PixelShader* D3D11PixelShader()const { return m_d3d_pixel_shader.get(); }
				inline ID3D11VertexShader* D3D11VertexShader()const { return m_d3d_vertex_shader.get(); }
				inline ID3D11InputLayout* D3D11VSInputLayout()const { return m_d3d_input_layout.get(); }
				inline collections::vector<com_wrapper<ID3D11Buffer>> const& D3D11VSConstBuffer()const { return m_d3d_vs_const_buffers; }
				inline collections::vector<com_wrapper<ID3D11Buffer>> const& D3D11PSConstBuffer()const { return m_d3d_ps_const_buffers; }
				inline collections::vector<com_wrapper<ID3D11SamplerState>> const& D3D11PSSamplers()const { return m_d3d_ps_samplers; }

			private:
				virtual~d3d11_shaders();
			};

			class d3d11_effect
				: public smart<d3d11_effect, effects::ieffect>
			{
			public:
				//ANG_DECLARE_INTERFACE();
			};

	
			class d3d11_effect_library
				: public smart<d3d11_effect_library, effects::ieffect_library>
			{
			private:
				d3d11_driver_t m_driver;
				core::files::ifile_system_t m_fs;
				core::async::mutex_ptr_t m_mutex;
				collections::hash_map<string, wstring> m_source_map;
				collections::hash_map<string, d3d11_shaders_t> m_shaders;
				collections::hash_map<string, d3d11_effect_t> m_effects;

			public:
				d3d11_effect_library(d3d11_driver_t parent);

				ANG_DECLARE_INTERFACE();

			public: //overrides
				bool load_sources(dom::xml::xml_node_t) override;
				void set_file_system(core::files::ifile_system_t) override;
				bool load_library(dom::xml::xml_node_t) override;
				core::async::iasync<effects::ieffect_library_t> load_library_async(dom::xml::xml_node_t) override;
				effects::ieffect_t load_effect(dom::xml::xml_node_t) override;
				core::async::iasync<effects::ieffect_t> load_effect_async(dom::xml::xml_node_t) override;
				effects::ishaders_t load_shaders(dom::xml::xml_node_t) override;
				core::async::iasync<effects::ishaders_t> load_shaders_async(dom::xml::xml_node_t) override;
				effects::ieffect_t find_effect(text::raw_cstr_t)const override;
				effects::ishaders_t find_shaders(text::raw_cstr_t)const override;

			public:
				inline core::files::ifile_system* get_file_system()const {
					return m_fs.is_empty() ? core::files::ifile_system::fs_instance() : m_fs.get();
				}
				inline d3d11_driver* driver()const { return m_driver.get(); }
				inline wstring find_file(text::raw_cstr_t sid)const {
					if (m_source_map.is_empty())
						return "";
					auto it = m_source_map->find(sid);
					
					return it.is_valid() ? it->value : wstring();
				}

			private:
				virtual~d3d11_effect_library();

			};


			bool d3d_load_shader_input(collections::vector<graphics::reflect::attribute_desc> const& attributes, collections::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout_desc);
		}
	}
}


inline bool operator == (D3D_SHADER_MACRO const&, D3D_SHADER_MACRO const&) { return false; } //dummy
inline bool operator == (D3D11_INPUT_ELEMENT_DESC const&, D3D11_INPUT_ELEMENT_DESC const&) { return false; } //dummy

//ANG_REGISTER_RUNTIME_TYPENAME(D3D_SHADER_MACRO);
//ANG_REGISTER_RUNTIME_TYPENAME(D3D11_INPUT_ELEMENT_DESC);

#endif
