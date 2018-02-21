#ifndef __ANGRAPH_HPP__
#define __ANGRAPH_HPP__

#include <angsys.hpp>
#include <ang/xml.hpp>
#include <ang/core/files.hpp>
#include <ang/maths/maths.hpp>
#include <ang/maths/vectors.hpp>
#include <ang/platform/platform.hpp>
#include <ang/collections/map.hpp>
#include <ang/collections/vector.hpp>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGRAPH_EXPORTS
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
		ANG_INTERFACE(idriver);
		ANG_INTERFACE(isurface);
		ANG_INTERFACE(iframe_buffer);

		namespace reflect
		{
			typedef struct variable_desc variable_desc_t;
			typedef struct attribute_desc attribute_desc_t;

			ANG_BEGIN_ENUM(LINK, var_type, uint)
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
			ANG_END_ENUM(var_type);

			ANG_BEGIN_ENUM(LINK, var_class, uint)
				none = 0,
				scalar = 1,
				vec2 = 2,
				vec3 = 3,
				vec4 = 4,
				mat2 = 4,
				mat3 = 9,
				mat4 = 16,
			ANG_END_ENUM(var_class);

			ANG_BEGIN_ENUM(LINK, var_semantic, uint)
				none = 0,
				position,
				color,
				tex_coord,
				normal,
				binormal,
				tangent,
				fog,
			ANG_END_ENUM(var_semantic);
		}

		namespace buffers
		{
			ANG_INTERFACE(igpu_buffer);
			ANG_INTERFACE(iindex_buffer);
			ANG_INTERFACE(ivertex_buffer);
		}

		namespace textures
		{
			ANG_INTERFACE(itexture);
			ANG_INTERFACE(itexture_loader);
		}

		namespace effects
		{
			ANG_INTERFACE(ishaders);
			ANG_INTERFACE(ieffect);
			ANG_INTERFACE(ieffect_library);
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

#include <ang/graphics/reflect.hpp>
#include <ang/graphics/buffers.hpp>
#include <ang/graphics/textures.hpp>
#include <ang/graphics/effects.hpp>
//#include <ang/graphics/geometry.hpp>

namespace ang
{
	namespace graphics
	{

		ANG_BEGIN_ENUM(LINK, cull_mode, uint)
			none = 0,
			front,
			back,
		ANG_END_ENUM(cull_mode);

		ANG_BEGIN_ENUM(LINK, front_face, uint)
			def,
			clockwise,
			counter_clockwise = def,
		ANG_END_ENUM(front_face);

		ANG_BEGIN_ENUM(LINK, blend_mode, uint)
			disable,
			enable,
		ANG_END_ENUM(blend_mode);

		typedef struct frame_buffer_desc
		{
			array_view<textures::tex_format_t> color_format;
			textures::tex_format_t depth_stencil_format;
			graphics::size<float> dimentions;
		}frame_buffer_desc_t, *frame_buffer_desc_ptr_t;

		ANG_BEGIN_INTERFACE(LINK, iframe_buffer)
			visible vcall graphics::size<float> dimentions()const pure;
			visible vcall uint color_buffer_count()const pure;
			visible vcall textures::itexture_t color_buffer(index)const pure;
			visible vcall bool has_depth_stencil_buffer()const pure;
		ANG_END_INTERFACE();

		ANG_BEGIN_INTERFACE(LINK, isurface)
			visible vcall void swap_buffers() pure;
			visible vcall iframe_buffer_t frame_buffer()const pure;
		ANG_END_INTERFACE();


		ANG_BEGIN_INTERFACE(LINK, idriver)
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
		ANG_END_INTERFACE();

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

#endif//__ANGRAPH_HPP__