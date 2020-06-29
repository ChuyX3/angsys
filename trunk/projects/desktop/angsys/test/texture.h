#pragma once

namespace ang
{
	namespace graphics
	{
		class gl_texture
			: public implement<gl_texture
			, iid("ang::graphics::gl_texture")>
		{
		private:
			uint m_tex_buffer_id;
			weak_ptr<gl_context> m_context;

		public:
			gl_texture();

			error init(gl_context_t, string file);
			core::async::iasync_op<gl_texture_t> init_async(gl_context_t, string file);
			virtual void dispose()override;

			uint tex_buffer()const;
		};
	}
}