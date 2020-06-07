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
				, ixml_streamer
				, collections::ienum<inode_t>>
			{
				virtual node_type_t type()const = 0;
				virtual void draw(idriver_t, iscene_t, iframe_buffer_t, effects::ipass_t) = 0;
				virtual void update(core::time::step_timer const&) = 0;
				virtual inode_t parent()const = 0;
				virtual void parent(inode_t) = 0;
				virtual collections::iterator<inode_t> find(function<bool(inode_t const&)> func, bool invert = false)const = 0;
				virtual collections::iterator<inode_t> find(function<bool(inode_t const&)> func, collections::base_iterator<inode_t> next_to, bool invert = false)const = 0;
				virtual collections::ienum_ptr<inode_t> find_all(function<bool(inode_t const&)> func)const = 0;
				virtual void push(inode_t const& val, bool last = true) = 0;
				virtual bool insert(collections::base_iterator<inode_t> it, inode_t const& val) = 0;
				virtual bool pop(bool last = true) = 0;
				virtual bool pop(inode_t& val, bool last = true) = 0;
				virtual bool pop_at(collections::base_iterator<inode_t> it) = 0;
				virtual bool pop_at(collections::base_iterator<inode_t> it, inode_t& val) = 0;
			};

			struct nvt LINK icamera
				: intf<icamera
				, iid("ang::graphics::scenes::icamera")
				, inode>
			{
				virtual itransform_t transform()const = 0;
				virtual void transform(itransform_t) = 0;
			};

			struct nvt LINK iscene
				: intf<icamera
				, iid("ang::graphics::scenes::iscene")
				, inode>
			{
				virtual void draw(idriver_t, iframe_buffer_t, icamera_t = nullptr)= 0;
				virtual void update(core::time::step_timer const&)= 0;
				virtual resources::ilibrary_t library()const = 0;
				virtual size<uint> clip_size()const = 0;
				virtual icamera_t active_camera()const = 0;
				virtual maths::float4 ambient_color()const = 0;
				virtual array_view<resources::light_info> ligths()const = 0;
			};

			template<typename T> 
			class node_impl
				: public T
			{
			public:
				typedef T											nonde_type;
				typedef inode_t										type;
				typedef node_impl<T>								self_t;	
				typedef collections::ienum<type>					ienum_t;
				typedef collections::base_iterator<type>			base_iterator_t;
				typedef collections::iterator<type>					iterator_t;
				typedef collections::const_iterator<type>			const_iterator_t;
				typedef collections::forward_iterator<type>			forward_iterator_t;
				typedef collections::const_forward_iterator<type>	const_forward_iterator_t;
				typedef collections::backward_iterator<type>		backward_iterator_t;
				typedef collections::const_backward_iterator<type>	const_backward_iterator_t;

			private:
				mutable weak_ptr<inode> m_parent;
				collections::list<inode_t> m_children;

			protected:
				node_impl()
					: m_children(new collections::list_object<inode_t>()) {
				}
				virtual void dispose() { 
					m_children = null; 
				}

			public: //ienum overrides
				inline wsize counter()const override { return m_children->counter(); }

				inline type& at(base_iterator_t const& it) override { return m_children->at(it); }
				inline bool increase(base_iterator_t& it)const override { return m_children->increase(it); }
				inline bool increase(base_iterator_t& it, int offset)const override { return m_children->increase(it, offset); }
				inline bool decrease(base_iterator_t& it)const override { return m_children->decrease(it); }
				inline bool decrease(base_iterator_t& it, int offset)const override { return m_children->decrease(it, offset); }

				inline forward_iterator_t begin() override { return m_children->begin(); }
				inline forward_iterator_t end() override { return m_children->end(); }
				inline const_forward_iterator_t begin()const override { return m_children->begin(); }
				inline const_forward_iterator_t end()const override { return m_children->end(); }
				inline forward_iterator_t last() override { return m_children->last(); }
				inline const_forward_iterator_t last()const override { return m_children->last(); }

				inline backward_iterator_t rbegin() override { return m_children->rbegin(); }
				inline backward_iterator_t rend() override { return m_children->rend(); }
				inline const_backward_iterator_t rbegin()const override { return m_children->rbegin(); }
				inline const_backward_iterator_t rend()const override { return m_children->rend(); }

			public: //overrides
				inline inode_t parent()const override { return m_parent.lock(); }
				inline void parent(inode_t node) override { m_parent = node; }

				inline iterator_t find(function<bool(type const&)> func, bool invert = false)const override { return m_children->find(func, invert); }
				inline iterator_t find(function<bool(type const&)> func, base_iterator_t next_to, bool invert = false)const override { return m_children->find(func,  next_to, invert); }
				inline collections::ienum_ptr<type> find_all(function<bool(type const&)> func)const override { return m_children->find_all(func); }
				inline void push(type const& node, bool last = true) override {
					if (!node.is_empty()) {
						m_children->push(node, last);
						node->parent(this);
					}
				}
				inline bool insert(base_iterator_t it, type const& node) override {
					if(!m_children->insert(it, node))
						return false;
					node->parent(this);
					return true;
				}
				inline bool pop(bool last = true) override {
					type node;
					if (!m_children->pop(node, last))
						return false;
					node->parent(null);
					return true;
				}
				inline bool pop(type& node, bool last = true) override { 
					if (!m_children->pop(node, last))
						return false;
					node->parent(null);
					return true;
				}
				inline bool pop_at(base_iterator_t it) override { 
					type node;
					if (!m_children->pop_at(it, node))
						return false;
					node->parent(null);
					return true;
				}
				inline bool pop_at(base_iterator_t it, type& node) override {
					if (!m_children->pop_at(it, node))
						return false;
					node->parent(null);
					return true;
				}
			};

		}
	}
}

#endif//__ANG_GRAPHICS_SCENES_H__