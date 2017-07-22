#pragma once

#include <ang/graphics/angraph.h>

#if DIRECTX_SUPPORT

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
				: public object
				, public effects::ishaders
			{
			private:
				string _technique_name;

				collections::vector<reflect::attribute_desc> _input_layout;
				collections::vector<reflect::variable_desc> _vs_uniforms;
				collections::vector<reflect::variable_desc> _ps_uniforms;

				com_wrapper<ID3D11PixelShader> d3d_pixel_shader;
				com_wrapper<ID3D11VertexShader> d3d_vertex_shader;
				com_wrapper<ID3D11InputLayout> d3d_input_layout;
				collections::vector<com_wrapper<ID3D11Buffer>> d3d_vs_const_buffers;
				collections::vector<com_wrapper<ID3D11Buffer>> d3d_ps_const_buffers;
				collections::vector<com_wrapper<ID3D11SamplerState>> d3d_ps_samplers;

			public:
				d3d11_shaders();

				ANG_DECLARE_INTERFACE();

			public:
				bool load(d3d11_effect_library_t, xml::xml_node_t);
				bool use_shaders(d3d11_driver_t);

			public: //overrides
				string resource_name()const override;
				static_array<reflect::attribute_desc> input_layout()const override;
				static_array<reflect::variable_desc> vs_uniforms_layouts()const override;
				static_array<reflect::variable_desc> ps_uniforms_layouts()const override;
				reflect::variable map_vs_uniform(idriver_t, index) override; //only one per time
				reflect::variable map_vs_uniform(idriver_t, cstr_t) override; //only one per time
				reflect::variable map_vs_uniform(idriver_t, cwstr_t) override; //only one per time
				reflect::variable map_ps_uniform(idriver_t, index) override; //only one per time
				reflect::variable map_ps_uniform(idriver_t, cstr_t) override; //only one per time
				reflect::variable map_ps_uniform(idriver_t, cwstr_t) override; //only one per time
				void unmap_vs_uniform(idriver_t driver, reflect::variable&) override;
				void unmap_ps_uniform(idriver_t driver, reflect::variable&) override;

			private:
				bool load_vertex_shader(d3d11_effect_library_t, xml::xml_node_t);
				bool load_pixel_shader(d3d11_effect_library_t, xml::xml_node_t);
				bool load_vs_const_buffer(d3d11_effect_library_t, xml::xml_node_t);
				bool load_ps_const_buffer(d3d11_effect_library_t, xml::xml_node_t);
				bool load_ps_samplers(d3d11_effect_library_t, xml::xml_node_t);

			public: //internal
				inline ID3D11PixelShader* D3D11PixelShader()const { return d3d_pixel_shader.get(); }
				inline ID3D11VertexShader* D3D11VertexShader()const { return d3d_vertex_shader.get(); }
				inline ID3D11InputLayout* D3D11VSInputLayout()const { return d3d_input_layout.get(); }
				inline collections::vector<com_wrapper<ID3D11Buffer>> const& D3D11VSConstBuffer()const { return d3d_vs_const_buffers; }
				inline collections::vector<com_wrapper<ID3D11Buffer>> const& D3D11PSConstBuffer()const { return d3d_ps_const_buffers; }
				inline collections::vector<com_wrapper<ID3D11SamplerState>> const& D3D11PSSamplers()const { return d3d_ps_samplers; }

			private:
				virtual~d3d11_shaders();
			};

			class d3d11_effect
				: public object
				, public effects::ieffect
			{
			public:
				ANG_DECLARE_INTERFACE();
			};

	
			class d3d11_effect_library
				: public object
				, public effects::ieffect_library
			{
			private:
				core::async::mutex_t main_mutex;
				d3d11_driver_t _driver;
				core::files::ifile_system_t _fs;
				collections::unordered_map<string, wstring> _source_map;
				collections::unordered_map<string, d3d11_shaders_t> _shaders;
				collections::unordered_map<string, d3d11_effect_t> _effects;

			public:
				d3d11_effect_library(d3d11_driver_t parent);

				ANG_DECLARE_INTERFACE();

			public: //overrides
				bool load_sources(xml::xml_node_t) override;
				void set_file_system(core::files::ifile_system_t) override;
				bool load_library(xml::xml_node_t) override;
				core::async::iasync_t<effects::ieffect_library_t> load_library_async(xml::xml_node_t) override;
				effects::ieffect_t load_effect(xml::xml_node_t) override;
				core::async::iasync_t<effects::ieffect_t> load_effect_async(xml::xml_node_t) override;
				effects::ishaders_t load_technique(xml::xml_node_t) override;
				core::async::iasync_t<effects::ishaders_t> load_technique_async(xml::xml_node_t) override;
				effects::ieffect_t find_effect(cstr_t)const override;
				effects::ieffect_t find_effect(cwstr_t)const override;
				effects::ishaders_t find_technique(cstr_t)const override;
				effects::ishaders_t find_technique(cwstr_t)const override;

			public:
				inline core::files::ifile_system* get_file_system()const {
					return _fs.is_empty() ? core::files::ifile_system::get_file_system() : _fs.get();
				}
				inline d3d11_driver* driver()const { return _driver.get(); }
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

			private:
				virtual~d3d11_effect_library();

			};


			bool d3d_load_shader_input(collections::vector<graphics::reflect::attribute_desc> const& attributes, collections::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout_desc);
		}
	}
}


inline bool operator == (D3D_SHADER_MACRO const&, D3D_SHADER_MACRO const&) { return false; } //dummy
inline bool operator == (D3D11_INPUT_ELEMENT_DESC const&, D3D11_INPUT_ELEMENT_DESC const&) { return false; } //dummy

ANG_REGISTER_RUNTIME_TYPENAME(D3D_SHADER_MACRO);
ANG_REGISTER_RUNTIME_TYPENAME(D3D11_INPUT_ELEMENT_DESC);

#endif
