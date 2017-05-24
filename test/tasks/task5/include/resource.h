#pragma once

namespace d3d11
{
	namespace resources
	{
		class resource
			: public object
		{
		protected:
			resource();
			virtual~resource();

		public:
			ANG_DECLARE_INTERFACE();

			virtual bool load(ID3D11Device2*, xml::xml_node_t) = 0;
			//TODO: virtual bool_t load(streams::ibinary_input_stream_t) = 0;
			//TODO: virtual bool_t save(xml::xml_builder_t)const;
			//TODO: virtual bool_t save(streams::ibinary_output_stream_t)const;
		};
	}

}