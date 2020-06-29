#ifndef __ANG_FRAMEWORK_H__
#elif !defined __ANG_FRAMEWORK_SCENES_H__
#define __ANG_FRAMEWORK_SCENES_H__

namespace ang
{
	namespace graphics
	{
		namespace scenes
		{
			/*class LINK camera 
				: public implement<camera
				, iid("ang::graphics::scenes::camera")
				, icamera
				, inode
				, ixml_streamer>
			{
			public:
				bool load(resources::ilibrary_t, dom::xml::ixml_node_t) override;
				core::async::iasync<bool> load_async(resources::ilibrary_t, dom::xml::ixml_node_t) override;
				
				string sid()const override;
				node_type_t type()const override;
				void draw(idriver_t, iscene_t, iframe_buffer_t, effects::ipass_t) override;
				void update(core::time::step_timer const&) override;

				inode_t parent()const override;
				void parent(inode_t) override;


				itransform_t transform()const override;
				void transform(itransform_t) override;

				bool move_up(bool);
				bool move_down(float deltaTime);
				bool move_forward(float deltaTime);
				bool move_backward(float deltaTime);
				bool move_left(float deltaTime);
				bool move_right(float deltaTime);

				void rotate_up(float deltaTime);
				void rotate_down(float deltaTime);
				void rotate_left(float deltaTime);
				void rotate_right(float deltaTime);

			private:
				collections::ienum_ptr<inode> children()const override;
				collections::ilist_ptr<inode> children() override;
			private:
				virtual~camera();
			};*/

			class LINK scene
				: public implement<scene
				, iid("ang::graphics::scenes::scene")
				, aligned<16>
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
				void draw(idriver_t, iscene_t, iframe_buffer_t, effects::ipass_t) override;
				inode_t parent()const override;
				void parent(inode_t) override;

			public: /*iscene override*/
				string sid()const override;
				node_type_t type()const override;
				collections::ienum_ptr<inode> children()const override;
				collections::ilist_ptr<inode> children() override;
				void draw(idriver_t, iframe_buffer_t, icamera_t = nullptr) override;
				void update(core::time::step_timer const&) override;
				resources::ilibrary_t library()const override;
				size<uint> clip_size()const override;
				icamera_t active_camera()const override;
				maths::float4 ambient_color()const override;
				array<resources::light_info> ligths()const override;
				bool load(resources::ilibrary_t, dom::xml::ixml_node_t) override;
				core::async::iasync<bool> load_async(resources::ilibrary_t, dom::xml::ixml_node_t) override;

			private: /*ilist overrides*/
				wsize counter()const override;
				inode_t at(collections::base_iterator<inode> const&) override;
				bool increase(collections::base_iterator<inode>&)const override;
				bool increase(collections::base_iterator<inode>&, int offset)const override;
				bool decrease(collections::base_iterator<inode>&)const override;
				bool decrease(collections::base_iterator<inode>&, int offset)const override;

				collections::forward_iterator<inode> begin() override;
				collections::forward_iterator<inode> end() override;
				collections::const_forward_iterator<inode> begin()const override;
				collections::const_forward_iterator<inode> end()const override;
				collections::forward_iterator<inode> last() override;
				collections::const_forward_iterator<inode> last()const override;

				collections::backward_iterator<inode> rbegin() override;
				collections::backward_iterator<inode> rend() override;
				collections::const_backward_iterator<inode> rbegin()const override;
				collections::const_backward_iterator<inode> rend()const override;

				collections::ienum_ptr<inode> items()const override;
				void copy(const collections::ienum_ptr<inode>&) override;
				void extend(const collections::ienum_ptr<inode>&) override;
				void push_front(inode_t&&) override;
				void push_front(inode_t const&) override;
				void push_back(inode_t&&) override;
				void push_back(inode_t const&) override;
				collections::iterator<inode> insert(collections::base_iterator<inode> it, inode_t&&) override;
				collections::iterator<inode> insert(collections::base_iterator<inode> it, inode_t const&) override;
				bool pop_front(inode_ptr_t = null) override;
				bool pop_back(inode_ptr_t = null) override;
				collections::iterator<inode> remove(collections::base_iterator<inode> it, inode_ptr_t = null) override;

			protected:
				virtual~scene();
			};
		}
	}
}

#endif/*__ANG_FRAMEWORK_SCENES_H__*/