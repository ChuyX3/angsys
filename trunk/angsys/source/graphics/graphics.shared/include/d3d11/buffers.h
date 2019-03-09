#pragma once

#if DIRECTX11_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace d3d11
		{
			class d3d11_index_buffer;
			class d3d11_vertex_buffer;

			typedef object_wrapper<d3d11_index_buffer> d3d11_index_buffer_t;
			typedef object_wrapper<d3d11_vertex_buffer> d3d11_vertex_buffer_t;

			class d3d11_index_buffer
				: public smart<d3d11_index_buffer, buffers::iindex_buffer>
			{
			private:
				wsize m_index_count;
				buffers::buffer_usage_t m_usage;
				reflect::var_type_t m_index_type;
				com_wrapper<ID3D11Buffer> m_index_buffer;

			public:
				d3d11_index_buffer();

			public: //overrides
				ANG_DECLARE_INTERFACE();

				buffers::buffer_type_t buffer_type()const override;
				buffers::buffer_usage_t buffer_usage()const override;
				buffers::buffer_bind_flag_t buffer_bind_flag()const override;
				ibuffer_t map(idriver_t, bool writeMode) override;
				bool unmap(idriver_t, ibuffer_t) override;
				reflect::var_type_t data_type()const override;
				wsize counter()const override;
				wsize size_in_bytes()const override;

			public: //internal
				bool create(
					d3d11_driver_t driver, 
					buffers::buffer_usage_t usage, 
					reflect::var_type_t type, 
					wsize count, 
					array_view<byte> init_data);
				bool close();
				void use_buffer(d3d11_driver_t);
				inline ID3D11Buffer* D3D11Buffer()const { return m_index_buffer.get(); }

			private:
				virtual~d3d11_index_buffer();
			};

			class d3d11_vertex_buffer
				: public smart<d3d11_vertex_buffer, buffers::ivertex_buffer>
			{
			private:
				uint m_vertex_count;
				uint m_stride;
				buffers::buffer_usage_t m_usage;
				collections::vector<reflect::attribute_desc> m_vertex_desc;
				com_wrapper<ID3D11Buffer> m_vertex_buffer;

			public:
				d3d11_vertex_buffer();

			public: //overrides
				ANG_DECLARE_INTERFACE();

				buffers::buffer_type_t buffer_type()const override;
				buffers::buffer_usage_t buffer_usage()const override;
				buffers::buffer_bind_flag_t buffer_bind_flag()const override;
				ibuffer_t map(idriver_t, bool writeMode) override;
				bool unmap(idriver_t, ibuffer_t) override;
				array_view<reflect::attribute_desc> descriptor()const override;
				wsize block_counter()const override;
				wsize size_in_bytes()const override;

			public: //internal
				bool create(
					d3d11_driver_t driver,
					buffers::buffer_usage_t usage,
					array_view<reflect::attribute_desc> vertex_desc,
					wsize vertex_count,
					array_view<byte> init_data);
				bool close();
				void use_buffer(d3d11_driver_t);
				inline ID3D11Buffer* D3D11Buffer()const { return m_vertex_buffer.get(); }

			private:
				virtual~d3d11_vertex_buffer();
			};

		}
	}
}

#endif
