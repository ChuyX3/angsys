#include "pch.h"
#include "file.h"

using namespace ang;
using namespace ang::core;

file::file()
	: handle(INVALID_HANDLE_VALUE)
{
}

file::~file()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::file);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::file, ang::object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::file, ang::object);


bool file::is_eof()const
{
	return size() == cursor();
}

void file::clear() {
	if (is_valid())
		CloseHandle(handle);
	handle = INVALID_HANDLE_VALUE;
}

bool file::is_valid()const {
	return handle != INVALID_HANDLE_VALUE;
}

long64 file::cursor()const
{
	if (!is_valid())
		return 0;
	LARGE_INTEGER cur;
	LARGE_INTEGER pos = { 0 };
	SetFilePointerEx(handle, pos, &cur, FILE_CURRENT);
	return cur.QuadPart;
}

void file::cursor(long64 pos)
{
	if (!is_valid())
		return;
	LARGE_INTEGER li;
	SetFilePointerEx(handle, reinterpret_cast<LARGE_INTEGER&>(pos), &li, FILE_BEGIN);
}

long64 file::size()const
{
	if (!is_valid())
		return 0;
	LARGE_INTEGER li;
	return GetFileSizeEx(handle, &li) == FALSE ? 0ll : li.QuadPart;
}

void file::size(long64 sz)
{
	if (!is_valid())
		return;
	auto cur = cursor();
	cursor(sz);
	SetEndOfFile(handle);
}

wsize file::read(pointer buff, wsize sz)
{
	if (!is_valid())
		return 0;
	dword readed = 0;
	ReadFile(handle, buff, (dword)sz, &readed, null);
	return readed;
}

wsize file::write(pointer buff, wsize sz)
{
	if (!is_valid())
		return 0;
	dword written = 0;
	WriteFile(handle, buff, (dword)sz, &written, null);
	return written;
}

//////////////////////////////////////////////////////////////////////////////

text::encoding_t get_file_encoding(pointer handle)
{
	dword bom[2] = { 0,0 };
	dword offset = 0;
	text::encoding_t result = text::encoding::ascii;
#ifdef WINDOWS_PLATFORM
	dword out;
	LARGE_INTEGER lint;
	lint.QuadPart = 0;
	SetFilePointerEx(handle, lint, 0, FILE_BEGIN);
	ReadFile(handle, bom, 4, &out, NULL);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	lseek(handle, 0, SEEK_SET);
	::read(handle, bom, 4);
#endif

	if (text::load_bom<text::encoding::utf8>(bom) > 0)
	{
		offset = 3;
		result = text::encoding::utf8;
	}
	else if (text::load_bom<text::encoding::utf32_be>(bom) > 0)
	{
		offset = 4;
		result = text::encoding::utf32_be;
	}
	else if (text::load_bom<text::encoding::utf32_le>(bom) > 0)
	{
		offset = 4;
		result = text::encoding::utf32_le;
	}
	else if (text::load_bom<text::encoding::utf16_be>(bom) > 0)
	{
		offset = 2;
		result = text::encoding::utf16_be;
	}
	else if (text::load_bom<text::encoding::utf16_le>(bom) > 0)
	{
		offset = 2;
		result = text::encoding::utf16_le;
	}
#ifdef WINDOWS_PLATFORM
	lint.QuadPart = offset;
	SetFilePointerEx(handle, lint, 0, FILE_BEGIN);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	::lseek(handle, offset, SEEK_SET);
#endif
	return result;
}


text_input_file::text_input_file()
	: file()
	, e(text::encoding::auto_detect)
{
}

text_input_file::~text_input_file()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::text_input_file);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::text_input_file, ang::core::file);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::text_input_file, ang::core::file);


bool text_input_file::open(wstring name)
{
	handle = CreateFile2((cwstr_t)name, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, NULL);
	if (!is_valid())
		return false;
	e = get_file_encoding(handle);
	return true;
}

void text_input_file::size(long64) { 
	throw_exception(except_code::invalid_access); 
}

void text_input_file::seek(text::raw_cstr_t str)
{
	wsize i = 0, j = 0, readed = 0, temp;
	byte buffer[204];
	text::raw_str raw;
	text::iencoder_t e1 = text::iencoder::get_encoder(e);
	text::iencoder_t e2 = text::iencoder::get_encoder(str.encoding());
	wsize cs1 = e1->char_type().size();
	wsize cs2 = e2->char_type().size();

	while (j < str.size() / cs2)
	{
		temp = file::read(buffer, 200);
		buffer[temp] = 0;
		buffer[temp + 1] = 0; //set end of string
		buffer[temp + 2] = 0; //set end of string
		buffer[temp + 3] = 0; //set end of string
		i = 0;
		while (temp > i * cs1)
		{
			char32 c1 = e1->to_char32(buffer, i);
			char32 c2 = e2->to_char32((pointer)str.ptr(), j);

			if ((c1 == U' ' || c1 == U'\t') && (c2 == U' ' || c2 == U'\t')) {
				do
					c1 = e1->to_char32(buffer, i);
				while ((c1 == U' ' || c1 == U'\t') && c1 != 0);

				do
					c2 = e1->to_char32((pointer)str.ptr(), j);
				while ((c2 == U' ' || c2 == U'\t') && c2 != 0);
			}
		}

	}

	

}

