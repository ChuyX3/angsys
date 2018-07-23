



basic_const_string_buffer<MY_ENCODING>::basic_const_string_buffer()
{

}

basic_const_string_buffer<MY_ENCODING>::~basic_const_string_buffer()
{

}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::strings::basic_const_string_buffer<MY_ENCODING>, ang::strings::basic_const_string_buffer_base);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::strings::basic_const_string_buffer<MY_ENCODING>);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::strings::basic_const_string_buffer<MY_ENCODING>, ang::strings::basic_const_string_buffer_base);


comparision_result_t basic_const_string_buffer<MY_ENCODING>::compare(object const* obj)const
{
	itext_buffer_t buffer = interface_cast<itext_buffer>(const_cast<object*>(obj));
	if (buffer.is_empty())
		return comparision_result::diferent;
	return (comparision_result)iencoder::get_encoder(MY_ENCODING)->compare(text_buffer().ptr(), buffer->text_buffer().ptr(), buffer->encoding());
}

text::encoding_t basic_const_string_buffer<MY_ENCODING>::encoding()const
{
	return MY_ENCODING;
}

const_pointer basic_const_string_buffer<MY_ENCODING>::buffer_ptr()const
{
	return const_str_data_ptr(wsize(this) + align_up<8, sizeof(basic_const_string_buffer<MY_ENCODING>)>())->buffer;
}

wsize basic_const_string_buffer<MY_ENCODING>::buffer_size()const 
{
	return const_str_data_ptr(wsize(this) + align_up<8, sizeof(basic_const_string_buffer<MY_ENCODING>)>())->size;
}

raw_str_t basic_const_string_buffer<MY_ENCODING>::text_buffer()
{
	auto data = const_str_data_ptr(wsize(this) + align_up<8, sizeof(basic_const_string_buffer<MY_ENCODING>)>());
	return raw_str(data->buffer, data->size * sizeof(char_t), MY_ENCODING);
}

raw_cstr_t basic_const_string_buffer<MY_ENCODING>::text_buffer()const
{
	auto data = const_str_data_ptr(wsize(this) + align_up<8, sizeof(basic_const_string_buffer<MY_ENCODING>)>());
	return raw_cstr(data->buffer, data->size * sizeof(char_t), MY_ENCODING);
}




