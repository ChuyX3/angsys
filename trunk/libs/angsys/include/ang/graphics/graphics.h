#ifndef __ANG_GRAPHICS_H__
#define __ANG_GRAPHICS_H__

#include <angsys.h>
#include <ang/dom/xml.h>
#include <ang/core/time.h>
#include <ang/maths/maths.h>
#include <ang/maths/vectors.h>
#include <ang/platform/platform.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANG_DYNAMIC_LIBRARY

#ifdef ANG_GRAPHICS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANG_GRAPHICS_EXPORTS
#else//#elif defined ANG_STATIC_LIBRARY
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
		ang_declare_interface(idriver);
		ang_declare_interface(isurface);
		ang_declare_interface(iframe_buffer);
		ang_declare_interface(ifactory);

		using driver_locker_t = core::async::scope_locker<graphics::idriver_t>;

		declare_enum(LINK, cull_mode, uint)
		{
			none = 0,
			front,
			back,
		};

		declare_enum(LINK, front_face, uint)
		{
			def,
			clockwise,
			counter_clockwise = 0,
		};

		declare_enum(LINK, blend_mode, uint)
		{
			disable,
			enable,
		};

		namespace resources
		{
			declare_enum(LINK, resource_type, uint)
			{
				none = 0,
				texture,
				frame_buffer,
				index_buffer,
				vertex_buffer,
				mesh,
				animation,
				shaders,
				pass,
				effect
			};
	
			declare_enum(LINK, light_type, uint)
			{
				directional,
					spot,
			};

			ang_declare_interface(ilibrary);
			ang_declare_interface(iresource);
			
			
		}
	
		namespace reflect
		{
			ang_declare_object(struct_buffer);
			typedef class varying varying_t;
			typedef struct varying_desc varying_desc_t, uniform_desc, uniform_desc_t;
			typedef struct attribute_desc attribute_desc_t;

			declare_enum(LINK, var_type, uint)
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

			declare_enum(LINK, var_class, uint)
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

			declare_enum(LINK, var_semantic, uint)
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
			ang_declare_interface(igpu_buffer);
			ang_declare_interface(iindex_buffer);
			ang_declare_interface(ivertex_buffer);

			declare_enum(LINK, buffer_type, uint)
			{
				none,
				texture,
				vertex_buffer,
				index_buffer,
			};

			declare_enum(LINK, buffer_usage, uint)
			{
				def,
				inmutable,
				dynamic,
				staging,
			};

			declare_flags(LINK, buffer_bind_flag, uint)
			{
				none = 0x0L,
				vertex_buffer = 0x1L,
				index_buffer = 0x2L,
				uniform_buffer = 0x4L,
				shader_resource = 0x8L,
				stream_output = 0x10L,
				render_target = 0x20L,
				depth_stencil = 0x40L,
			};

		}

		namespace textures
		{
			ang_declare_interface(itexture);
			ang_declare_interface(itexture_loader);//for user implement

			declare_enum(LINK, tex_type, uint)
			{
				null,
				tex1D,
				tex2D,
				tex3D,
				texCube,
			};

			declare_enum(LINK, tex_format, uint)
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
			
			declare_enum(LINK, tex_wrap_mode, uint)
			{
				def = 0,
				repeate = def,
				clamp,
				mirrored,
			};

		}

		namespace effects
		{
			ang_declare_interface(ishaders);
			ang_declare_interface(ipass); //unimplemented
			ang_declare_interface(ieffect); //unimplemented
			ang_declare_interface(ieffect_library); //for user implement
		}

		namespace meshes
		{
			ang_declare_interface(imaterial);
			ang_declare_interface(igeometry);
			ang_declare_interface(ianimation);
			ang_declare_interface(ibone);
			ang_declare_interface(imesh);
			ang_declare_interface(imesh_loader);
		}

		namespace scenes
		{
			ang_declare_interface(itransform);
			ang_declare_interface(inode);
			ang_declare_interface(imodel);
			ang_declare_interface(icamera);	
			ang_declare_interface(iscene);

			declare_enum(LINK, node_type, uint)
			{
				none = 0,
				scene,
				camera,
				model,
			};
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

	namespace core
	{
		namespace async
		{
			template<> struct LINK scope_locker<graphics::idriver_t>
			{
				graphics::idriver_t m_driver;
			public:
				scope_locker(graphics::idriver_t);
				~scope_locker();
			};
		}
	}

}

#include <ang/graphics/reflect.h>
#include <ang/graphics/resources.h>
#include <ang/graphics/buffers.h>
#include <ang/graphics/textures.h>
#include <ang/graphics/shaders.h>
#include <ang/graphics/meshes.h>
#include <ang/graphics/scenes.h>


namespace ang
{
	namespace graphics
	{
	
		typedef struct frame_buffer_desc
		{
			vector<textures::tex_format_t> color_format;
			textures::tex_format_t depth_stencil_format;
			size<uint> dimentions;
			bool is_stereo;
		}frame_buffer_desc_t, *frame_buffer_desc_ptr_t;

		struct nvt LINK iframe_buffer
			: intf<iframe_buffer, iid("ang::graphics::iframe_buffer")>
		{
			virtual resources::iresource_t resource()const = 0;
			virtual bool is_stereo()const = 0;
			virtual size<uint> dimentions()const = 0;
			virtual uint color_buffer_count()const = 0;
			virtual textures::itexture_t color_buffer(windex)const = 0;
			virtual textures::itexture_t depth_stencil_buffer()const = 0;
			virtual bool try_resize(size<uint>) = 0;
		};

		struct nvt LINK isurface
			: intf<isurface, iid("ang::graphics::isurface")>
		{
			/*Updates Surface, return value for Virtual Reality Extension Only (otherwise return null) */
			virtual collections::ienum_ptr<collections::tuple<scenes::itransform_t, iframe_buffer_t>> update() = 0;
			/*Presents Back Buffer*/
			virtual bool swap_buffers(bool syncronize) = 0;
			/*Gets Back Buffer to Render */
			virtual iframe_buffer_t frame_buffer()const = 0;
			
		};

		struct nvt LINK ifactory
			: intf<ifactory, iid("ang::graphics::ifactory")>
		{
			/*Gets parent driver*/
			virtual idriver_t driver()const = 0;
			/*Sets a file sistem to read source files*/
			virtual void set_file_system(core::files::ifile_system_t)= 0;
			/*creates a buffer to allocate vertices*/
			virtual optional<buffers::ivertex_buffer> create_vertex_buffer(
				buffers::buffer_usage_t usage,
				vector<reflect::attribute_desc> vertex_desc,
				wsize vertex_count,
				ibuffer_t init_data,
				string sid = null
			)const = 0;
			/*creates a buffer to allocate vertices asynchronously*/
			virtual core::async::iasync_op<buffers::ivertex_buffer> create_vertex_buffer_async(
				buffers::buffer_usage_t usage,
				vector<reflect::attribute_desc> vertex_desc,
				wsize vertex_count,
				ibuffer_t init_data,
				string sid = null
			)const = 0;
			/*creates a buffer to allocate indices*/
			virtual optional<buffers::iindex_buffer> create_index_buffer(
				buffers::buffer_usage_t usage,
				reflect::var_type_t index_type,
				wsize index_count,
				ibuffer_t init_data,
				string sid = null
			)const = 0;
			/*creates a buffer to allocate indices asynchronously*/
			virtual core::async::iasync_op<buffers::iindex_buffer> create_index_buffer_async(
				buffers::buffer_usage_t usage,
				reflect::var_type_t index_type,
				wsize index_count,
				ibuffer_t init_data,
				string sid = null
			)const = 0;
			/*creates a buffer to allocate textures*/
			virtual optional<textures::itexture> create_texture(
				textures::tex_type_t type,
				textures::tex_format_t color_format,
				buffers::buffer_usage_t usage,
				buffers::buffer_bind_flag_t flags,
				size3d<uint> dimentions,
				string sid = null
			)const = 0;
			/*creates a buffer to allocate textures asynchronously*/
			virtual core::async::iasync_op<textures::itexture> create_texture_async(
				textures::tex_type_t type,
				textures::tex_format_t color_format,
				buffers::buffer_usage_t usage,
				buffers::buffer_bind_flag_t flags,
				size3d<uint> dimentions,
				string sid = null
			)const = 0;
			/*creates a texture object from resource (API depending resource) */
			virtual optional<textures::itexture> create_texture(
				unknown_t tex_handle,
				string sid = null
			)const = 0;
			/*creates a texture object from resource asynchronously (API depending resource)*/
			virtual core::async::iasync_op<textures::itexture> create_texture_async(
				unknown_t tex_handle,
				string sid = null
			)const = 0;
			/*loads textures from file*/
			virtual optional<textures::itexture> load_texture(
				text::string file,
				textures::tex_type_t,
				string sid = null
			)const = 0;
			/*loads textures from file asynchronously*/
			virtual core::async::iasync_op<textures::itexture> load_texture_async(
				text::string file,
				textures::tex_type_t,
				string sid = null
			)const = 0;
			/*loads textures from file*/
			virtual optional<textures::itexture> load_texture(
				array_view<text::string> files,
				textures::tex_type_t,
				string sid = null
			)const = 0;
			/*loads textures from file asynchronously*/
			virtual core::async::iasync_op<textures::itexture> load_texture_async(
				array_view<text::string> files,
				textures::tex_type_t,
				string sid = null
			)const = 0;
			/*create a frame buffer*/
			virtual optional<iframe_buffer> create_frame_buffer(
				array_view<textures::tex_format_t> color_format,
				textures::tex_format_t depth_stencil_format,
				size<uint> dimentions,
				bool is_stereo = false,
				string sid = null
			)const = 0;
			/*create a frame buffer asynchronously*/
			virtual core::async::iasync_op<iframe_buffer> create_frame_buffer_async(
				array_view<textures::tex_format_t> color_format,
				textures::tex_format_t depth_stencil_format,
				size<uint> dimentions,
				bool is_stereo = false,
				string sid = null
			)const = 0;
			/*create a frame buffer from resource*/
			virtual optional<iframe_buffer> create_frame_buffer(
				array_view<textures::itexture_t> color_tex,
				textures::itexture_t depth_stencil_format = null,
				string sid = null
			)const = 0;
			/*create a frame buffer from resource asynchronously*/
			virtual core::async::iasync_op<iframe_buffer> create_frame_buffer_async(
				array_view<textures::itexture_t> color_tex,
				textures::itexture_t depth_stencil_format = null,
				string sid = null
			)const = 0;
			/*create a gpu program*/
			virtual optional<effects::ishaders> compile_shaders(
				string vertex_shader,
				string pixel_shader,
				string sid = null
			)const = 0;
			/*create a gpu program asynchronously*/
			virtual core::async::iasync_op<effects::ishaders> compile_shaders_async(
				string vertex_shader,
				string pixel_shader,
				string sid = null
			)const = 0;
			/*create a gpu program*/
			virtual optional<effects::ishaders> compile_shaders(
				effects::shader_info_t const& vertex_shader,
				effects::shader_info_t const& pixel_shader,
				string sid = null
			)const = 0;
			/*create a gpu program asynchronously*/
			virtual core::async::iasync_op<effects::ishaders> compile_shaders_async(
				effects::shader_info_t const& vertex_shader,
				effects::shader_info_t const& pixel_shader,
				string sid = null
			)const = 0;

			inline optional<buffers::iindex_buffer> create_index_buffer(buffers::buffer_usage_t usage, reflect::struct_buffer_t data, string sid = null)const;
			inline core::async::iasync_op<buffers::iindex_buffer> create_index_buffer_async(buffers::buffer_usage_t usage, reflect::struct_buffer_t data, string sid = null)const;

			template<typename T> inline optional<buffers::iindex_buffer> create_index_buffer(buffers::buffer_usage_t usage, array_view<T> init_data, string sid = null)const { return null; }
			template<typename T> inline optional<buffers::iindex_buffer> create_index_buffer(buffers::buffer_usage_t usage, smart_array<T> init_data, string sid = null)const { return null; }
			template<typename T> inline optional<buffers::iindex_buffer> create_index_buffer(buffers::buffer_usage_t usage, smart_vector<T> init_data, string sid = null)const { return null; }
			template<typename T, wsize N> inline optional<buffers::iindex_buffer> create_index_buffer(buffers::buffer_usage_t usage, T(&init_data)[N], string sid = null)const { return create_index_buffer(usage, collections::to_array(init_data), sid); }

			template<typename T> inline core::async::iasync_op<buffers::iindex_buffer> create_index_buffer_async(buffers::buffer_usage_t usage, array_view<T> init_data, string sid = null)const { return null; }
			template<typename T> inline core::async::iasync_op<buffers::iindex_buffer> create_index_buffer_async(buffers::buffer_usage_t usage, smart_array<T> init_data, string sid = null)const { return null; }
			template<typename T> inline core::async::iasync_op<buffers::iindex_buffer> create_index_buffer_async(buffers::buffer_usage_t usage, smart_vector<T> init_data, string sid = null)const { return null; }
			template<typename T, wsize N> inline core::async::iasync_op<buffers::iindex_buffer> create_index_buffer_async(buffers::buffer_usage_t usage, T(&init_data)[N], string sid = null)const { return create_index_buffer_async(usage, collections::to_array(init_data), sid); }
		};

		struct nvt LINK idriver 
			: intf<idriver, iid("ang::graphics::idriver")>
		{
			/*return the API used by the graphic driver*/
			virtual graph_driver_type_t graph_driver_type()const = 0;
			/*return the API used by the graphic driver*/
			virtual bool matrix_transpose_needed()const = 0;
			/*return the API used by the graphic driver*/
			virtual bool vr_extension_support()const = 0;
			/*return the graphic driver min version */
			virtual ulong min_version()const = 0;
			/*return the graphic driver max version */
			virtual ulong max_version()const = 0;
			/*create a surface to present from a core_view*/
			virtual isurface_t create_surface(platform::icore_view_t)const = 0;
			/*create a factory to load resources*/
			virtual ifactory_t get_factory()const = 0;

			virtual void viewport(box<float>) = 0;
			virtual box<float> viewport()const = 0;

			virtual void cull_mode(cull_mode_t) = 0;
			virtual cull_mode_t cull_mode()const = 0;

			virtual void front_face(front_face_t) = 0;
			virtual front_face_t front_face()const = 0;

			virtual void blend_mode(blend_mode_t) = 0;
			virtual blend_mode_t blend_mode()const = 0;

			virtual void bind_frame_buffer(iframe_buffer_t) = 0;
			virtual void bind_shaders(effects::ishaders_t) = 0;
			virtual void bind_texture(textures::itexture_t, windex) = 0;
			virtual void bind_index_buffer(buffers::iindex_buffer_t) = 0;
			virtual void bind_vertex_buffer(buffers::ivertex_buffer_t) = 0;

			virtual void clear(color_t) = 0;
			virtual void clear(maths::float4 const&) = 0;
			virtual void draw(uint count, primitive_t) = 0;
			virtual void draw_indexed(uint count, primitive_t) = 0;

			virtual bool lock()const = 0;
			virtual bool unlock()const = 0;
			virtual core::async::idispatcher_t dispatcher()const = 0;
			template<typename f_t> inline auto execute_on_thread_safe(f_t func)const -> decltype(func()) {
				driver_locker_t _lock = const_cast<idriver*>(this);
				return func();
			}
		};

		LINK idriver_t create_graphic_driver(graph_driver_type_t, long64 adapter_id = 0, platform::icore_view_t = null, isurface_ptr_t = null);	

		inline optional<buffers::iindex_buffer> ifactory::create_index_buffer(buffers::buffer_usage_t usage, reflect::struct_buffer_t data, string sid)const {
			return create_index_buffer(usage, data->descriptor().var_type(), data->descriptor().array_count(), data.get(), sid);
		}
		
		inline core::async::iasync_op<buffers::iindex_buffer> ifactory::create_index_buffer_async(buffers::buffer_usage_t usage, reflect::struct_buffer_t data, string sid )const {
			return create_index_buffer_async(usage, data->descriptor().var_type(), data->descriptor().array_count(), data.get(), sid);
		}


		template<> inline optional<buffers::iindex_buffer> ifactory::create_index_buffer(buffers::buffer_usage_t usage, array_view<short> init_data, string sid)const {
			ibuffer_t buffer = smart<array<byte>>(to_array((byte*)init_data.get(), (byte*)init_data.get() + size_of<ushort>() * init_data.size())).get();
			return create_index_buffer(usage, reflect::var_type::s16, init_data.size(), buffer, sid);
		}
		template<> inline optional<buffers::iindex_buffer> ifactory::create_index_buffer(buffers::buffer_usage_t usage, array_view<ushort> init_data, string sid)const {
			ibuffer_t buffer = smart<array<byte>>(to_array((byte*)init_data.get(), (byte*)init_data.get() + size_of<ushort>() * init_data.size())).get();
			return create_index_buffer(usage, reflect::var_type::u16, init_data.size(), buffer, sid);
		}
		template<> inline optional<buffers::iindex_buffer> ifactory::create_index_buffer(buffers::buffer_usage_t usage, array_view<int> init_data, string sid)const {
			ibuffer_t buffer = smart<array<byte>>(to_array((byte*)init_data.get(), (byte*)init_data.get() + size_of<int>() * init_data.size())).get();
			return create_index_buffer(usage, reflect::var_type::s32, init_data.size(), buffer, sid);
		}
		template<> inline optional<buffers::iindex_buffer> ifactory::create_index_buffer(buffers::buffer_usage_t usage, array_view<uint> init_data, string sid)const {
			ibuffer_t buffer = smart<array<byte>>(to_array((byte*)init_data.get(), (byte*)init_data.get() + size_of<uint>() * init_data.size())).get();
			return create_index_buffer(usage, reflect::var_type::u32, init_data.size(), buffer, sid);
		}

		template<> inline optional<buffers::iindex_buffer> ifactory::create_index_buffer(buffers::buffer_usage_t usage, smart_array<short> init_data, string sid)const {
			return create_index_buffer(usage, reflect::var_type::s16, init_data->counter(), init_data.get(), sid);
		}
		template<> inline optional<buffers::iindex_buffer> ifactory::create_index_buffer(buffers::buffer_usage_t usage, smart_array<ushort> init_data, string sid)const {
			return create_index_buffer(usage, reflect::var_type::u16, init_data->counter(), init_data.get(), sid);
		}
		template<> inline optional<buffers::iindex_buffer> ifactory::create_index_buffer(buffers::buffer_usage_t usage, smart_array<int> init_data, string sid)const {
			return create_index_buffer(usage, reflect::var_type::s32, init_data->counter(), init_data.get(), sid);
		}
		template<> inline optional<buffers::iindex_buffer> ifactory::create_index_buffer(buffers::buffer_usage_t usage, smart_array<uint> init_data, string sid)const {
			return create_index_buffer(usage, reflect::var_type::u32, init_data->counter(), init_data.get(), sid);
		}

		template<> inline optional<buffers::iindex_buffer> ifactory::create_index_buffer(buffers::buffer_usage_t usage, smart_vector<short> init_data, string sid)const {
			return create_index_buffer(usage, reflect::var_type::s16, init_data->counter(), init_data.get(), sid);
		}
		template<> inline optional<buffers::iindex_buffer> ifactory::create_index_buffer(buffers::buffer_usage_t usage, smart_vector<ushort> init_data, string sid)const {
			return create_index_buffer(usage, reflect::var_type::u16, init_data->counter(), init_data.get(), sid);
		}
		template<> inline optional<buffers::iindex_buffer> ifactory::create_index_buffer(buffers::buffer_usage_t usage, smart_vector<int> init_data, string sid)const {
			return create_index_buffer(usage, reflect::var_type::s32, init_data->counter(), init_data.get(), sid);
		}
		template<> inline optional<buffers::iindex_buffer> ifactory::create_index_buffer(buffers::buffer_usage_t usage, smart_vector<uint> init_data, string sid)const {
			return create_index_buffer(usage, reflect::var_type::u32, init_data->counter(), init_data.get(), sid);
		}

		template<> inline core::async::iasync_op<buffers::iindex_buffer> ifactory::create_index_buffer_async(buffers::buffer_usage_t usage, array_view<short> init_data, string sid)const {
			ibuffer_t buffer = smart_array<byte>(to_array((byte*)init_data.get(), (byte*)init_data.get() + size_of<ushort>() * init_data.size())).get();
			return create_index_buffer_async(usage, reflect::var_type::s16, init_data.size(), buffer, sid);
		}
		template<> inline core::async::iasync_op<buffers::iindex_buffer> ifactory::create_index_buffer_async(buffers::buffer_usage_t usage, array_view<ushort> init_data, string sid)const {
			ibuffer_t buffer = smart_array<byte>(to_array((byte*)init_data.get(), (byte*)init_data.get() + size_of<ushort>() * init_data.size())).get();
			return create_index_buffer_async(usage, reflect::var_type::u16, init_data.size(), buffer, sid);
		}
		template<> inline core::async::iasync_op<buffers::iindex_buffer> ifactory::create_index_buffer_async(buffers::buffer_usage_t usage, array_view<int> init_data, string sid)const {
			ibuffer_t buffer = smart_array<byte>(to_array((byte*)init_data.get(), (byte*)init_data.get() + size_of<int>() * init_data.size())).get();
			return create_index_buffer_async(usage, reflect::var_type::s32, init_data.size(), buffer, sid);
		}
		template<> inline core::async::iasync_op<buffers::iindex_buffer> ifactory::create_index_buffer_async(buffers::buffer_usage_t usage, array_view<uint> init_data, string sid)const {
			ibuffer_t buffer = smart_array<byte>(to_array((byte*)init_data.get(), (byte*)init_data.get() + size_of<uint>() * init_data.size())).get();
			return create_index_buffer_async(usage, reflect::var_type::u32, init_data.size(), buffer, sid);
		}

		template<> inline core::async::iasync_op<buffers::iindex_buffer> ifactory::create_index_buffer_async(buffers::buffer_usage_t usage, smart_array<short> init_data, string sid)const {
			return create_index_buffer_async(usage, reflect::var_type::s16, init_data->counter(), init_data.get(), sid);
		}
		template<> inline core::async::iasync_op<buffers::iindex_buffer> ifactory::create_index_buffer_async(buffers::buffer_usage_t usage, smart_array<ushort> init_data, string sid)const {
			return create_index_buffer_async(usage, reflect::var_type::u16, init_data->counter(), init_data.get(), sid);
		}
		template<> inline core::async::iasync_op<buffers::iindex_buffer> ifactory::create_index_buffer_async(buffers::buffer_usage_t usage, smart_array<int> init_data, string sid)const {
			return create_index_buffer_async(usage, reflect::var_type::s32, init_data->counter(), init_data.get(), sid);
		}
		template<> inline core::async::iasync_op<buffers::iindex_buffer> ifactory::create_index_buffer_async(buffers::buffer_usage_t usage, smart_array<uint> init_data, string sid)const {
			return create_index_buffer_async(usage, reflect::var_type::u32, init_data->counter(), init_data.get(), sid);
		}

		template<> inline core::async::iasync_op<buffers::iindex_buffer> ifactory::create_index_buffer_async(buffers::buffer_usage_t usage, smart_vector<short> init_data, string sid)const {
			return create_index_buffer_async(usage, reflect::var_type::s16, init_data->counter(), init_data.get(), sid);
		}
		template<> inline core::async::iasync_op<buffers::iindex_buffer> ifactory::create_index_buffer_async(buffers::buffer_usage_t usage, smart_vector<ushort> init_data, string sid)const {
			return create_index_buffer_async(usage, reflect::var_type::u16, init_data->counter(), init_data.get(), sid);
		}
		template<> inline core::async::iasync_op<buffers::iindex_buffer> ifactory::create_index_buffer_async(buffers::buffer_usage_t usage, smart_vector<int> init_data, string sid)const {
			return create_index_buffer_async(usage, reflect::var_type::s32, init_data->counter(), init_data.get(), sid);
		}
		template<> inline core::async::iasync_op<buffers::iindex_buffer> ifactory::create_index_buffer_async(buffers::buffer_usage_t usage, smart_vector<uint> init_data, string sid)const {
			return create_index_buffer_async(usage, reflect::var_type::u32, init_data->counter(), init_data.get(), sid);
		}
	}
}

ANG_ENUM_DECLARATION(LINK, ang::graphics::blend_mode)
ANG_ENUM_DECLARATION(LINK, ang::graphics::cull_mode)
ANG_ENUM_DECLARATION(LINK, ang::graphics::front_face)
ANG_ENUM_DECLARATION(LINK, ang::graphics::resources::resource_type)
ANG_ENUM_DECLARATION(LINK, ang::graphics::resources::light_type)
ANG_ENUM_DECLARATION(LINK, ang::graphics::reflect::var_class)
ANG_ENUM_DECLARATION(LINK, ang::graphics::reflect::var_type)
ANG_ENUM_DECLARATION(LINK, ang::graphics::reflect::var_semantic)
ANG_FLAGS_DECLARATION(LINK, ang::graphics::buffers::buffer_bind_flag)
ANG_ENUM_DECLARATION(LINK, ang::graphics::buffers::buffer_type)
ANG_ENUM_DECLARATION(LINK, ang::graphics::buffers::buffer_usage)
ANG_ENUM_DECLARATION(LINK, ang::graphics::textures::tex_format)
ANG_ENUM_DECLARATION(LINK, ang::graphics::textures::tex_type)
ANG_ENUM_DECLARATION(LINK, ang::graphics::textures::tex_wrap_mode)
ANG_ENUM_DECLARATION(LINK, ang::graphics::scenes::node_type)

#ifdef  LINK
#undef  LINK
#endif //LINK

#endif //__ANG_GRAPHICS_H__
