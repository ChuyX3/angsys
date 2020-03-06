/*********************************************************************************************************************/
/*   File Name: output_text_file.cpp                                                                                 */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/core/files.h"

using namespace ang;
using namespace ang::streams;
using namespace ang::core::files;
 /*
output_text_file::output_text_file()
{

}

output_text_file::output_text_file(cstr_t path, text::encoding_t e)
{
	open(path, e);
}

bool output_text_file::open(cstr_t path, text::encoding_t e)
{
	if (is_valid())
		return false;
	if (e == text::encoding::auto_detect) {
		if (!create(path, open_flags_t(open_flags::access_out) + open_flags::open_alway + open_flags::format_text))
			return false;
	}
	else {
		switch (e)
		{
		case text::encoding::ascii:
			if (!create(path, open_flags_t(open_flags::access_out) + open_flags::open_alway + open_flags::format_ascii))
				return false;
			break;
		}

	}

	return true;
}

stream_mode_t output_text_file::mode()const
{

}

text::encoding_t output_text_file::format()const
{

}

file_offset_t output_text_file::cursor()const
{

}

file_size_t output_text_file::size()const
{

}

bool output_text_file::cursor(file_offset_t size, stream_reference_t ref)
{

}

bool output_text_file::command(streams::special_command_t)
{

}

wsize output_text_file::write(cstr_t)
{

}

wsize output_text_file::write_line(cstr_t)
{

}

wsize output_text_file::write_format(cstr_t, var_args_t)
{

}
*/
