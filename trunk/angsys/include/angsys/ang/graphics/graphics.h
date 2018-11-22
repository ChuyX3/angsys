#ifndef __ANG_GRAPHICS_H__
#define __ANG_GRAPHICS_H__

#include <angsys.h>
#include <ang/maths/maths.h>
#include <ang/maths/vectors.h>
#include <ang/platform/platform.h>
//#include <ang/collections/map.h>
//#include <ang/collections/vector.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef GRAPHICS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif

namespace ang
{
	namespace graphics
	{
		ang_interface(idriver);
		ang_interface(isurface);
		ang_interface(iframe_buffer);

		namespace reflect
		{
			typedef struct variable_desc variable_desc_t;
			typedef struct attribute_desc attribute_desc_t;

			safe_enum(LINK, var_type, uint)
			{
				none = 0,
				s8,
				u8,
				s16,
				u16,
				s32,
				u32,
				f32,

				tex1D,
				tex2D,
				tex3D,
				texCube,

				buffer,
				block,
			};

			safe_enum(LINK, var_class, uint)
			{
				none = 0,
				scalar = 1,
				vec2 = 2,
				vec3 = 3,
				vec4 = 4,
				mat2 = 4,
				mat3 = 9,
				mat4 = 16,
			};

			safe_enum(LINK, var_semantic, uint)
			{
				none = 0,
				position,
				color,
				tex_coord,
				normal,
				binormal,
				tangent,
				fog,
			};
		}

		namespace buffers
		{
			ang_interface(igpu_buffer);
			ang_interface(iindex_buffer);
			ang_interface(ivertex_buffer);
		}

		namespace textures
		{
			ang_interface(itexture);
			ang_interface(itexture_loader);
		}

		namespace effects
		{
			ang_interface(ishaders);
			ang_interface(ieffect);
			ang_interface(ieffect_library);
		}
	}

	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::idriver);
	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::isurface);
	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::iframe_buffer);

	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::buffers::igpu_buffer);
	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::buffers::iindex_buffer);
	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::buffers::ivertex_buffer);

	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::textures::itexture);
	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::textures::itexture_loader);

	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::effects::ishaders);
	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::effects::ieffect);
	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::effects::ieffect_library);

}

#include <ang/graphics/reflect.h>
#include <ang/graphics/buffers.h>
#include <ang/graphics/textures.h>
#include <ang/graphics/effects.h>
//#include <ang/graphics/geometry.hpp>

namespace ang
{
	namespace graphics
	{

		safe_enum(LINK, cull_mode, uint)
		{
			none = 0,
			front,
			back,
		};

		safe_enum(LINK, front_face, uint)
		{
			def,
			clockwise,
			counter_clockwise = 0,
		};

		safe_enum(LINK, blend_mode, uint)
		{
			disable,
			enable,
		};

		typedef struct frame_buffer_desc
		{
			array_view<textures::tex_format_t> color_format;
			textures::tex_format_t depth_stencil_format;
			graphics::size<float> dimentions;
		}frame_buffer_desc_t, *frame_buffer_desc_ptr_t;

		ang_begin_interface(LINK iframe_buffer)
			visible vcall graphics::size<float> dimentions()const pure;
			visible vcall uint color_buffer_count()const pure;
			visible vcall textures::itexture_t color_buffer(index)const pure;
			visible vcall bool has_depth_stencil_buffer()const pure;
		ang_end_interface();

		ang_begin_interface(LINK isurface)
			visible vcall void swap_buffers() pure;
			visible vcall iframe_buffer_t frame_buffer()const pure;
		ang_end_interface();


		ang_begin_interface(LINK idriver)
			visible vcall graph_driver_type_t graph_driver_type()const pure;
			visible vcall isurface_t create_surface(platform::icore_view_t)const pure;
			visible vcall effects::ieffect_library_t create_effect_library()const pure;
			visible vcall textures::itexture_loader_t create_texture_loader()const pure;
			visible vcall buffers::ivertex_buffer_t create_vertex_buffer(
				buffers::buffer_usage_t usage,
				array_view<reflect::attribute_desc> vertex_desc,
				wsize vertex_count,
				array_view<byte> init_data
			)const pure;
			visible vcall buffers::iindex_buffer_t create_index_buffer(
				buffers::buffer_usage_t usage,
				reflect::var_type_t index_type,
				wsize index_count,
				array_view<byte> init_data
			)const pure;
			visible vcall iframe_buffer_t create_frame_buffer(
				array_view<textures::tex_format_t> color_format,
				textures::tex_format_t depth_stencil_format,
				graphics::size<float> dimentions
			)const pure;

			visible vcall void cull_mode(cull_mode_t) pure;
			visible vcall cull_mode_t cull_mode()const pure;

			visible vcall void front_face(front_face_t) pure;
			visible vcall front_face_t front_face()const pure;

			visible vcall void blend_mode(blend_mode_t) pure;
			visible vcall blend_mode_t blend_mode()const pure;

			visible vcall void bind_frame_buffer(iframe_buffer_t) pure;
			visible vcall void bind_shaders(effects::ishaders_t) pure;
			visible vcall void bind_texture(textures::itexture_t, index) pure;
			visible vcall void bind_index_buffer(buffers::iindex_buffer_t) pure;
			visible vcall void bind_vertex_buffer(buffers::ivertex_buffer_t) pure;

			visible vcall void clear(color_t) pure;
			visible vcall void clear(maths::float4 const&) pure;
			visible vcall void draw(uint count, primitive_t) pure;
			visible vcall void draw_indexed(uint count, primitive_t) pure;

			visible vcall core::async::mutex_ptr_t driver_guard()const pure;
			template<typename f_t> inline auto execute_on_thread_safe(f_t func)const -> decltype(func()) {
				core::async::scope_locker<core::async::mutex_ptr_t> _lock = driver_guard();
				return func();
			}

			template<typename T> inline buffers::iindex_buffer_t create_index_buffer(buffers::buffer_usage_t usage, array_view<T> init_data)const { return null; }
			template<typename T, wsize N> inline buffers::iindex_buffer_t create_index_buffer(buffers::buffer_usage_t usage, T(&init_data)[N])const { return create_index_buffer(usage, collections::to_array(init_data)); }
		ang_end_interface();

		LINK idriver_t create_graphic_driver(graph_driver_type_t, platform::icore_view_t = null, isurface_ptr_t = null);	

		template<> inline buffers::iindex_buffer_t idriver::create_index_buffer(buffers::buffer_usage_t usage, array_view<short> init_data)const {
			return create_index_buffer(usage, reflect::var_type::s16, init_data.size(), collections::to_array((byte*)init_data.get(), sizeof(short) * init_data.size()));
		}
		template<> inline buffers::iindex_buffer_t idriver::create_index_buffer(buffers::buffer_usage_t usage, array_view<ushort> init_data)const {
			return create_index_buffer(usage, reflect::var_type::u16, init_data.size(), collections::to_array((byte*)init_data.get(), sizeof(ushort) * init_data.size()));
		}
		template<> inline buffers::iindex_buffer_t idriver::create_index_buffer(buffers::buffer_usage_t usage, array_view<int> init_data)const {
			return create_index_buffer(usage, reflect::var_type::s32, init_data.size(), collections::to_array((byte*)init_data.get(), sizeof(int) * init_data.size()));
		}
		template<> inline buffers::iindex_buffer_t idriver::create_index_buffer(buffers::buffer_usage_t usage, array_view<uint> init_data)const {
			return create_index_buffer(usage, reflect::var_type::u32, init_data.size(), collections::to_array((byte*)init_data.get(), sizeof(uint) * init_data.size()));
		}
	}
}

#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANG_GRAPHICS_H__