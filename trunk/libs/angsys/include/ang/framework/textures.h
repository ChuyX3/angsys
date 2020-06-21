#ifndef __ANG_FRAMEWORK_H__
#elif !defined __ANG_FRAMEWORK_TEXTURES_H__
#define __ANG_FRAMEWORK_TEXTURES_H__

namespace ang
{
	namespace graphics
	{
		namespace textures
		{
			using namespace resources;

			typedef struct tex_info {
				string sid;
				vector<string> files;
				tex_type_t type;
			}tex_info_t;


			class LINK texture_loader
				: public implement<texture_loader
				, iid("ang::graphics::textures::texture_loader")
				, itexture_loader>
			{
				weak_ptr<ilibrary> m_parent;
				mutable core::async::mutex_t m_mutex;
				core::async::idispatcher_t m_async_worker;
				collections::hash_map<string, tex_info> m_tex_info_map;
				collections::hash_map<string, weak_ptr<iresource>> m_texture_map;

			public:
				texture_loader(ilibrary* parent);

			private: //overrides
				void dispose()override;

			public: //overrides
				void clear()override;
				ifactory_t factory()const override;
				core::files::ifile_system_t file_system()const;
				core::async::idispatcher_t dispatcher()const override;
				bool load(dom::xml::ixml_node_t)override;
				bool save(dom::xml::ixml_document_t)const override;
				core::async::iasync<bool> load_async(dom::xml::ixml_node_t)override;
				core::async::iasync<bool> save_async(dom::xml::ixml_document_t)const override;

				bool load_sources(dom::xml::ixml_node_t) override;
				optional<ilibrary> load_library(dom::xml::ixml_node_t) override;
				optional<iresource> load_resource(dom::xml::ixml_node_t) override;
				core::async::iasync_op<ilibrary> load_library_async(dom::xml::ixml_node_t) override;
				core::async::iasync_op<iresource> load_resource_async(dom::xml::ixml_node_t) override;

				string find_source(cstr_t)const override;
				iresource_t find_resource(cstr_t)const override;
				core::async::iasync<iresource_t> find_resource_async(cstr_t) override;

				optional<itexture> load_texture(cstr_t sid) override;
				bool load_texture_info(dom::xml::ixml_node_t) override;
				optional<itexture> load_texture(dom::xml::ixml_node_t) override;
				core::async::iasync_op<itexture> load_texture_async(cstr_t sid) override;
				core::async::iasync<bool> load_texture_info_async(dom::xml::ixml_node_t) override;
				core::async::iasync_op<itexture> load_texture_async(dom::xml::ixml_node_t) override;
				itexture_t find_texture(cstr_t)const override;

			public: //internal
				inline ilibrary_t parent()const {
					return m_parent.lock();
				}

			private:
				template<typename T>
				inline core::async::iasync<T> create_task(function<T(core::async::iasync<T>)> func)const {
					return dispatcher()->run_async<T>(ang::move(func));
				}

			private:
				virtual~texture_loader();
			};
		}
	}
}

#endif/*__ANG_FRAMEWORK_TEXTURES_H__*/