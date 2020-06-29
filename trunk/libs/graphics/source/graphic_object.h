#pragma once
#include <ang/graphics/graphics.h>

namespace ang
{
	namespace graphics
	{
		declare_enum(, shader_type, uint)
		{
			vertex,
			pixel,
			geometry,
		};

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

	namespace algorithms
	{
		template<>
		struct hash<graphics::vertex_buffer_id>
		{
			using type = graphics::vertex_buffer_id;
			static ulong64 make(graphics::vertex_buffer_id const& value) {
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
		template<typename T, typename IID, typename...Ts>
		using graphic = implement<T, IID, aligned<16>, Ts...>;
	}

	//COFFE_DECLARE_CLASS_INFO_OVERRIDE(, graphics::graphic_object);
}
