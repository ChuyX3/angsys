#pragma once

namespace ang
{
	namespace graphics
	{
		class gl_mesh final
			: public implement<gl_mesh
			, iid("ang::graphics::gl_mesh")>
		{
		private:
			uint m_index_count;
			uint m_vertex_count;
			uint m_index_buffer_id;
			uint m_vertex_buffer_id;
			weak_ptr<gl_context> m_context;

		public:
			gl_mesh();

			error init(gl_context_t, string file);
			core::async::iasync_op<gl_mesh_t> init_async(gl_context_t, string file);

			uint index_count()const;
			uint vertex_count()const;
			uint index_buffer()const;
			uint vertex_buffer()const;

		private:
			void dispose()override;
		};

	}
}