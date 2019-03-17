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

			struct LINK varying_desc : auto_self<varying_desc>
			{
			private:
				var_type_t m_var_type;
				var_class_t m_var_class;
				astring m_var_name;
				uint m_array_count;
				uint m_aligment;
				uint m_position;
				uniform_fields_t m_fields;

			public:
				varying_desc(
					var_type_t _type = var_type::none,
					var_class_t _class = var_class::none,
					astring name = null,
					uint _array = 1U,
					uint aligment = 4U
				);

				varying_desc(astring name, uniform_fields_t vars, uint _array = 1U, uint aligment = 4U);

				varying_desc(varying_desc&& value);
				varying_desc(const varying_desc& value);
				~varying_desc();

				bool load(dom::xml::xml_node_t, uint aligment = -1);
				bool save(dom::xml::xml_document_t)const;

				var_type_t var_type()const;
				var_class_t var_class()const;
				astring const& var_name()const;
				uint array_count()const;
				uint aligment()const;
				uint position()const;
				//uniform_fields_t& fields();
				uniform_fields_t const& fields()const;

				void var_type(var_type_t);
				void var_class(var_class_t);
				void var_name(astring);
				void array_count(uint);
				void aligment(uint);
				void position(uint);
				void fields(uniform_fields_t, bool calc_pos = true);

				wsize get_size_in_bytes()const;
				wsize get_size_in_bytes(uint aligment)const;

				wsize calculate_positions(bool recursive = false);

				varying_desc& operator = (varying_desc&& value);
				varying_desc& operator = (const varying_desc& value);
				bool operator == (const varying_desc& other)const;
				bool operator != (const varying_desc& other)const;
			};

			struct LINK attribute_desc : auto_self<attribute_desc>
			{
			private:
				var_type_t m_var_type;
				var_class_t m_var_class;
				astring m_var_name;
				var_semantic_t m_semantic;
				index m_semantic_index;
				uint m_position;

			public:
				static wsize calculate_positions(array_view<attribute_desc>&);
				static wsize get_size_in_bytes(array_view<attribute_desc>, wsize aligment = 16, uint from = 0, uint to = -1);
				static bool load(dom::xml::xml_node_t, collections::vector<attribute_desc>&);

				attribute_desc(
					var_type_t _type = var_type::none,
					var_class_t _class = var_class::none,
					astring name = null,
					var_semantic_t semantic = var_semantic::none,
					index idx = 0U,
					uint pos = 0U
				);

				attribute_desc(const attribute_desc&);
				attribute_desc(attribute_desc&&) = default;

				bool load(dom::xml::xml_node_t);
				bool save(dom::xml::xml_document_t)const;

				var_type_t var_type()const;
				var_class_t var_class()const;
				astring const& var_name()const;
				var_semantic_t semantic()const;
				uint semantic_index()const;
				uint position()const;

				void var_type(var_type_t);
				void var_class(var_class_t);
				void var_name(astring);
				void semantic(var_semantic_t);
				void semantic_index(index);
				void position(uint);

				wsize get_size_in_bytes()const;

				attribute_desc& operator = (const attribute_desc& value);
				attribute_desc& operator = (attribute_desc&& value) = default;

				bool operator == (const attribute_desc& other)const;
				bool operator != (const attribute_desc& other)const;
			};


			typedef class LINK varying : auto_self<varying>
			{
			private:
				array_view<byte> _raw_data;
				varying_desc _descriptor;

				template<typename T> struct varying_cast {
					static T cast(varying*) {
						static_assert(is_type_of(), "error: ang::graphics::reflect::varying: unsupported type cast");
					}
					static constexpr bool is_type_of(varying*) {
						return false;
					}
				};

			public:
				explicit varying(); //empty
				varying(varying &&);
				varying(varying const&);
				varying(array_view<byte> bytes, varying_desc desc, wsize aligment = (wsize)invalid_index);

				uint aligment()const;
				array_view<byte> raw_data()const;
				varying_desc const& descriptor()const;
				varying field(index idx);
				varying field(cstr_t idx);
				index find_field(cstr_t)const;

				varying& operator = (varying &&);
				varying& operator = (varying const&);
				collections::vector<varying> fragment();

				varying operator [](index);
				varying operator [](cstr_t);

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
					return varying_cast<T>::is_type_of(this);
				}

				bool operator == (const varying& other)const;
				bool operator != (const varying& other)const;
			}varying_t;

			template<typename T> varying_desc type_desc(cstr_t);

			/*inline wsize get_memory_size_aligned(wsize size, wsize aligment)
			{
				wsize res = (size % aligment);
				if (res == 0u) return size;
				return size + aligment - res;
			}
			inline wsize get_memory_size_aligned_less(wsize size, wsize aligment)
			{
				wsize res = (size % aligment);
				if (res == 0u) return size;
				return size - res;
			}*/

		}
	}
}

template<typename T> struct ang::graphics::reflect::varying::varying_cast<ang::array_view<T>>
{
	static ang::array_view<T> force_cast(ang::graphics::reflect::varying* var) {
		wsize size = var->_raw_data.size() / get_memory_size_aligned_less(sizeof(T), var->descriptor().aligment());
		return ang::array_view<T>((T*)var->_raw_data.get(),  min(size, var->descriptor().array_count()));
	}
	static ang::array_view<T> cast(ang::graphics::reflect::varying* var) {
		return ang::array_view<T>((T*)var->_raw_data.get(), var->descriptor()->array_count);
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
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::unaligned_float2, ang::maths::unaligned_float2&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::unaligned_float3, ang::maths::unaligned_float3&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::unaligned_float4, ang::maths::unaligned_float4&);
ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::maths::unaligned_float4x4, ang::maths::unaligned_float4x4&);

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


#endif//__ANG_GRAPHICS_REFLECT_H__