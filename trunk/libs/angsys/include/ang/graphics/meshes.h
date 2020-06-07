#ifndef __ANG_GRAPHICS_H__
#elif !defined __ANG_GRAPHICS_MESHES_H__
#define __ANG_GRAPHICS_MESHES_H__

namespace ang
{
	namespace graphics
	{
		namespace meshes
		{
			struct nvt LINK imaterial
				: intf<imaterial
				, iid("ang::graphics::meshes::imaterial")>
			{
				virtual string technique()const = 0;
				virtual array_view<reflect::varying_desc> fields_layout()const = 0;
				virtual reflect::varying fields() = 0; //only one per time
				virtual reflect::varying field(windex) = 0; //only one per time
				virtual reflect::varying field(cstr_t) = 0; //only one per time
				virtual array_view<textures::itexture_t> textures() const = 0;
			};

			struct nvt LINK igeometry
				: intf<igeometry
				, iid("ang::graphics::meshes::igeometry")>
			{
				virtual string technique() const = 0;
				virtual buffers::iindex_buffer_t index_buffer() const = 0;
				virtual buffers::ivertex_buffer_t vertex_buffer() const = 0;
				virtual void transform(maths::mat4 const&) = 0;
				virtual maths::mat4 const& transform() const = 0;
				virtual imaterial_t material() const = 0;
			};

			struct nvt LINK ianimation
				: intf<ianimation
				, iid("ang::graphics::meshes::ianimation")>
			{
				virtual resources::iresource_t resource()const = 0;
				virtual bool is_running()const = 0;
				virtual bool cyclic()const = 0;
				virtual void cyclic(bool) = 0;
				virtual double duration()const = 0;
				virtual double cursor()const = 0;
				virtual void cursor(double) = 0;
				virtual void start() = 0;
				virtual void update(double delta) = 0;
				virtual void stop() = 0;
			};

			struct nvt LINK imesh
				: intf<imesh
				, iid("ang::graphics::meshes::imesh")>
			{
				virtual resources::iresource_t resource()const = 0;;
				virtual array_view<igeometry_t> elements()const = 0;
			};

			struct nvt LINK imesh_loader
				: intf<imesh_loader
				, iid("ang::graphics::meshes::imesh_loader")
				, resources::ilibrary>
			{
				virtual optional<imesh> load_mesh(dom::xml::ixml_node_t) = 0;;
				virtual core::async::iasync_op<imesh> load_mesh_async(dom::xml::ixml_node_t) = 0;;
				virtual imesh_t find_mesh(cstr_t)const = 0;;
			};
		}
	}
}

#endif//__ANG_GRAPHICS_MESHES_H__