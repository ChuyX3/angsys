#ifndef __ANG_GRAPHICS_H__
#elif !defined __ANG_GRAPHICS_SCENES_H__
#define __ANG_GRAPHICS_SCENES_H__

namespace ang
{
	namespace graphics
	{
		namespace scenes
		{
			struct nvt LINK ixml_streamer
				: intf<ixml_streamer
				, iid("ang::graphics::scenes::ixml_streamer")>
			{
				virtual bool load(resources::ilibrary_t, dom::xml::ixml_node_t)= 0;
				virtual core::async::iasync<bool> load_async(resources::ilibrary_t, dom::xml::ixml_node_t)= 0;
			};

			struct nvt LINK itransform
				: intf<itransform
				, iid("ang::graphics::scenes::itransform")>
			{
				/*Gets "is stereo"*/
				virtual bool is_stereo()const = 0;
				/*Sets the Current Viewport*/
				virtual void viewport(box<float>) = 0;
				/*Gets the Current Viewport*/
				virtual box<float> viewport()const = 0;
				/*Gets the View Matrix*/
				virtual maths::float4x4 view_matrix(windex = 0)const = 0;
				/*Gets the Projection Matrix*/
				virtual maths::float4x4 projection_matrix(windex = 0)const = 0;
			};

			struct nvt LINK inode
				: intf<inode
				, iid("ang::graphics::scenes::inode")
				, ixml_streamer>
			{
				virtual string sid()const = 0;
				virtual node_type_t type()const = 0;
				virtual void draw(idriver_t, iscene_t, iframe_buffer_t, effects::ipass_t) = 0;
				virtual void update(core::time::step_timer const&) = 0;
				virtual inode_t parent()const = 0;
				virtual void parent(inode_t) = 0;

				virtual collections::ienum_ptr<inode> children()const = 0;
				virtual collections::ilist_ptr<inode> children() = 0;
			};

			struct nvt LINK icamera
				: intf<icamera
				, iid("ang::graphics::scenes::icamera")
				, ixml_streamer>
			{
				virtual void update(core::time::step_timer const&) = 0;
				virtual itransform_t transform()const = 0;
				virtual void transform(itransform_t) = 0;
			};

			struct nvt LINK iscene
				: intf<iscene
				, iid("ang::graphics::scenes::iscene")
				, inode>
			{
				virtual void draw(idriver_t, iframe_buffer_t, icamera_t = nullptr)= 0;
				virtual resources::ilibrary_t library()const = 0;
				virtual size<uint> clip_size()const = 0;
				virtual icamera_t active_camera()const = 0;
				virtual maths::float4 ambient_color()const = 0;
				virtual array<resources::light_info> ligths()const = 0;
			};

		}
	}
}

#endif//__ANG_GRAPHICS_SCENES_H__