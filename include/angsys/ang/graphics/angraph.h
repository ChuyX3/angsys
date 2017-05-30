#ifndef __ANGRAPH_H__
#define __ANGRAPH_H__

#include <angsys.h>
#include <ang/xml.h>
#include <ang/maths/maths.h>
#include <ang/maths/vectors.h>
#include <ang/platform/platform.h>
#include <ang/collections/vector.h>

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
		struct idriver;
		struct isurface;
		struct iframe_buffer;

		typedef intf_wrapper<idriver> idriver_t;
		typedef intf_wrapper<isurface> isurface_t;
		typedef intf_wrapper<iframe_buffer> iframe_buffer_t;

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

		//namespace scenes
		//{
		//	struct iscene;
		//	struct iscene_object;
		//	struct iscene_manager;

		//	typedef intf_wrapper<iscene> iscene_t;
		//	typedef intf_wrapper<iscene_object> iscene_object_t;
		//	typedef intf_wrapper<iscene_manager> iscene_manager_t;
		//}
	}
}

#include <ang/graphics/reflect.h>
#include <ang/graphics/buffers.h>
#include <ang/graphics/textures.h>
#include <ang/graphics/effects.h>

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

		ANG_BEGIN_INTERFACE(LINK, iframe_buffer)
			visible vcall foundation::size<float> dimentions()const pure;
			visible vcall uint color_buffer_count()const pure;
			visible vcall bool has_depth_stencil_buffer()const pure;
		ANG_END_INTERFACE();

		ANG_BEGIN_INTERFACE(LINK, isurface)
			visible vcall void swap_buffers() pure;
			visible vcall iframe_buffer_t frame_buffer()const pure;
		ANG_END_INTERFACE();


		ANG_BEGIN_INTERFACE(LINK, idriver)
			visible vcall isurface_t create_surface(platform::icore_view_t)const pure;
			visible vcall effects::ieffect_library_t create_effect_library()const pure;
			visible vcall textures::itexture_loader_t create_texture_loader()const pure;
			visible vcall buffers::ivertex_buffer_t create_vertex_buffer(
				buffers::buffer_usage_t usage,
				static_array<reflect::attribute_desc> vertex_desc,
				wsize vertex_count,
				static_array<byte> init_data
			)const pure;
			visible vcall buffers::iindex_buffer_t create_index_buffer(
				buffers::buffer_usage_t usage,
				reflect::var_type_t index_type,
				wsize index_count,
				static_array<byte> init_data
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

			visible vcall core::async::mutex_t driver_guard()const pure;
			template<class f_t> inline auto execute_on_thread_safe(f_t f)const ->decltype(f()) {
				core::async::scope_locker locker = driver_guard();
				return f();
			}
		ANG_END_INTERFACE();

		LINK idriver_t create_graphic_driver(graph_driver_type_t);

	}
}



#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANGRAPH_H__