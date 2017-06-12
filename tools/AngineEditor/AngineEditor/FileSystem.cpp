#include "stdafx.h"
#include "ProjectManager.h"

using namespace AngineEditor;
using namespace AngineEditor::filesystem;


file::file(cwstr_t full)
	: _name(invalid_index)
	, _ext(invalid_index)
	, _full_path(full)
{

	full = _full_path;

	index idx = full.find("/", 0);
	while (idx != invalid_index)
	{
		_full_path[idx] = '\\';
		idx = full.find("/", idx);
	}

	idx = full.find_rev('.', full.size() - 1);
	if (idx == invalid_index) //has no extension
	{
		idx = full.find_rev('\\', full.size() - 1);
		if (idx == invalid_index)
		{
			_name = 0;
		}
		else if (idx + 1 >= full.size())
			_name = invalid_index;
		else
			_name = idx + 1;
	}
	else
	{
		if (idx + 1 >= full.size())
			_ext = invalid_index;
		else
			_ext = idx + 1;

		idx = full.find_rev('\\', idx + 1);
		if (idx == invalid_index)
			_name = 0;
		else
			_name = idx + 1;
	}

}

file::file(cwstr_t path, cwstr_t name)
	: _name(invalid_index)
	, _ext(invalid_index)
	, _full_path(path)
{
	path = _full_path;
	index idx = path.find("/", 0);
	while (idx != invalid_index)
	{
		_full_path[idx] = '\\';
		idx = path.find("/", idx);
	}

	if (*_full_path->end() != '\\')
		_full_path << "\\"_s;
	_name = _full_path->length();
	_full_path << name;

	idx = name.find_rev('.', name.size() - 1);
	if (idx != invalid_index) //has no extension
	{
		if (idx + 1 >= name.size())
			_ext = invalid_index;
		else
			_ext = _name + idx + 1;
	}
}

file::file(cwstr_t path, cwstr_t name, cwstr_t ext)
{
	path = _full_path;
	index idx = path.find("/", 0);
	while (idx != invalid_index)
	{
		_full_path[idx] = '\\';
		idx = path.find("/", idx);
	}
	if (*_full_path->end() != '\\')
		_full_path << "\\"_s;
	_name = _full_path->length();
	_full_path << name << "."_s;
	_ext = _full_path->length();
	_full_path << ext;
}

wstring file::full_path()const
{
	return _full_path;
}

wstring file::path()const
{
	wstring out;
	if (_name == 0)
		return ".\\"_s;
	if (_name == invalid_index)
		return _full_path;
	else
		_full_path->sub_string(out, _full_path->begin(), _full_path->at(_name));
	return move(out);
}

wstring file::name()const
{
	if (_name == invalid_index)
		return null;
	wstring out;
	if (_ext == invalid_index)
		_full_path->sub_string(out, _full_path->at(_name), _full_path->end());
	else
		_full_path->sub_string(out, _full_path->at(_name), _full_path->at(_ext));	
	return move(out);
}

wstring file::ext()const
{
	wstring out;
	if (_ext == invalid_index)
		return null;
	else
		_full_path->sub_string(out, _full_path->at(_ext), _full_path->end());
	return move(out);
}

void file::path(cwstr_t)
{

}

void file::name(cwstr_t)
{

}

void file::ext(cwstr_t)
{

}


//////////////////////////////////////////////////////////////////////


folder::folder()
{

}

folder::~folder()
{

}

bool folder::load(xml::xml_node_t node)
{

}

bool folder::load(xml::xml_node_t, cwstr_t root)
{

}

bool folder::load(xml::xml_node_t, cwstr_t root, folder_t)
{

}

bool folder::save(xml::xml_builder_t)const
{

}

wstring folder::path()const
{

}

wstring folder::full_path()const
{

}

static_array<file_t> folder::files()const { return _files; }
static_array<folder_t> folder::folders()const { return _folders; }