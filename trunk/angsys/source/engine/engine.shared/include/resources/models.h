#pragma once

namespace ang
{
	namespace graphics
	{
		namespace resources
		{
			ang_object(model_loader);

			class model_loader
				: public smart<model_loader, imodel_loader>
			{
				weak_ptr<core::files::ifile_system_t> m_file_system;
				weak_ptr<core::async::idispatcher> m_async_worker;
				vector<model_element> m_elements;

			public:
				model_loader(core::files::ifile_system* fs);

				ANG_DECLARE_INTERFACE();

			protected: //overrides
				void dispose()override;

			public: //overrides
				void clear()override;
				bool load(dom::xml::xml_node_t)override;
				bool save(dom::xml::xml_document_t)const override;
				bool load(core::files::input_text_file_t)override;
				bool save(core::files::output_text_file_t)const override;
				bool load(core::files::input_binary_file_t)override;
				bool save(core::files::output_binary_file_t)const override;
				core::async::iasync<imodel_loader_t> load_async(dom::xml::xml_node_t)override;
				core::async::iasync<imodel_loader_t> save_async(dom::xml::xml_document_t)const override;
				core::async::iasync<imodel_loader_t> load_async(core::files::input_text_file_t)override;
				core::async::iasync<imodel_loader_t> save_async(core::files::output_text_file_t)const override;
				core::async::iasync<imodel_loader_t> load_async(core::files::input_binary_file_t)override;
				core::async::iasync<imodel_loader_t> save_async(core::files::output_binary_file_t)const override;
				collections::ienum_ptr<model_element> elements()const override;

			private:
				bool load_material(core::files::input_text_file_t, model_element& out);

				bool load_element(dom::xml::xml_node_t, model_element& out);
				bool load_material(dom::xml::xml_node_t, model_element& out);
				reflect::struct_buffer_t load_buffer(dom::xml::xml_node_t);

			private:
				virtual~model_loader();
			};
		}
	}
}