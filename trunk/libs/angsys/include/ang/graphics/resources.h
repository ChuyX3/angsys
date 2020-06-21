#ifndef __ANG_GRAPHICS_H__
#elif !defined __ANG_GRAPHICS_RESOURCES_H__
#define __ANG_GRAPHICS_RESOURCES_H__

namespace std {
	template<> struct is_base_of<ang::iintf, ang::graphics::resources::ilibrary> : true_type { };
	template<> struct is_base_of<ang::object, ang::graphics::resources::ilibrary> : false_type { };
}

namespace ang
{
	namespace graphics
	{
		namespace resources
		{
			template<resource_type TYPE>
			struct resource_type_helper {
				using type = iintf;
				using type_ptr = intfptr;
			};

			template<> struct resource_type_helper<resource_type::texture> {
				using type = textures::itexture;
				using type_ptr = textures::itexture_t;
			};

			template<> struct resource_type_helper<resource_type::frame_buffer> {
				using type = iframe_buffer;
				using type_ptr = iframe_buffer_t;
			};

			template<> struct resource_type_helper<resource_type::index_buffer> {
				using type = buffers::iindex_buffer;
				using type_ptr = buffers::iindex_buffer_t;
			};

			template<> struct resource_type_helper<resource_type::vertex_buffer> {
				using type = buffers::ivertex_buffer;
				using type_ptr = buffers::ivertex_buffer_t;
			};

			template<> struct resource_type_helper<resource_type::mesh> {
				using type = meshes::imesh;
				using type_ptr = meshes::imesh_t;
			};

			template<> struct resource_type_helper<resource_type::animation> {
				using type = meshes::ianimation;
				using type_ptr = meshes::ianimation_t;
			};

			template<> struct resource_type_helper<resource_type::shaders> {
				using type = effects::ishaders;
				using type_ptr = effects::ishaders_t;
			};

			template<> struct resource_type_helper<resource_type::pass> {
				using type = effects::ipass;
				using type_ptr = effects::ipass_t;
			};

			template<> struct resource_type_helper<resource_type::effect> {
				using type = effects::ieffect;
				using type_ptr = effects::ieffect_t;
			};

			struct light_info
			{
				maths::float3 color;
				maths::float3 info;// position or direction
				light_type_t type;
			};

			/*See more: http://paulbourke.net/dataformats/mtl/ */
			typedef struct alignas(16) material_data
			{
				/*Most common data:
				"uint illum" statement specifies the illumination model to use in the material.
					It can be a number from 0 to 10:
				 0	Color on and Ambient off
				 1	Color on and Ambient on
				 2	Highlight on
				 3	Reflection on and Ray trace on
				 4	Transparency: Glass on
					Reflection: Ray trace on
				 5	Reflection: Fresnel on and Ray trace on
				 6	Transparency: Refraction on
					Reflection: Fresnel off and Ray trace on
				 7	Transparency: Refraction on
					Reflection: Fresnel on and Ray trace on
				 8	Reflection on and Ray trace off
				 9	Transparency: Glass on
					Reflection: Ray trace off
				 10	Casts shadows onto invisible surfaces
				"float d" specifies the dissolve for the current material
				"factor" is the amount this material dissolves into the background.  A
				factor of 1.0 is fully opaque.  This is the default when a new material
				is created.  A factor of 0.0 is fully dissolved (completely
				transparent)
				"d -halo factor" specifies that a dissolve is dependent on the surface orientation
				relative to the viewer.  For example, a sphere with the following dissolve,
				d -halo 0.0, will be fully dissolved at its center and will
				appear gradually more opaque toward its edge.
				"float Ns" exponent specifies the specular exponent for the current material.  This defines
				the focus of the specular highlight.
				"float3 Ka" statement specifies the ambient reflectivity of the current material
				"float3 Kd" statement specifies the diffuse reflectivity of the current material
				"float3 Ks" statement specifies the specular reflectivity of the current material
				"float3 Tf" statement specifies the transmission filter of the current material.
				Any light passing through the object is filtered by the transmission
				filter, which only allows the specifiec colors to pass through*/
				reflect::struct_buffer_t fields;

				/* Most common samplers
				- map_Ka: ambient color texture
				- map_Kd: diffuse color texture
				- map_Ks: specular color texture
				- map_d: alpha channel texture*/
				collections::hash_map<string, string> samplers;
			}material_data_t;

			typedef struct geometry_data
			{
				string technique_name;
				collections::tuple<reflect::var_type_t, windex, ibuffer_t, string> index_data;
				collections::tuple<array<reflect::attribute_desc>, windex, ibuffer_t, string> vertex_data;
				material_data_t material;

				geometry_data() {
					technique_name = null;
					index_data = { reflect::var_type::none, 0, null };
					vertex_data = { null, 0, null };
				}
				geometry_data(geometry_data&& other) {
					technique_name = ang::move(other.technique_name);
					index_data = ang::move(other.index_data);
					vertex_data = ang::move(other.vertex_data);
					material.fields = ang::move(other.material.fields);
					material.samplers = ang::move(other.material.samplers);
				}
				geometry_data(geometry_data const& other) {
					technique_name = other.technique_name;
					index_data = other.index_data;
					vertex_data = other.vertex_data;
					material.fields = new reflect::struct_buffer(other.material.fields.get());
					material.samplers.copy(other.material.samplers);
				}

				geometry_data& operator = (geometry_data&& other) {
					if (this != &other)
					{
						technique_name = ang::move(other.technique_name);
						index_data = ang::move(other.index_data);
						vertex_data = ang::move(other.vertex_data);
						material.fields = ang::move(other.material.fields);
						material.samplers = ang::move(other.material.samplers);
					}
					return*this;
				}
				geometry_data& operator = (geometry_data const& other) {
					if (this != &other)
					{
						technique_name = other.technique_name;
						index_data = other.index_data;
						vertex_data = other.vertex_data;
						material.fields = new reflect::struct_buffer(other.material.fields.get());
						material.samplers.copy(other.material.samplers);
					}
					return*this;
				}
			}geometry_data_t;

			typedef struct animation_data {
				//TODO:
				uint dummy;
			}animation_data_t;

			struct nvt LINK iresource
				: intf<iresource
				, iid("ang::graphics::resources::iresource")>
			{
				virtual resource_type_t resource_type()const = 0;
				virtual string resource_sid()const = 0;
				virtual void resource_sid(cstr_t) = 0;
				virtual intfptr fast_cast(resource_type_t) = 0;
				template<resources::resource_type TYPE>
				inline typename resource_type_helper<TYPE>::type_ptr cast() {
					return reinterpret_cast<typename resource_type_helper<TYPE>::type*>(fast_cast(TYPE).get());
				}
			};

			struct nvt LINK ilibrary
				: intf<ilibrary
				, iid("ang::graphics::resources::ilibrary")
				, dom::xml::ixml_streamer>
			{
				virtual core::async::idispatcher_t dispatcher()const = 0;
				virtual bool load_sources(dom::xml::ixml_node_t) = 0;
				virtual optional<ilibrary> load_library(dom::xml::ixml_node_t) = 0;
				virtual optional<iresource> load_resource(dom::xml::ixml_node_t) = 0;
				virtual core::async::iasync_op<ilibrary> load_library_async(dom::xml::ixml_node_t) = 0;
				virtual core::async::iasync_op<iresource> load_resource_async(dom::xml::ixml_node_t) = 0;
				virtual ifactory_t factory()const = 0;
				virtual core::files::ifile_system_t file_system()const = 0;
				virtual string find_source(cstr_t)const = 0;
				virtual iresource_t find_resource(cstr_t)const = 0;
				virtual core::async::iasync<iresource_t> find_resource_async(cstr_t) = 0;
				virtual void clear() = 0;
			};
		}
	}
}

#endif//__ANG_GRAPHICS_RESOURCES_H__