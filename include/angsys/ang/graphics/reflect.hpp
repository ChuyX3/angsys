#ifndef __ANGRAPH_HPP__
#elif !defined __ANGRAPH_REFLECT_HPP__
#define __ANGRAPH_REFLECT_HPP__

//ANG_DECLARE_VECTOR_DATA_SPECIALIZATION(LINK, ang::graphics::reflect::variable_desc)
//ANG_DECLARE_VECTOR_DATA_SPECIALIZATION(LINK, ang::graphics::reflect::attribute_desc)

namespace ang
{
	namespace graphics
	{
		namespace reflect
		{
			typedef collections::vector<variable_desc> uniform_fields_t;

			struct LINK variable_desc 
			{
			private:
				var_type_t _var_type;
				var_class_t _var_class;
				string _var_name;
				uint _array_count;
				uint _aligment;
				uint _position;
				uniform_fields_t _fields;

			public:
				variable_desc(
					var_type_t _type = var_type::none,
					var_class_t _class = var_class::none,
					string name = null,
					uint _array = 1U,
					uint aligment = 4U
				);

				variable_desc(string name, uniform_fields_t vars, uint _array = 1U, uint aligment = 4U);

				variable_desc(variable_desc&& value);
				variable_desc(const variable_desc& value);
				~variable_desc();

				bool load(xml::ixml_node_t, uint aligment = -1);
				bool save(xml::xml_document_t)const;

				var_type_t var_type()const;
				var_class_t var_class()const;
				string const& var_name()const;
				uint array_count()const;
				uint aligment()const;
				uint position()const;
				//uniform_fields_t& fields();
				uniform_fields_t const& fields()const;

				void var_type(var_type_t);
				void var_class(var_class_t);
				void var_name(string);
				void array_count(uint);
				void aligment(uint);
				void position(uint);
				void fields(uniform_fields_t);

				wsize get_size_in_bytes()const;
				wsize get_size_in_bytes(uint aligment)const;

				wsize calculate_positions(bool recursive = false);

				variable_desc& operator = (variable_desc&& value);
				variable_desc& operator = (const variable_desc& value);
				bool operator == (const variable_desc& other)const;
				bool operator != (const variable_desc& other)const;
			};

			struct LINK attribute_desc
			{
			private:
				var_type_t _var_type;
				var_class_t _var_class;
				string _var_name;
				var_semantic_t _semantic;
				index _semantic_index;
				uint _position;

			public:
				static wsize calculate_positions(array_view<attribute_desc>&);
				static wsize get_size_in_bytes(array_view<attribute_desc>, wsize aligment = 16, uint from = 0, uint to = -1);
				static bool load(xml::ixml_node_t, collections::vector<attribute_desc>&);

				attribute_desc(
					var_type_t _type = var_type::none,
					var_class_t _class = var_class::none,
					string name = null,
					var_semantic_t semantic = var_semantic::none,
					index idx = 0U,
					uint pos = 0U
				);

				attribute_desc(const attribute_desc&);
				attribute_desc(attribute_desc&&) = default;

				bool load(xml::ixml_node_t);
				bool save(xml::xml_document_t)const;

				var_type_t var_type()const;
				var_class_t var_class()const;
				string const& var_name()const;
				var_semantic_t semantic()const;
				uint semantic_index()const;
				uint position()const;

				void var_type(var_type_t);
				void var_class(var_class_t);
				void var_name(string);
				void semantic(var_semantic_t);
				void semantic_index(index);
				void position(uint);

				wsize get_size_in_bytes()const;

				attribute_desc& operator = (const attribute_desc& value);
				attribute_desc& operator = (attribute_desc&& value) = default;

				bool operator == (const attribute_desc& other)const;
				bool operator != (const attribute_desc& other)const;
			};

			class LINK variable
			{
			private:
				array_view<byte> _raw_data;
				variable_desc _descriptor;

				template<typename T> struct variable_cast {
					static T cast(variable*) {
						static_assert(is_type_of(), "error: ang::graphics::reflect::variable: unsupported type cast");
					}
					static constexpr bool is_type_of(variable*) {
						return false;
					}
				};

			public:
				explicit variable(); //empty
				variable(variable &&);
				variable(variable const&);
				variable(array_view<byte> bytes, variable_desc desc, uint aligment = invalid_index);

				uint aligment()const;
				array_view<byte> raw_data()const;
				variable_desc const& descriptor()const;
				variable field(index idx);
				variable field(cstr_t idx);
				variable field(cwstr_t idx);
				index find_field(cstr_t)const;
				index find_field(cwstr_t)const;

				variable& operator = (variable &&);
				variable& operator = (variable const&);
				collections::vector<variable> fragment();

				variable operator [](index);
				variable operator [](cstr_t);
				variable operator [](cwstr_t);

				template<typename T> inline auto cast() -> decltype(variable_cast<T>::cast(this)) {
					return variable_cast<T>::cast(this);
				}
				template<typename T> inline auto force_cast() -> decltype(variable_cast<T>::cast(this)) {
					return variable_cast<T>::force_cast(this);
				}
				template<typename T> inline auto array_cast() -> decltype(variable_cast<ang::array_view<T>>::cast(this)) {
					return variable_cast<ang::array_view<T>>::force_cast(this);
				}
				template<typename T> inline bool is_type_of()const {
					return variable_cast<T>::is_type_of(this);
				}

				bool operator == (const variable& other)const;
				bool operator != (const variable& other)const;
			};

			template<typename T> variable_desc type_desc(cstr_t);

			inline wsize get_memory_size_aligned(wsize size, uint aligment)
			{
				wsize res = (size % aligment);
				if (res == 0u) return size;
				return size + aligment - res;
			}
			inline wsize get_memory_size_aligned_less(wsize size, uint aligment)
			{
				wsize res = (size % aligment);
				if (res == 0u) return size;
				return size - res;
			}

		}
	}
}

template<typename T> struct ang::graphics::reflect::variable::variable_cast<ang::array_view<T>>
{
	static ang::array_view<T> force_cast(ang::graphics::reflect::variable* var) {
		wsize size = var->_raw_data.size() / get_memory_size_aligned_less(sizeof(T), var->descriptor().aligment());
		return ang::array_view<T>((T*)var->_raw_data.get(),  min(size, var->descriptor().array_count()));
	}
	static ang::array_view<T> cast(ang::graphics::reflect::variable* var) {
		return ang::array_view<T>((T*)var->_raw_data.get(), var->descriptor()->array_count);
	}
	static bool is_type_of(variable* var) { return variable_cast<T>::is_type_of(var); }
};


template<> struct LINK ang::graphics::reflect::variable::variable_cast<ang::array_view<ang::graphics::reflect::variable>>
{
	static ang::collections::vector<ang::graphics::reflect::variable> force_cast(ang::graphics::reflect::variable* var);
	static ang::collections::vector<ang::graphics::reflect::variable> cast(ang::graphics::reflect::variable* var);
	static bool is_type_of(variable* var);
};

#define ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(_LINK, _TYPE, _RETURNTYPE) \
template<> struct _LINK ang::graphics::reflect::variable::variable_cast<_TYPE> { \
	static _RETURNTYPE cast(ang::graphics::reflect::variable*); \
	static _RETURNTYPE force_cast(ang::graphics::reflect::variable*); \
	static bool is_type_of(ang::graphics::reflect::variable*); \
}; \
template<> _LINK ang::graphics::reflect::variable_desc ang::graphics::reflect::type_desc<_TYPE>(cstr_t);


ANG_GRAPHICS_REFLECT_DECLARE_TEMPLATE_VARIABLE_CAST(LINK, ang::graphics::reflect::variable, ang::graphics::reflect::variable);

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

ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::reflect::variable)
ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::reflect::variable_desc)
ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::reflect::attribute_desc)

template<> inline ang::graphics::reflect::var_type_t ang::xml::xml_text::xml_as<ang::graphics::reflect::var_type_t>()const { return ang::graphics::reflect::var_type_t::parse(xml_as<cwstr_t>()); }
template<> inline ang::graphics::reflect::var_class_t ang::xml::xml_text::xml_as<ang::graphics::reflect::var_class_t>()const { return ang::graphics::reflect::var_class_t::parse(xml_as<cwstr_t>()); }
template<> inline ang::graphics::reflect::var_semantic_t ang::xml::xml_text::xml_as<ang::graphics::reflect::var_semantic_t>()const { return ang::graphics::reflect::var_semantic_t::parse(xml_as<cwstr_t>()); }

#endif//__ANGRAPH_REFLECT_HPP__