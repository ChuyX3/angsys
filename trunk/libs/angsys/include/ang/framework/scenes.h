#ifndef __ANG_FRAMEWORK_H__
#elif !defined __ANG_FRAMEWORK_SCENES_H__
#define __ANG_FRAMEWORK_SCENES_H__

namespace ang
{
	namespace graphics
	{
		namespace scenes
		{
			/*struct nvt LINK iscene
				: intf<icamera
				, iid("ang::graphics::scenes::iscene")
				, inode>
			{
				virtual void draw(idriver_t, iframe_buffer_t, icamera_t = nullptr) = 0;
				virtual void update(core::time::step_timer const&) = 0;
				virtual resources::ilibrary_t library()const = 0;
				virtual size<uint> clip_size()const = 0;
				virtual icamera_t active_camera()const = 0;
				virtual maths::float4 ambient_color()const = 0;
				virtual array<resources::light_info> ligths()const = 0;
			};*/

			class LINK scene
				: public implement<scene
				, iid("ang::graphics::scenes::scene")
				, framework::aligned
				, iscene
				, inode
				, ixml_streamer
				, collections::ilist<inode>
				, collections::ienum<inode>>
			{
			private:
				string m_sid;
				resources::library_t m_library;
				vector<inode_t> m_children;
				effects::ieffect_t m_effect;

			public:
				scene();

			private: /*iscene override*/
				virtual void draw(idriver_t, iscene_t, iframe_buffer_t, effects::ipass_t) override;
				virtual inode_t parent()const override;
				virtual void parent(inode_t) override;

			public: /*iscene override*/
				virtual string sid()const override;
				virtual node_type_t type()const override;
				virtual collections::ienum_ptr<inode> children()const override;
				virtual collections::ilist_ptr<inode> children() override;
				virtual void draw(idriver_t, iframe_buffer_t, icamera_t = nullptr) override;
				virtual void update(core::time::step_timer const&) override;
				virtual resources::ilibrary_t library()const override;
				virtual size<uint> clip_size()const override;
				virtual icamera_t active_camera()const override;
				virtual maths::float4 ambient_color()const override;
				virtual array<resources::light_info> ligths()const override;	
				virtual bool load(resources::ilibrary_t, dom::xml::ixml_node_t) override;
				virtual core::async::iasync<bool> load_async(resources::ilibrary_t, dom::xml::ixml_node_t) override;			

			private: /*ilist overrides*/
				virtual wsize counter()const override;
				virtual inode_t at(collections::base_iterator<inode> const&) override;
				virtual bool increase(collections::base_iterator<inode>&)const override;
				virtual bool increase(collections::base_iterator<inode>&, int offset)const override;
				virtual bool decrease(collections::base_iterator<inode>&)const override;
				virtual bool decrease(collections::base_iterator<inode>&, int offset)const override;

				virtual collections::forward_iterator<inode> begin() override;
				virtual collections::forward_iterator<inode> end() override;
				virtual collections::const_forward_iterator<inode> begin()const override;
				virtual collections::const_forward_iterator<inode> end()const override;
				virtual collections::forward_iterator<inode> last() override;
				virtual collections::const_forward_iterator<inode> last()const override;

				virtual collections::backward_iterator<inode> rbegin() override;
				virtual collections::backward_iterator<inode> rend() override;
				virtual collections::const_backward_iterator<inode> rbegin()const override;
				virtual collections::const_backward_iterator<inode> rend()const override;

				virtual collections::ienum_ptr<inode> items()const override;
				virtual void copy(const collections::ienum_ptr<inode>&) override;
				virtual void extend(const collections::ienum_ptr<inode>&) override;
				virtual void push_front(inode_t &&) override;
				virtual void push_front(inode_t const&) override;
				virtual void push_back(inode_t &&) override;
				virtual void push_back(inode_t const&) override;
				virtual collections::iterator<inode> insert(collections::base_iterator<inode> it, inode_t &&) override;
				virtual collections::iterator<inode> insert(collections::base_iterator<inode> it, inode_t const&) override;
				virtual bool pop_front(inode_ptr_t = null) override;
				virtual bool pop_back(inode_ptr_t = null) override;
				virtual collections::iterator<inode> remove(collections::base_iterator<inode> it, inode_ptr_t = null) override;

			protected:
				virtual~scene();
			};
		}
	}
}

#endif/*__ANG_FRAMEWORK_SCENES_H__*/