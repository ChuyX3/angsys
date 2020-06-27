#pragma once

#if OPENGL_FAMILY_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace gl
		{
			class gl_shaders
				: public graphic<gl_shaders
				, iid("ang::graphics::gl::gl_shader")
				, effects::ishaders
				, resources::iresource>
			{
			private:
				astring m_resource_sid;
				collections::vector<reflect::attribute_desc> m_input_layout;
				collections::vector<reflect::varying_desc> m_vs_uniforms;
				collections::vector<reflect::varying_desc> m_ps_uniforms;
				
				GLuint m_gl_program;
				GLuint m_gl_frag_shader;
				GLuint m_gl_vertex_shader;
				
				collections::hash_map<string, GLint> m_attribute_map;
				collections::hash_map<string, GLint> m_uniform_map;
				collections::hash_map<string, GLint> m_texture_map;

			public:
				gl_shaders();

			protected: //override
				void dispose()override;

			public: //overrides		
				resources::iresource_t resource()const override;
				array_view<reflect::attribute_desc>const& input_layout()const override;
				array_view<reflect::varying_desc>const& vs_uniforms_layouts()const override;
				array_view<reflect::varying_desc>const& ps_uniforms_layouts()const override;
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
				error load(gl_driver_t, string, string, string);
				error load(gl_driver_t, effects::shader_info_t const&, effects::shader_info_t const&, string);
				bool use_shaders(gl_driver_t);
				bool close();

			private:
				error load_vertex_shader(gl_driver_t, effects::shader_info_t const&);
				error load_vertex_shader(gl_driver_t, string);
				error load_pixel_shader(gl_driver_t, effects::shader_info_t const&);
				error load_pixel_shader(gl_driver_t, string);
				bool load_vs_const_buffer(gl_driver_t, reflect::varying_desc&);
				bool load_ps_const_buffer(gl_driver_t, reflect::varying_desc&);
				//bool load_ps_samplers(gl_driver_t, dom::xml::xml_node_t);

			public: //internal
				inline GLuint GLProgram()const { return m_gl_program; }
				inline GLuint GLFragShader()const { return m_gl_frag_shader; }
				inline GLuint GLVertexShader()const { return m_gl_vertex_shader; }
				
			private:
				virtual~gl_shaders();
			};

			//bool d3d_load_shader_input(collections::vector<graphics::reflect::attribute_desc> const& attributes, collections::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout_desc);
			//bool d3d_reflect_shader_input(collections::vector<graphics::reflect::attribute_desc>& attributes, ID3D11ShaderReflection* vertexShaderReflection);
			//bool d3d_reflect_shader_variable(reflect::varying_desc_t& field, ID3D11ShaderReflectionVariable* varInfo);
			//bool d3d_reflect_shader_uniforms(reflect::uniform_fields_t uniforms, ID3D11ShaderReflection* vertexShaderReflection);
		}
	}
}



#endif
