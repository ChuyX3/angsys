#pragma once

namespace ang
{
	namespace graphics
	{
		class gl_shaders final
			: public implement<gl_shaders
			, iid("ang::graphics::gl_shaders")>
		{
		private:
			uint m_program;
			uint m_vertex_shader;
			uint m_fragment_shader;
			weak_ptr<gl_context> m_context;
			collections::hash_map<string, int> m_uniforms;
			collections::hash_map<string, int> m_attributes;

		public:
			gl_shaders();

		public:
			error init(gl_context_t, string vsfile, string psfile);
			core::async::iasync_op<gl_shaders_t> init_async(gl_context_t, string vsfile, string psfile);



			int uniform(cstr_t)const;
			int attribute(cstr_t)const;

			void use_program()const;

		private:
			void dispose()override;
		};
	}
}