#include "pch.h"
#include "ang/dom/xml.h"
#include "xml/xml_impl.h"

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif


using namespace ang;
using namespace ang::dom::xml;

static xml_encoding_t  xml_detect_encoding(pointer bom, xml_encoding_t e, windex& idx);

error xml_document::load(core::files::input_text_file_t file)
{
	error er = error_code::success;
	if (file.is_empty() || !file->is_valid())
		return error_code::invalid_param;

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


static xml_encoding_t  xml_detect_encoding(pointer bom, xml_encoding_t e, windex& idx)
{
	if (bom == null)
		return xml_encoding::none;

	switch (e.get())
	{
	case text::encoding::auto_detect:
		if (text::load_bom<text::encoding::utf8>(bom) > 0)
		{
			idx += 3;
			return text::encoding::utf8;
		}
		else if (text::load_bom<text::encoding::utf32_le>(bom) > 0)
		{
			idx += 1;
			return text::native_encoding<text::encoding::utf32_le>();
		}
		else  if (text::load_bom<text::encoding::utf32_be>(bom) > 0)
		{
			idx += 1;
			return text::native_encoding<text::encoding::utf32_be>();
		}
		else if (text::load_bom<text::encoding::utf16_le>(bom) > 0)
		{
			idx += 1;
			return text::native_encoding<text::encoding::utf16_le>();
		}
		else  if (text::load_bom<text::encoding::utf16_be>(bom) > 0)
		{
			idx += 1;
			return text::native_encoding<text::encoding::utf16_be>();
		}

		else if (text::utf8::compare_until((mchar const*)bom, u8"<?xml"_svm.cstr()) == 5)
			return text::encoding::utf8;
		else if (text::utf32::compare_until((char32_t const*)bom, U"<?xml") == 5)
			return text::encoding::utf32;
		else if (text::utf32_se::compare_until((char32_t const*)bom, U"<?xml") == 5)
			return text::encoding::utf32_se;
		else if (text::utf16::compare_until((char16_t const*)bom, u"<?xml") == 5)
			return text::encoding::utf16;
		else if (text::utf16_se::compare_until((char16_t const*)bom, u"<?xml") == 5)
			return text::encoding::utf16_se;

		return text::encoding::utf8;

	case text::encoding::utf16_le: return text::native_encoding<text::encoding::utf16_le>();
	case text::encoding::utf16_be: return text::native_encoding<text::encoding::utf16_be>();
	case text::encoding::utf32_le: return text::native_encoding<text::encoding::utf32_le>();
	case text::encoding::utf32_be: return text::native_encoding<text::encoding::utf32_be>();
	default:
		return e;
	}
}

error xml_document::parse(cstr_t data)
{
	windex idx = 0;
	xml_encoding_t e = xml_detect_encoding((void*)data.ptr(), text::encoding::auto_detect, idx);
	return xml_utils::parse(this, (void*)data.ptr(), data.size(), e, idx);
}

error xml_document::parse(string data)
{
	return parse((cstr_t)data);
}

