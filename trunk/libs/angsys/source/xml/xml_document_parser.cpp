/*********************************************************************************************************************/
/*   File Name: xml_document_parser.cpp                                                                              */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) coffe sys, Jesus Angel Rocha Morales                                                              */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/
#include "pch.h"
#include "coffe/dom/xml.h"
#include "xml/xml_impl.h"

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif


using namespace coffe;
using namespace coffe::dom::xml;

error xml_document::load(core::files::input_text_file_t file)
{
	error er = error_code::success;
	file->map([&](ibuffer_view_t data)
	{
		auto e = data->encoding();
		cstr_t code = cstr_t(data->buffer_ptr()
			, data->buffer_size()
			, e == text::encoding::binary ? text::encoding::auto_detect : e);
		try {
			er = parse(code);
			return true;
		}
		catch (exception const& e) {
			er = e; return false;
		}
	});
	return er;
}

void xml_document::save(core::files::output_text_file_t)const
{

}

error xml_document::parse(cstr_t data)
{
	return error_code::success;
}

error xml_document::parse(string data)
{
	return error_code::success;
}

