#ifndef __ANG_GRAPHICS_H__
#define __ANG_GRAPHICS_H__

#include <angsys.h>
#include <ang/dom/xml.h>
#include <ang/core/timer.h>
#include <ang/maths/maths.h>
#include <ang/maths/vectors.h>
#include <ang/platform/platform.h>

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
		ang_interface(ifactory);

		namespace resources
		{
			safe_enum(LINK, resource_type, uint)
			{
				none = 0,
				texture,
				frame_buffer,
				index_buffer,
				vertex_buffer,
				model,
				shaders,
				technique = shaders,
				pass,
				effect
			};
	
			ang_interface(ilibrary);
			ang_interface(iresource);
		}
	
		namespace reflect
		{
			typedef struct varying_desc varying_desc_t, uniform_desc, uniform_desc_t;
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
				shaders,

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
			ang_interface(itexture_loader);//for user implement

			safe_enum(LINK, tex_type, uint)
			{
				null,
				tex1D,
				tex2D,
				tex3D,
				texCube,
			};

			safe_enum(LINK, tex_format, uint)
			{
				null = 0,
				R32G32B32,
				R32G32B32A32,
				R10G310B10A2,

				A8,
				R8G8B8,
				R8G8B8A8,//Default
				R5G6B5,
				R5G5B5A1,
				R4G4B4A4,
				D16,
				D24S8,
			};
			
			safe_enum(LINK, tex_wrap_mode, uint)
			{
				def = 0,
				repeate = def,
				clamp,
				mirrored,
			};

		}

		namespace effects
		{
			ang_interface(ishaders);
			ang_interface(ipass); //unimplemented
			ang_interface(ieffect); //unimplemented
			ang_interface(ieffect_library); //for user implement
		}

		namespace scenes
		{
			ang_interface(icamera);
			ang_interface(imodel);
			ang_interface(ibone);
			ang_interface(imodel_element);
			ang_interface(iscene_object);
			ang_interface(iscene);
		}
	}

	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::idriver);
	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::isurface);
	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::iframe_buffer);

	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::buffers::igpu_buffer);
	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::buffers::iindex_buffer);
	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::buffers::ivertex_buffer);

	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::textures::itexture);
	//ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::textures::itexture_loader);

	ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::effects::ishaders);
	//ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::effects::ieffect);
	//ANG_INTF_WRAPPER_DECLARATION(LINK, graphics::effects::ieffect_library);

}

#include <ang/graphics/reflect.h>
#include <ang/graphics/resources.h>
#include <ang/graphics/buffers.h>
#include <ang/graphics/textures.h>
#include <ang/graphics/shaders.h>
#include <ang/graphics/scenes.h>


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
			vector<textures::tex_format_t> color_format;
			textures::tex_format_t depth_stencil_format;
			size<uint> dimentions;
		}frame_buffer_desc_t, *frame_buffer_desc_ptr_t;

		ang_begin_interface(LINK iframe_buffer)
			visible vcall resources::iresource_t resource()const pure
			visible vcall size<uint> dimentions()const pure
			visible vcall uint color_buffer_count()const pure
			visible vcall textures::itexture_t color_buffer(index)const pure
			visible vcall textures::itexture_t depth_stencil_buffer()const pure
			visible vcall bool try_resize(size<uint>) pure
		ang_end_interface();

		ang_begin_interface(LINK isurface)
			/*Updates Surface for resolution changes*/
			visible vcall void update() pure
			/*Presents Back Buffer*/
			visible vcall void swap_buffers(bool syncronize) pure
			/*Gets Back Buffer to Render */
			visible vcall iframe_buffer_t frame_buffer()const pure
			/*For Virtual Reality Extension Only*/
			visible vcall collections::ienum_ptr<collections::tuple<scenes::icamera_t, iframe_buffer_t>> cameras()const pure
		ang_end_interface();

		ang_begin_interface(LINK ifactory)
			/*Gets parent driver*/
			visible vcall idriver_t driver()const pure
			/*Sets a file sistem to read source files*/
			visible vcall void set_file_system(core::files::ifile_system_t)pure
			/*creates a buffer to allocate vertices*/
			visible vcall buffers::ivertex_buffer_t create_vertex_buffer(
				buffers::buffer_usage_t usage,
				vector<reflect::attribute_desc> vertex_desc,
				wsize vertex_count,
				ibuffer_t init_data,
				string sid = null
			)const pure
			/*creates a buffer to allocate vertices asynchronously*/
			visible vcall core::async::iasync<buffers::ivertex_buffer_t> create_vertex_buffer_async(
				buffers::buffer_usage_t usage,
				vector<reflect::attribute_desc> vertex_desc,
				wsize vertex_count,
				ibuffer_t init_data,
				string sid = null
			)const pure
			/*creates a buffer to allocate indices*/
			visible vcall buffers::iindex_buffer_t create_index_buffer(
				buffers::buffer_usage_t usage,
				reflect::var_type_t index_type,
				wsize index_count,
				ibuffer_t init_data,
				string sid = null
			)const pure
			/*creates a buffer to allocate indices asynchronously*/
			visible vcall core::async::iasync<buffers::iindex_buffer_t> create_index_buffer_async(
				buffers::buffer_usage_t usage,
				reflect::var_type_t index_type,
				wsize index_count,
				ibuffer_t init_data,
				string sid = null
			)const pure
			/*creates a buffer to allocate textures*/
			visible vcall textures::itexture_t create_texture(
				textures::tex_type_t type,
				textures::tex_format_t color_format,
				buffers::buffer_usage_t usage,
				buffers::buffer_bind_flag_t flags,
				size3d<uint> dimentions,
				string sid = null
			)const pure
			/*creates a buffer to allocate textures asynchronously*/
			visible vcall core::async::iasync<textures::itexture_t> create_texture_async(
				textures::tex_type_t type,
				textures::tex_format_t color_format,
				buffers::buffer_usage_t usage,
				buffers::buffer_bind_flag_t flags,
				size3d<uint> dimentions,
				string sid = null
			)const pure
			/*creates a texture object from resource (API depending resource) */
			visible vcall textures::itexture_t create_texture(
				unknown_t tex_handle,
				string sid = null
			)const pure
			/*creates a texture object from resource asynchronously (API depending resource)*/
			visible vcall core::async::iasync<textures::itexture_t> create_texture_async(
				unknown_t tex_handle,
				string sid = null
			)const pure
			/*loads textures from file*/
			visible vcall textures::itexture_t load_texture(
				text::string file,
				textures::tex_type_t,
				string sid = null
			)const pure
			/*loads textures from file asynchronously*/
			visible vcall core::async::iasync<textures::itexture_t> load_texture_async(
				text::string file,
				textures::tex_type_t,
				string sid = null
			)const pure
			/*loads textures from file*/
			visible vcall textures::itexture_t load_texture(
				array_view<text::string> files,
				textures::tex_type_t,
				string sid = null
			)const pure
			/*loads textures from file asynchronously*/
			visible vcall core::async::iasync<textures::itexture_t> load_texture_async(
				array_view<text::string> files,
				textures::tex_type_t,
				string sid = null
			)const pure
			/*create a frame buffer*/
			visible vcall iframe_buffer_t create_frame_buffer(
				array_view<textures::tex_format_t> color_format,
				textures::tex_format_t depth_stencil_format,
				size<uint> dimentions,
				string sid = null
			)const pure
			/*create a frame buffer asynchronously*/
			visible vcall core::async::iasync<iframe_buffer_t> create_frame_buffer_async(
				array_view<textures::tex_format_t> color_format,
				textures::tex_format_t depth_stencil_format,
				size<uint> dimentions,
				string sid = null
			)const pure
			/*create a frame buffer from resource*/
			visible vcall iframe_buffer_t create_frame_buffer(
				array_view<textures::itexture_t> color_tex,
				textures::itexture_t depth_stencil_format = null,
				string sid = null
			)const pure
			/*create a frame buffer from resource asynchronously*/
			visible vcall core::async::iasync<iframe_buffer_t> create_frame_buffer_async(
				array_view<textures::itexture_t> color_tex,
				textures::itexture_t depth_stencil_format = null,
				string sid = null
			)const pure
			/*create a gpu program*/
			visible vcall effects::ishaders_t compile_shaders(
				wstring vertex_shader,
				wstring pixel_shader,
				string sid = null,
				string_ptr_t log = null
			)const pure
			/*create a gpu program asynchronously*/
			visible vcall core::async::iasync<effects::ishaders_t> compile_shaders_async(
				wstring vertex_shader,
				wstring pixel_shader,
				string sid = null
			)const pure
			/*create a gpu program*/
			visible vcall effects::ishaders_t compile_shaders(
				effects::shader_info_t const& vertex_shader,
				effects::shader_info_t const& pixel_shader,
				string sid = null,
				string_ptr_t log = null
			)const pure
			/*create a gpu program asynchronously*/
			visible vcall core::async::iasync<effects::ishaders_t> compile_shaders_async(
				effects::shader_info_t const& vertex_shader,
				effects::shader_info_t const& pixel_shader,
				string sid = null
			)const pure

			template<typename T> inline buffers::iindex_buffer_t create_index_buffer(buffers::buffer_usage_t usage, array_view<T> init_data, string sid = null)const { return null; }
				template<typename T> inline buffers::iindex_buffer_t create_index_buffer(buffers::buffer_usage_t usage, array<T> init_data, string sid = null)const { return null; }
				template<typename T> inline buffers::iindex_buffer_t create_index_buffer(buffers::buffer_usage_t usage, vector<T> init_data, string sid = null)const { return null; }
			template<typename T, wsize N> inline buffers::iindex_buffer_t create_index_buffer(buffers::buffer_usage_t usage, T(&init_data)[N], string sid = null)const { return create_index_buffer(usage, collections::to_array(init_data), sid); }
		ang_end_interface();

		ang_begin_interface(LINK idriver)
			/*return the API used by the graphic driver*/
			visible vcall graph_driver_type_t graph_driver_type()const pure
			/*create a surface to present from a core_view*/
			visible vcall isurface_t create_surface(platform::icore_view_t)const pure
			/*create a factory to load resources*/
			visible vcall ifactory_t get_factory()const pure

			visible vcall void viewport(box<float>) pure
			visible vcall box<float> viewport()const pure

			visible vcall void cull_mode(cull_mode_t) pure
			visible vcall cull_mode_t cull_mode()const pure

			visible vcall void front_face(front_face_t) pure
			visible vcall front_face_t front_face()const pure

			visible vcall void blend_mode(blend_mode_t) pure
			visible vcall blend_mode_t blend_mode()const pure

			visible vcall void bind_frame_buffer(iframe_buffer_t) pure
			visible vcall void bind_shaders(effects::ishaders_t) pure
			visible vcall void bind_texture(textures::itexture_t, index) pure
			visible vcall void bind_index_buffer(buffers::iindex_buffer_t) pure
			visible vcall void bind_vertex_buffer(buffers::ivertex_buffer_t) pure

			visible vcall void clear(color_t) pure
			visible vcall void clear(maths::float4 const&) pure
			visible vcall void draw(uint count, primitive_t) pure
			visible vcall void draw_indexed(uint count, primitive_t) pure

			visible vcall core::async::mutex_ptr_t driver_guard()const pure
			visible vcall core::async::idispatcher_t dispatcher()const pure
			template<typename f_t> inline auto execute_on_thread_safe(f_t func)const -> decltype(func()) {
				core::async::scope_locker<core::async::mutex_ptr_t> _lock = driver_guard();
				return func();
			}
		ang_end_interface();

		LINK idriver_t create_graphic_driver(graph_driver_type_t, long64 adapter_id = 0, platform::icore_view_t = null, isurface_ptr_t = null);	

		template<> inline buffers::iindex_buffer_t ifactory::create_index_buffer(buffers::buffer_usage_t usage, array_view<short> init_data, string sid)const {
			ibuffer_t buffer = array<byte>(to_array((byte*)init_data.get(), size_of<ushort>() * init_data.size())).get();
			return create_index_buffer(usage, reflect::var_type::s16, init_data.size(), buffer, sid);
		}
		template<> inline buffers::iindex_buffer_t ifactory::create_index_buffer(buffers::buffer_usage_t usage, array_view<ushort> init_data, string sid)const {
			ibuffer_t buffer = array<byte>(to_array((byte*)init_data.get(), size_of<ushort>() * init_data.size())).get();
			return create_index_buffer(usage, reflect::var_type::u16, init_data.size(), buffer, sid);
		}
		template<> inline buffers::iindex_buffer_t ifactory::create_index_buffer(buffers::buffer_usage_t usage, array_view<int> init_data, string sid)const {
			ibuffer_t buffer = array<byte>(to_array((byte*)init_data.get(), size_of<int>() * init_data.size())).get();
			return create_index_buffer(usage, reflect::var_type::s32, init_data.size(), buffer, sid);
		}
		template<> inline buffers::iindex_buffer_t ifactory::create_index_buffer(buffers::buffer_usage_t usage, array_view<uint> init_data, string sid)const {
			ibuffer_t buffer = array<byte>(to_array((byte*)init_data.get(), size_of<uint>() * init_data.size())).get();
			return create_index_buffer(usage, reflect::var_type::u32, init_data.size(), buffer, sid);
		}

		template<> inline buffers::iindex_buffer_t ifactory::create_index_buffer(buffers::buffer_usage_t usage, array<short> init_data, string sid)const {
			return create_index_buffer(usage, reflect::var_type::s16, init_data->counter(), init_data.get(), sid);
		}
		template<> inline buffers::iindex_buffer_t ifactory::create_index_buffer(buffers::buffer_usage_t usage, array<ushort> init_data, string sid)const {
			return create_index_buffer(usage, reflect::var_type::s16, init_data->counter(), init_data.get(), sid);
		}
		template<> inline buffers::iindex_buffer_t ifactory::create_index_buffer(buffers::buffer_usage_t usage, array<int> init_data, string sid)const {
			return create_index_buffer(usage, reflect::var_type::s16, init_data->counter(), init_data.get(), sid);
		}
		template<> inline buffers::iindex_buffer_t ifactory::create_index_buffer(buffers::buffer_usage_t usage, array<uint> init_data, string sid)const {
			return create_index_buffer(usage, reflect::var_type::s16, init_data->counter(), init_data.get(), sid);
		}

		template<> inline buffers::iindex_buffer_t ifactory::create_index_buffer(buffers::buffer_usage_t usage, vector<short> init_data, string sid)const {
			return create_index_buffer(usage, reflect::var_type::s16, init_data->counter(), init_data.get(), sid);
		}
		template<> inline buffers::iindex_buffer_t ifactory::create_index_buffer(buffers::buffer_usage_t usage, vector<ushort> init_data, string sid)const {
			return create_index_buffer(usage, reflect::var_type::s16, init_data->counter(), init_data.get(), sid);
		}
		template<> inline buffers::iindex_buffer_t ifactory::create_index_buffer(buffers::buffer_usage_t usage, vector<int> init_data, string sid)const {
			return create_index_buffer(usage, reflect::var_type::s16, init_data->counter(), init_data.get(), sid);
		}
		template<> inline buffers::iindex_buffer_t ifactory::create_index_buffer(buffers::buffer_usage_t usage, vector<uint> init_data, string sid)const {
			return create_index_buffer(usage, reflect::var_type::s16, init_data->counter(), init_data.get(), sid);
		}
	}
}

#ifdef  LINK
#undef  LINK
#endif //LINK

#endif //__ANG_GRAPHICS_H__
