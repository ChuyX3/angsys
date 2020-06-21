#pragma once

#if DIRECTX11_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace d3d11
		{
			struct vertex_buffer_id
			{
				reflect::var_semantic_t semantic;
				windex semantic_idx;
				vertex_buffer_id()
					: semantic(reflect::var_semantic::none)
					, semantic_idx(0)
				{
				}
				vertex_buffer_id(vertex_buffer_id&& id)
					: semantic(id.semantic)
					, semantic_idx(id.semantic_idx)
				{
				}
				vertex_buffer_id(vertex_buffer_id const& id)
					: semantic(id.semantic)
					, semantic_idx(id.semantic_idx)
				{
				}
				vertex_buffer_id(reflect::var_semantic_t semantic, windex semantic_idx)
					: semantic(semantic)
					, semantic_idx(semantic_idx)
				{
				}
				~vertex_buffer_id() 
				{
				}

				bool operator == (vertex_buffer_id const& id)const
				{
					return semantic_idx == id.semantic_idx && semantic == id.semantic;
				}
				bool operator != (vertex_buffer_id const& id)const
				{
					return semantic_idx != id.semantic_idx && semantic != id.semantic;
				}
			};
		}
	}

	namespace algorithms
	{
		template<>
		struct hash<graphics::d3d11::vertex_buffer_id>
		{
			using type = graphics::d3d11::vertex_buffer_id;
			static ulong64 make(graphics::d3d11::vertex_buffer_id const& value) {
				union {
					struct {
						graphics::reflect::var_semantic semantic;
						uint semantic_idx;
					};
					ulong64 val;
				} key;
				key.semantic = value.semantic;
				key.semantic_idx = value.semantic_idx;

				return (ulong64)((2475825 + key.val + 1));
			}
			ulong64 operator()(type const& value)const {
				return make(value);
			}
		};
	}

	namespace graphics
	{
		namespace d3d11
		{
			class d3d11_index_buffer;
			class d3d11_vertex_buffer;

			typedef object_wrapper<d3d11_index_buffer> d3d11_index_buffer_t;
			typedef object_wrapper<d3d11_vertex_buffer> d3d11_vertex_buffer_t;

			class d3d11_index_buffer
				: public graphic<d3d11_index_buffer
				, iid("ang::graphics::d3d11::d3d11_index_buffer")
				, buffers::iindex_buffer
				, buffers::igpu_buffer
				, resources::iresource>
			{
			private:
				string m_resource_sid;
				wsize m_index_count;
				buffers::buffer_usage_t m_usage;
				reflect::var_type_t m_index_type;
				com_wrapper<ID3D11Buffer> m_index_buffer;

			public:
				d3d11_index_buffer();

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
					d3d11_driver_t driver, 
					buffers::buffer_usage_t usage, 
					reflect::var_type_t type, 
					wsize count, 
					array_view<byte> init_data,
					string sid = null);
				bool close();
				void use_buffer(d3d11_driver_t);
				inline ID3D11Buffer* D3D11Buffer()const { return m_index_buffer.get(); }
			};

			class d3d11_vertex_buffer
				: public graphic<d3d11_vertex_buffer
				, iid("ang::graphics::d3d11::d3d11_vertex_buffer")
				, buffers::ivertex_buffer
				, buffers::igpu_buffer
				, resources::iresource>
			{
			private:
				string m_resource_sid;
				uint m_vertex_count;
				uint m_stride;
				buffers::buffer_usage_t m_usage;
				collections::vector<reflect::attribute_desc> m_vertex_desc;
				com_wrapper<ID3D11Buffer> m_vertex_buffer;
				collections::hash_map<vertex_buffer_id, windex> m_attribute_by_id;

			public:
				d3d11_vertex_buffer();

			protected: //override
				void dispose()override;

			public: //overrides
				resources::iresource_t resource()const override;
				buffers::buffer_type_t buffer_type()const override;
				buffers::buffer_usage_t buffer_usage()const override;
				buffers::buffer_bind_flag_t buffer_bind_flag()const override;
				ibuffer_t map(idriver_t, bool writeMode) override;
				bool unmap(idriver_t, ibuffer_t) override;
				array<reflect::attribute_desc> descriptor()const override;
				wsize block_counter()const override;
				wsize size_in_bytes()const override;

				resources::resource_type_t resource_type()const override;
				string resource_sid()const override;
				void resource_sid(cstr_t) override;
				intfptr fast_cast(resources::resource_type_t) override;

			public: //internal
				bool create(
					d3d11_driver_t driver,
					buffers::buffer_usage_t usage,
					array_view<reflect::attribute_desc_t> vertex_desc,
					wsize vertex_count,
					ibuffer_t vertex_data,
					string sid = null);
				bool close();
				void use_buffer(d3d11_driver_t);
				void use_buffer(d3d11_driver_t, array_view<reflect::attribute_desc_t> const&);
				inline ID3D11Buffer* D3D11Buffer()const { return m_vertex_buffer.get(); }
			};

		}
	}
}

#endif
