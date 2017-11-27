/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_STREAMS_HPP__
#error ...
#elif !defined __ANG_STREAMS_INL__
#define __ANG_STREAMS_INL__

template<ang::text::encoding_enum ENCODING>
inline ang::type_name_t ang::streams::itext_input_stream<ENCODING>::class_name() {
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::streams::itext_input_stream<"_o += ang::text::encoding_t(ENCODING).to_string()) += ">"_s));
	return name;
}

template<ang::text::encoding_enum ENCODING>
inline bool ang::streams::itext_input_stream<ENCODING>::is_inherited_of(ang::type_name_t name) {
	return name == class_name() ||
		iinput_stream::is_inherited_of(name);
}

template<ang::text::encoding_enum ENCODING>
inline ang::type_name_t ang::streams::itext_input_stream<ENCODING>::object_name()const {
	return class_name();
}

template<ang::text::encoding_enum ENCODING>
inline bool ang::streams::itext_input_stream<ENCODING>::is_kind_of(ang::type_name_t name)const {
	return name == object_name() ||
		iinput_stream::is_kind_of(name);
}

template<ang::text::encoding_enum ENCODING>
inline bool ang::streams::itext_input_stream<ENCODING>::query_object(ang::type_name_t name, ang::unknown_ptr_t out) {
	if (out == null) return false;
	if (name == class_name())
	{
		*out = static_cast<itext_input_stream<ENCODING>*>(this);
		return true;
	}
	else if (iinput_stream::query_object(name, out))
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////

template<ang::text::encoding_enum ENCODING>
inline ang::type_name_t ang::streams::itext_output_stream<ENCODING>::class_name() {
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::streams::itext_output_stream<"_o += ang::text::encoding_t(ENCODING).to_string()) += ">"_s));
	return name;
}

template<ang::text::encoding_enum ENCODING>
inline bool ang::streams::itext_output_stream<ENCODING>::is_inherited_of(ang::type_name_t name) {
	return name == class_name() ||
		ioutput_stream::is_inherited_of(name);
}

template<ang::text::encoding_enum ENCODING>
inline ang::type_name_t ang::streams::itext_output_stream<ENCODING>::object_name()const {
	return class_name();
}

template<ang::text::encoding_enum ENCODING>
inline bool ang::streams::itext_output_stream<ENCODING>::is_kind_of(ang::type_name_t name)const {
	return name == object_name() ||
		ioutput_stream::is_kind_of(name);
}

template<ang::text::encoding_enum ENCODING>
inline bool ang::streams::itext_output_stream<ENCODING>::query_object(ang::type_name_t name, ang::unknown_ptr_t out) {
	if (out == null) return false;
	if (name == class_name())
	{
		*out = static_cast<itext_output_stream<ENCODING>*>(this);
		return true;
	}
	else if (ioutput_stream::query_object(name, out))
	{
		return true;
	}
	return false;
}


#endif//__ANG_STREAMS_INL__
