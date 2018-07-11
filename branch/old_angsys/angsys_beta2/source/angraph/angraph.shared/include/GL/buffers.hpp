#pragma once

#include <ang/graphics/angraph.hpp>

#if OPENGL_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace gl
		{
			class gl_index_buffer;
			class gl_vertex_buffer;

			typedef object_wrapper<gl_index_buffer> gl_index_buffer_t;
			typedef object_wrapper<gl_vertex_buffer> gl_vertex_buffer_t;


			class gl_index_buffer
				: public object
				, public buffers::iindex_buffer
			{
			private:
				wsize _index_count;
				buffers::buffer_usage_t _usage;
				reflect::var_type_t _index_type;
				uint _index_buffer;

			public:
				gl_index_buffer();

			public: //overrides
				ANG_DECLARE_INTERFACE();

				buffers::buffer_type_t buffer_type()const override;
				buffers::buffer_usage_t buffer_usage()const override;
				buffers::buffer_bind_flag_t buffer_bind_flag()const override;
				ibuffer_t map(idriver_t, bool writeMode) override;
				bool unmap(idriver_t, ibuffer_t) override;
				reflect::var_type_t data_type()const override;
				wsize counter()const override;
				wsize size_in_bytes()const override;

			public: //internal
				bool create(
					gl_driver_t driver, 
					buffers::buffer_usage_t usage, 
					reflect::var_type_t type, 
					wsize count, 
					array_view<byte> init_data);
				bool close();
				void use_buffer(gl_driver_t);
				inline uint buffer_object()const { return _index_buffer; }

			private:
				virtual~gl_index_buffer();
			};


			class gl_vertex_buffer
				: public object
				, public buffers::ivertex_buffer
			{
			private:
				uint _vertex_count;
				uint _stride;
				buffers::buffer_usage_t _usage;
				collections::vector <reflect::attribute_desc> _vertex_desc;
				uint _vertex_buffer;

			public:
				gl_vertex_buffer();

			public: //overrides
				ANG_DECLARE_INTERFACE();

				buffers::buffer_type_t buffer_type()const override;
				buffers::buffer_usage_t buffer_usage()const override;
				buffers::buffer_bind_flag_t buffer_bind_flag()const override;
				ibuffer_t map(idriver_t, bool writeMode) override;
				bool unmap(idriver_t, ibuffer_t) override;
				array_view<reflect::attribute_desc> descriptor()const override;
				wsize block_counter()const override;
				wsize size_in_bytes()const override;

			public: //internal
				bool create(
					gl_driver_t driver,
					buffers::buffer_usage_t usage,
					array_view<reflect::attribute_desc> vertex_desc,
					wsize vertex_count,
					array_view<byte> init_data);
				bool close();
				void use_buffer(gl_driver_t);
				inline uint buffer_object()const { return _vertex_buffer; }

			private:
				virtual~gl_vertex_buffer();
			};

		}
	}
}

#endif
