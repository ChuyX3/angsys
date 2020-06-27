#pragma once

#if OPENGL_FAMILY_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace gl
		{
			class gl_index_buffer
				: public graphic<gl_index_buffer
				, iid("ang::graphics::gl::gl_index_buffer")
				, buffers::iindex_buffer
				, resources::iresource>
			{
			private:
				string m_resource_sid;
				wsize m_index_count;
				buffers::buffer_usage_t m_usage;
				reflect::var_type_t m_index_type;
				GLuint m_index_buffer;

			public:
				gl_index_buffer();

			protected: //override
				void dispose()override;

			public: //overrides
				resources::iresource_t resource()const override;
				buffers::buffer_type_t buffer_type()const override;
				buffers::buffer_usage_t buffer_usage()const override;
				buffers::buffer_bind_flag_t buffer_bind_flag()const override;
				ibuffer_t map(idriver_t, bool writeMode) override;
				bool unmap(idriver_t, ibuffer_t) override;
				reflect::var_type_t data_type()const override;
				wsize counter()const override;
				wsize size_in_bytes()const override;

				resources::resource_type_t resource_type()const override;
				string resource_sid()const override;
				void resource_sid(cstr_t) override;
				intfptr fast_cast(resources::resource_type_t) override;

			public: //internal
				bool create(
					gl_driver_t driver,
					buffers::buffer_usage_t usage,
					reflect::var_type_t type,
					wsize count,
					array_view<byte> init_data,
					string sid = null);
				bool close();
				void use_buffer(gl_driver_t);
				inline GLuint GLBuffer()const { return m_index_buffer; }
				inline GLenum GLIndexType()const { 
					switch (m_index_type)
					{
					case reflect::var_type::s16:
						return GL_SHORT;
					case reflect::var_type::u16:
						return GL_UNSIGNED_SHORT;
					case reflect::var_type::s32:
						return GL_INT;
					//case reflect::var_type::u32:
					default:
						return GL_UNSIGNED_INT;
					}
				}
			};

			class gl_vertex_buffer
				: public graphic<gl_vertex_buffer
				, iid("ang::graphics::gl::gl_vertex_buffer")
				, buffers::ivertex_buffer
				, resources::iresource>
			{
			private:
				string m_resource_sid;
				uint m_vertex_count;
				uint m_stride;
				buffers::buffer_usage_t m_usage;
				collections::vector<reflect::attribute_desc> m_vertex_desc;
				GLuint m_vertex_buffer;

			public:
				gl_vertex_buffer();

			protected: //override
				void dispose()override;

			public: //overrides
				resources::iresource_t resource()const override;
				buffers::buffer_type_t buffer_type()const override;
				buffers::buffer_usage_t buffer_usage()const override;
				buffers::buffer_bind_flag_t buffer_bind_flag()const override;
				ibuffer_t map(idriver_t, bool writeMode) override;
				bool unmap(idriver_t, ibuffer_t) override;
				array_view<reflect::attribute_desc>const& descriptor()const override;
				wsize block_counter()const override;
				wsize size_in_bytes()const override;

				resources::resource_type_t resource_type()const override;
				string resource_sid()const override;
				void resource_sid(cstr_t) override;
				intfptr fast_cast(resources::resource_type_t) override;

			public: //internal
				bool create(
					gl_driver_t driver,
					buffers::buffer_usage_t usage,
					array_view<reflect::attribute_desc> vertex_desc,
					wsize vertex_count,
					array_view<byte> init_data,
					string sid = null);
				bool close();
				void use_buffer(gl_driver_t);
				inline GLuint GLBuffer()const { return m_vertex_buffer; }
			};

		}
	}
}

#endif
