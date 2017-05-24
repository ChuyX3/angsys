/*********************************************************************************************************************/
/*   File Name: pack_file.cpp                                                                                        */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/core/files.h"

using namespace ang;
using namespace ang::core;
using namespace ang::core::files;

class packed_file_impl :
	public object,
	public ifile
{
private:
	wstring _path;
	ifile_t _original_source;
	file_flags_t _flags;
	ulong64 _size;
	ulong64 _offset;
	ulong64 _cursor;

public:
	packed_file_impl();

private:
	virtual~packed_file_impl();

public: //Overrides
	ANG_DECLARE_INTERFACE();

	bool create(cwstr_t path, open_flags_t flags, ifile_t original_source);
	bool is_created()const;
	bool close();

public: //Methos
	virtual streams::stream_mode_t mode()const override;
	virtual wstring file_path()const override;
	virtual file_size_t file_size()const override;
	virtual text::encoding_t encoding()const override;
	virtual void cursor(file_reference_t, file_cursor_t) override;
	virtual file_size_t cursor()const override;
	virtual wsize read(wsize size, pointer buffer)override;
	virtual wsize write(wsize size, pointer buffer)override;
	virtual wsize read(ibuffer_t buffer)override;
	virtual wsize write(ibuffer_t buffer)override;
	virtual ibuffer_t map(wsize size, file_size_t offset)override;
	virtual bool unmap(ibuffer_t)override;

private:
	static ulong64 get_file_size(pointer h);
	static text::encoding_t get_file_encoding(pointer h);
	static void set_file_encoding(pointer h, text::encoding_t);

};