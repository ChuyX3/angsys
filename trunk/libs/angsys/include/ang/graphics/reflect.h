#ifndef __ANG_GRAPHICS_H__
#elif !defined __ANG_GRAPHICS_REFLECT_H__
#define __ANG_GRAPHICS_REFLECT_H__

//ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ang::graphics::reflect::varying_desc_t, ang::memory::default_allocator)
//ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ang::graphics::reflect::attribute_desc_t, ang::memory::default_allocator)

namespace ang
{

	namespace graphics
	{
		namespace reflect
		{
			typedef collections::vector<varying_desc> uniform_fields_t;		

			struct LINK varying_desc
			{
				template<typename T> static varying_desc_t from_type(string = null);

			private:
				var_type_t m_var_type;
				var_class_t m_var_class;
				astring m_var_name;
				wsize m_array_count;
				wsize m_aligment;
				wsize m_position;
				uniform_fields_t m_fields;


			public:
				varying_desc(
					var_type_t _type = var_type::none,
					var_class_t _class = var_class::none,
					astring name = null,
					wsize _array = 1U,
					wsize aligment = 4U
				);

				varying_desc(astring name, collections::vector<varying_desc> vars, wsize _array = 1U, wsize aligment = 4U);

				varying_desc(varying_desc&& value);
				varying_desc(const varying_desc& value);
				~varying_desc();

				wsize load(dom::xml::ixml_node_t, wsize aligment = -1);
				bool save(dom::xml::ixml_document_t)const;

				var_type_t var_type()const;
				var_class_t var_class()const;
				astring const& var_name()const;
				wsize array_count()const;
				wsize aligment()const;
				wsize position()const;
				//uniform_fields_t& fields();
				uniform_fields_t const& fields()const;

				void var_type(var_type_t);
				void var_class(var_class_t);
				void var_name(astring);
				void array_count(wsize);
				void aligment(wsize);
				void position(wsize);
				void fields(uniform_fields_t, bool calc_pos = true);
				void push_field(varying_desc_t, bool calc_pos = true);

				wsize get_size_in_bytes()const;
				wsize get_size_in_bytes(wsize aligment)const;

				wsize calculate_positions(bool recursive = false);

				varying_desc& operator = (varying_desc&& value);
				varying_desc& operator = (const varying_desc& value);
				bool operator == (const varying_desc& other)const;
				bool operator != (const varying_desc& other)const;
			};

			struct LINK attribute_desc
			{
			private:
				var_type_t m_var_type;
				var_class_t m_var_class;
				astring m_var_name;
				var_semantic_t m_semantic;
				windex m_semantic_index;
				wsize m_position;

			public:
				static wsize calculate_positions(array_view<attribute_desc>&);
				static wsize get_size_in_bytes(array_view<attribute_desc>, wsize aligment = 16, wsize from = 0, wsize to = -1);
				static bool load(dom::xml::ixml_node_t, collections::vector<attribute_desc>&);

				attribute_desc(
					var_type_t _type = var_type::none,
					var_class_t _class = var_class::none,
					astring name = null,
					var_semantic_t semantic = var_semantic::none,
					windex idx = 0U,
					wsize pos = 0U
				);

				attribute_desc(const attribute_desc&);
				attribute_desc(attribute_desc&&) = default;

				bool load(dom::xml::ixml_node_t);
				bool save(dom::xml::ixml_document_t)const;

				var_type_t var_type()const;
				var_class_t var_class()const;
				astring const& var_name()const;
				var_semantic_t semantic()const;
				windex semantic_index()const;
				wsize position()const;

				void var_type(var_type_t);
				void var_class(var_class_t);
				void var_name(astring);
				void semantic(var_semantic_t);
				void semantic_index(windex);
				void position(wsize);

				wsize get_size_in_bytes()const;

				attribute_desc& operator = (const attribute_desc& value);
				attribute_desc& operator = (attribute_desc&& value) = default;

				bool operator == (const attribute_desc& other)const;
				bool operator != (const attribute_desc& other)const;
			};

			template<typename T>
			struct __varying_desc_from_type {
				static inline varying_desc_t desc(string) {
					return varying_desc(); //null descriptor
				}
			};

			template<typename T> 
			inline varying_desc_t type_desc(cstr_t name) {
				return __varying_desc_from_type<T>::desc(name);
			}
			
			template<typename T, wsize N> struct __varying_desc_from_type<T[N]> {
				static inline varying_desc_t desc(string name) {
					varying_desc_t desc = type_desc<T>(name);
					desc.array_count(N);
					return desc;
				}
			};


			class LINK varying
				: public intf<varying
				, iid("ang::graphics::reflect::varying")>
			{
			public:
				template<typename T> class iterator;
				typedef iterator<varying> iterator_t;
				typedef iterator<const varying> const_iterator_t;

				typedef class LINK base_iterator
				{
				protected:
					friend varying;
					varying_t* m_parent;
					windex m_offset;

				public:
					base_iterator();
					base_iterator(base_iterator &&) = default;
					base_iterator(base_iterator const&);
					base_iterator(varying_t*, windex idx);

					varying_t* parent()const;
					windex offset()const;

					base_iterator& operator++();
					base_iterator& operator--();

					base_iterator operator++(int);
					base_iterator operator--(int);

					bool operator == (const base_iterator&)const;
					bool operator != (const base_iterator&)const;
					bool operator >= (const base_iterator&)const;
					bool operator <= (const base_iterator&)const;
					bool operator > (const base_iterator&)const;
					bool operator < (const base_iterator&)const;

				private:
					void parent(varying_t*);
					void offset(windex);
				}base_iterator_t;

			protected:
				array_view<byte> m_raw_data;
				varying_desc m_descriptor;

				template<typename T> struct varying_cast {
					static T cast(varying* var) {
						static_assert(is_type_of(var), "error: ang::graphics::reflect::varying: unsupported type cast");
					}
					static constexpr bool is_type_of(varying*) {
						return false;
					}
				};

			public:
				explicit varying(); //empty
				template<typename T> 
				inline explicit varying(T&, cstr_t = null); //from var
				varying(varying &&);
				varying(varying const&);
				varying(array_view<byte> bytes, varying_desc desc, wsize aligment = (wsize)invalid_index);
				virtual~varying() {}

				virtual rtti_t const& runtime_info()const override;
				virtual bool query_interface(rtti_t const&, unknown_ptr_t) override;

			public:
				wsize aligment()const;
				array_view<byte> raw_data()const;
				varying_desc const& descriptor()const;
				varying field(windex idx);
				varying field(cstr_t idx);
				windex find_field(cstr_t)const;

				varying& operator = (varying &&);
				varying& operator = (varying const&);
				collections::vector<varying> fragment();
				varying operator [](cstr_t);

			public: //iteration
				wsize counter()const;
				varying at(base_iterator_t const&);
				base_iterator_t increase(base_iterator_t&);
				base_iterator_t decrease(base_iterator_t&);
				iterator_t begin();
				iterator_t end();
				const_iterator_t begin()const;
				const_iterator_t end()const;
				varying operator [](windex);	

				template<typename T> inline auto cast() -> decltype(varying_cast<T>::cast(this)) {
					return varying_cast<T>::cast(this);
				}
				template<typename T> inline auto force_cast() -> decltype(varying_cast<T>::cast(this)) {
					return varying_cast<T>::force_cast(this);
				}
				template<typename T> inline auto array_cast() -> decltype(varying_cast<ang::array_view<T>>::cast(this)) {
					return varying_cast<ang::array_view<T>>::force_cast(this);
				}
				template<typename T> inline bool is_type_of()const {
					return varying_cast<T>::is_type_of(const_cast<varying*>(this));
				}

				template<typename T> bool try_copy(T const& value) {
					if (!is_type_of<T>())
						return false;
					force_cast<T>() = value;
					return true;
				}

				template<typename T> bool try_copy(varying const& value) {
					if (!is_type_of<T>() && !value.is_type_of<T>())
						return false;
					cast<T>() = value.cast<T>();
					return true;
				}

				bool operator == (const varying& other)const;
				bool operator != (const varying& other)const;
			};


		
			template<> class LINK varying::iterator<varying_t> : public varying::base_iterator
			{
			public:
				iterator();
				iterator(base_iterator_t &&);
				iterator(base_iterator_t const&);
				iterator(varying_t*, windex idx);
				base_iterator_t& operator =(base_iterator_t const&);
				varying_t operator*()const;
			};

			template<> class LINK varying::iterator<varying_t const> : public varying::base_iterator
			{
			public:
				iterator();
				iterator(base_iterator_t &&);
				iterator(base_iterator_t const&);
				iterator(varying_t*, windex idx);
				base_iterator_t& operator =(base_iterator_t const&);
				const varying_t operator*()const;
			};


			class LINK struct_buffer
				: public implement<struct_buffer
				, iid("ang::graphics::reflect::struct_buffer")
				, varying
				, ibuffer>
			{
			private:
				string m_name;
				collections::vector<long long, memory::aligned16_allocator> m_aligned_data;

			public:
				explicit struct_buffer(); //empty
				struct_buffer(varying_t const&);
				struct_buffer(struct_buffer const*);
				struct_buffer(varying_desc desc, wsize aligment = (wsize)invalid_index);

			private: //overrides	
				void dispose()override;	
				bool is_readonly()const override;
				text::encoding_t encoding()const override;
				pointer buffer_ptr() override;
				const_pointer buffer_ptr()const override;
				wsize buffer_size()const override;
				wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::binary) override;
				ibuffer_view_t map_buffer(windex, wsize) override;
				bool unmap_buffer(ibuffer_view_t&, wsize) override;
				bool can_realloc_buffer()const override;
				bool realloc_buffer(wsize) override;

				operator varying_t&() { return*this; }
				operator varying_t const&()const { return*this; }

			public:
				void clear();
				bool load(dom::xml::ixml_node_t node);
				bool save(dom::xml::ixml_document_t doc);

				bool copy(varying_t const&);
				void push_var(varying_t desc);
				varying_t push_var(varying_desc_t desc);
				bool push_var(varying_desc_t desc, vector<varying>*);
				varying_t make_var(varying_desc_t const& desc, wsize ALIGMENT = 16U);
				varying_t make_struct(wsize ALIGMENT = 16U);
				varying_t make_struct(array_view<varying_desc> const& desc, wsize ALIGMENT = 16U);
				vector<varying> make_array(varying_desc_t const& desc, wsize count, wsize ALIGMENT = 16U);
				vector<varying> make_struct_array(array_view<varying_desc> const& desc, wsize count, wsize ALIGMENT = 16U);

			private:
				virtual~struct_buffer();
			};



			template<var_type TYPE, var_class CLASS>
			struct text_data_value_loader;

			typedef struct text_data_loader_input_context
			{
				//text::iparser_t parser;
				cstr_t data;
				windex idx;
			}text_data_loader_input_context_t;

			typedef struct text_data_loader_output_context
			{
				byte* data;
				windex idx;
			}text_data_loader_output_context_t;

			typedef struct LINK text_data_loader_context
			{
				using load_data_func_t = wsize(*)(
					text_data_loader_context const&,
					text_data_loader_input_context_t& input,
					text_data_loader_output_context_t& output);

				wsize position;
				wsize aligment;
				wsize array_count;
				vector<text_data_loader_context> children;
				load_data_func_t load_data;

				text_data_loader_context(
					wsize position = 0,
					wsize aligment = 4,
					wsize array_count = 1,
					vector<text_data_loader_context> children = null,
					load_data_func_t load_data = null
				);
				text_data_loader_context(text_data_loader_context&&);
				text_data_loader_context(const text_data_loader_context&);

				text_data_loader_context& operator = (text_data_loader_context&&);
				text_data_loader_context& operator = (const text_data_loader_context&);
			}text_data_loader_context_t;

			template<> struct LINK text_data_value_loader<var_type::s8, var_class::scalar> {
				static wsize load_data(
					text_data_loader_context_t const&,
					text_data_loader_input_context_t& input,
					text_data_loader_output_context_t& output);
			};

			template<> struct LINK text_data_value_loader<var_type::u8, var_class::scalar> {
				static wsize load_data(
					text_data_loader_context_t const&,
					text_data_loader_input_context_t& input,
					text_data_loader_output_context_t& output);
			};

			template<> struct LINK text_data_value_loader<var_type::s16, var_class::scalar> {
				static wsize load_data(
					text_data_loader_context_t const&,
					text_data_loader_input_context_t& input,
					text_data_loader_output_context_t& output);
			};

			template<> struct LINK text_data_value_loader<var_type::u16, var_class::scalar> {
				static wsize load_data(
					text_data_loader_context_t const&,
					text_data_loader_input_context_t& input,
					text_data_loader_output_context_t& output);
			};

			template<> struct LINK text_data_value_loader<var_type::s32, var_class::scalar> {
				static wsize load_data(
					text_data_loader_context_t const&,
					text_data_loader_input_context_t& input,
					text_data_loader_output_context_t& output);
			};

			template<> struct LINK text_data_value_loader<var_type::u32, var_class::scalar> {
				static wsize load_data(
					text_data_loader_context_t const&,
					text_data_loader_input_context_t& input,
					text_data_loader_output_context_t& output);
			};

			template<> struct LINK text_data_value_loader<var_type::f32, var_class::scalar> {
				static wsize load_data(
					text_data_loader_context_t const&,
					text_data_loader_input_context_t& input,
					text_data_loader_output_context_t& output);
			};

			template<> struct LINK text_data_value_loader<var_type::f32, var_class::vec2> {
				static wsize load_data(
					text_data_loader_context_t const&,
					text_data_loader_input_context_t& input,
					text_data_loader_output_context_t& output);
			};

			template<> struct LINK text_data_value_loader<var_type::f32, var_class::vec3> {
				static wsize load_data(
					text_data_loader_context_t const&,
					text_data_loader_input_context_t& input,
					text_data_loader_output_context_t& output);
			};

			template<> struct LINK text_data_value_loader<var_type::f32, var_class::vec4> {
				static wsize load_data(
					text_data_loader_context_t const&,
					text_data_loader_input_context_t& input,
					text_data_loader_output_context_t& output);
			};

			template<> struct LINK text_data_value_loader<var_type::f32, var_class::mat4> {
				static wsize load_data(
					text_data_loader_context_t const&,
					text_data_loader_input_context_t& input,
					text_data_loader_output_context_t& output);
			};


			template<> struct LINK text_data_value_loader<var_type::block, var_class::scalar> {
				static wsize load_data(
					text_data_loader_context_t const&,
					text_data_loader_input_context_t& input,
					text_data_loader_output_context_t& output);

				static bool create_context(varying_desc_t const& desc, text_data_loader_context_t&);
			};

			using text_data_loader = text_data_value_loader<var_type::block, var_class::scalar>;
			
		}
	}

	ANG_BEGIN_OBJECT_WRAPPER(LINK, graphics::reflect::struct_buffer)
		graphics::reflect::varying_t operator [](windex idx);
		graphics::reflect::varying_t operator [](cstr_t name);
		graphics::reflect::varying::iterator_t begin();
		graphics::reflect::varying::iterator_t end();
		graphics::reflect::varying::const_iterator_t begin()const;
		graphics::reflect::varying::const_iterator_t end()const;
	ANG_END_OBJECT_WRAPPER();
}

template<typename T> struct ang::graphics::reflect::varying::varying_cast<ang::array_view<T>>
{
	static ang::array_view<T> force_cast(ang::graphics::reflect::varying* var) {
		wsize size = var->m_raw_data.size() / max(sizeof(T), align_down(var->descriptor().aligment(), sizeof(T)));
		return to_array((T*)var->m_raw_data.get(),  min(size, var->descriptor().array_count()));
	}
	static ang::array_view<T> cast(ang::graphics::reflect::varying* var) {
		return to_array((T*)var->m_raw_data.get(), var->descriptor().array_count());
	}
	static bool is_type_of(varying* var) { return varying_cast<T>::is_type_of(var); }
};


template<> struct LINK ang::graphics::reflect::varying::varying_cast<ang::array_view<ang::graphics::reflect::varying>>
{
	static ang::collections::vector<ang::graphics::reflect::varying> force_cast(ang::graphics::reflect::varying* var);
	static ang::collections::vector<ang::graphics::reflect::varying> cast(ang::graphics::reflect::varying* var);
	static bool is_type_of(varying* var);
};

#define ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(_LINK, _TYPE, _RETURNTYPE) \
template<> struct _LINK ang::graphics::reflect::varying::varying_cast<_TYPE> { \
	static _RETURNTYPE cast(ang::graphics::reflect::varying*); \
	static _RETURNTYPE force_cast(ang::graphics::reflect::varying*); \
	static bool is_type_of(ang::graphics::reflect::varying*); \
}; \
template<> _LINK ang::graphics::reflect::varying_desc ang::graphics::reflect::type_desc<_TYPE>(ang::cstr_t);


ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::graphics::reflect::varying, ang::graphics::reflect::varying);

ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, float, float&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::float2, ang::maths::float2&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::float3, ang::maths::float3&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::float4, ang::maths::float4&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::float4x4, ang::maths::float4x4&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::ul_float2, ang::maths::ul_float2&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::ul_float3, ang::maths::ul_float3&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::ul_float4, ang::maths::ul_float4&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::ul_float4x4, ang::maths::ul_float4x4&);

ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, byte, byte&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, char, char&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, short, short&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ushort, ushort&);

ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, int, int&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::int2, ang::maths::int2&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::int3, ang::maths::int3&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::int4, ang::maths::int4&);

ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, uint, uint&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::uint2, ang::maths::uint2&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::uint3, ang::maths::uint3&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::uint4, ang::maths::uint4&);


namespace ang
{
	namespace graphics
	{
		namespace reflect
		{
			template<typename T> inline varying::varying(T& val, cstr_t name)
				:varying(to_array((byte*)&val, size_of<T>()), type_desc<T>(name))
			{
			}
		}
	}
}

#endif//__ANG_GRAPHICS_REFLECT_H__