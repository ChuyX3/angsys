#include "pch.h"
#include "d3d11/driver.hpp"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

#if DIRECTX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;


d3d11_texture::d3d11_texture()
{
}

d3d11_texture::~d3d11_texture()
{
}


ANG_IMPLEMENT_CLASSNAME(ang::graphics::d3d11::d3d11_texture);
ANG_IMPLEMENT_OBJECTNAME(ang::graphics::d3d11::d3d11_texture);

bool d3d11_texture::is_inherited_of(type_name_t name)
{
	return name == type_of<d3d11_texture>()
		|| object::is_inherited_of(name)
		|| textures::itexture::is_inherited_of(name);
}

bool d3d11_texture::is_kind_of(type_name_t name)const
{
	return name == type_of<d3d11_texture>()
		|| object::is_kind_of(name)
		|| textures::itexture::is_kind_of(name);
}

bool d3d11_texture::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<d3d11_texture>())
	{
		*out = static_cast<d3d11_texture*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (textures::itexture::query_object(name, out))
	{
		return true;
	}
	return false;
}

bool d3d11_texture::attach(com_wrapper<ID3D11ShaderResourceView> view)
{
	_texture_name = nullptr;
	_tex_format = DXGI_FORMAT_UNKNOWN;
	_tex_type = textures::tex_type::null;
	_tex_dimentions = { 0,0 };
	_tex_dimentions_depth = 0;
	d3d_raw_resource = nullptr;
	d3d_shader_view = view;
	if (!view.is_empty())
	{
		D3D11_RESOURCE_DIMENSION dim;
		view->GetResource(&d3d_raw_resource);
		d3d_raw_resource->GetType(&dim);
		switch (dim)
		{
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D: {
				_tex_type = textures::tex_type::tex1D;
				D3D11_TEXTURE1D_DESC tex_desc;
				static_cast<ID3D11Texture1D*>(d3d_raw_resource.get())->GetDesc(&tex_desc);
				_tex_format = tex_desc.Format;
				_tex_dimentions = { float(tex_desc.Width), float(1) };
				_tex_dimentions_depth = 1;
			}
			break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D: {
				_tex_type = textures::tex_type::tex2D;
				D3D11_TEXTURE2D_DESC tex_desc;
				static_cast<ID3D11Texture2D*>(d3d_raw_resource.get())->GetDesc(&tex_desc);
				_tex_format = tex_desc.Format;
				_tex_dimentions = { float(tex_desc.Width), float(tex_desc.Height) };
				_tex_dimentions_depth = 1;
			}
			break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D: {
				_tex_type = textures::tex_type::tex3D;
				D3D11_TEXTURE3D_DESC tex_desc;
				static_cast<ID3D11Texture3D*>(d3d_raw_resource.get())->GetDesc(&tex_desc);
				_tex_format = tex_desc.Format;
				_tex_dimentions = { float(tex_desc.Width), float(tex_desc.Height) };
				_tex_dimentions_depth = tex_desc.Depth;
			}
			break;
		default:
			break;
		}
	}

	return true;
}

bool d3d11_texture::load(d3d11_texture_loader_t loader, xml::ixml_node_t node)
{
	if (node.is_empty())
		return false;

	_texture_name = (cwstr_t)node->xml_attributes()["name"_s];

	if (node->xml_has_children())
	{
		bool res = true;
		d3d11_driver_t driver = loader->driver();
		auto att = node->xml_attributes();
		_tex_type = att["type"_s]->xml_as<textures::tex_type_t>();
		_tex_dimentions_depth = 1;
		collections::vector<core::files::input_binary_file_t> files;

		for (xml::ixml_node_t tex_node : node->xml_children())
		{
			if ((cwstr_t)tex_node->xml_name() != "texture"_s)
				continue;
			wstring filename = tex_node->xml_value();
			auto _filename = loader->find_file(filename);
			core::files::input_binary_file_t file;
			if (_filename.is_empty())  loader->get_file_system()->open(filename, file); // file = new core::files::input_binary_file(filename);
			else loader->get_file_system()->open(_filename, file); //file = new core::files::input_binary_file(_filename);
			if (!file->is_valid())
				continue;
			files += file;
		}

		tex_file_info_t info;

		if (files.is_empty() || !d3d11_texture_loader::load_texture(
			driver
			, files
			, info
			, &d3d_raw_resource
			, &d3d_shader_view
			, _tex_type.get() == textures::tex_type::texCube))
			return false;
		
		_tex_dimentions.width = (float)info.width;
		_tex_dimentions.height = (float)info.height;
		_tex_format = (DXGI_FORMAT)info.format;

		return true;
	}
	else
	{
		bool res = true;
		d3d11_driver_t driver = loader->driver();
		auto att = node->xml_attributes();
		_tex_type = att["type"_s]->xml_as<textures::tex_type_t>();

		wstring filename = node->xml_value();

		auto _filename = loader->find_file(filename);
		core::files::input_binary_file_t file;
		if (_filename.is_empty())loader->get_file_system()->open(filename, file); // file = new core::files::input_binary_file(filename);
		else loader->get_file_system()->open(_filename, file); //file = new core::files::input_binary_file(_filename);

		tex_file_info_t info;

		if (!file->is_valid()|| !d3d11_texture_loader::load_texture(
			driver
			, file
			, info
			, &d3d_raw_resource
			, &d3d_shader_view
			, _tex_type.get() == textures::tex_type::texCube))
			return false;
		
		_tex_dimentions.width = (float)info.width;
		_tex_dimentions.height = (float)info.height;
		_tex_format = (DXGI_FORMAT)info.format;

		return true;
	}
}

bool d3d11_texture::use_texture(d3d11_driver_t driver, index idx)const
{
	driver->D3D11Context()->PSSetShaderResources(idx, 1, (ID3D11ShaderResourceView**)&d3d_shader_view);
	return true;
}


string d3d11_texture::resource_name()const { return _texture_name; }
textures::tex_type_t d3d11_texture::tex_type()const { return _tex_type; }

textures::tex_format_t d3d11_texture::tex_format()const { return traslate_format_dx(_tex_format); }
graphics::size<float> d3d11_texture::tex_dimentions()const { return _tex_dimentions; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


d3d11_texture_loader::d3d11_texture_loader(d3d11_driver_t parent)
	: main_mutex()
	, _driver(parent)
{
	
	_work_thead = core::async::thread::create_dispatcher();
	_work_thead->resume();
}

d3d11_texture_loader::~d3d11_texture_loader()
{
	_work_thead->stop();
	_work_thead->worker_thread()->wait(core::async::async_action_status::completed, -1);
	_work_thead = null;
	main_mutex->lock();
	_textures.clean();
	_texture_info_map.clean();
	_source_map.clean();
	main_mutex->unlock();
}

ANG_IMPLEMENT_CLASSNAME(ang::graphics::d3d11::d3d11_texture_loader);
ANG_IMPLEMENT_OBJECTNAME(ang::graphics::d3d11::d3d11_texture_loader);

bool d3d11_texture_loader::is_inherited_of(type_name_t name)
{
	return name == type_of<d3d11_texture_loader>()
		|| object::is_inherited_of(name)
		|| textures::itexture_loader::is_inherited_of(name);
}

bool d3d11_texture_loader::is_kind_of(type_name_t name)const
{
	return name == type_of<d3d11_texture_loader>()
		|| object::is_kind_of(name)
		|| textures::itexture_loader::is_kind_of(name);
}

bool d3d11_texture_loader::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<d3d11_texture_loader>())
	{
		*out = static_cast<d3d11_texture_loader*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (textures::itexture_loader::query_object(name, out))
	{
		return true;
	}
	return false;
}

void d3d11_texture_loader::set_file_system(core::files::ifile_system_t fs) { _fs = fs; }

bool d3d11_texture_loader::load_library(xml::ixml_node_t library)
{
	if (library.is_empty() || !library->xml_has_children())
		return false;

	for(xml::ixml_node_t node : library->xml_children())
	{
		auto name = node->xml_name()->xml_as<cwstr_t>();
		if (name == "sources"_s)
			load_sources(node);
		else if (name == "texture"_s) {
			main_mutex->lock();
			_texture_info_map += {node->xml_attributes()["name"_s]->xml_as<cwstr_t>(), node };
			main_mutex->unlock();
		}
	}
	return true;
}

core::async::iasync_t<textures::itexture_loader_t> d3d11_texture_loader::load_library_async(xml::ixml_node_t library)
{
	return _work_thead->run_async<textures::itexture_loader_t>([=](core::async::iasync<textures::itexture_loader_t>* async)->textures::itexture_loader*
	{
		if (library.is_empty() || !library->xml_has_children())
		{
			async->cancel();
			return null;
		}

		for(xml::ixml_node_t node : library->xml_children())
		{
			auto name = node->xml_name()->xml_as<cwstr_t>();
			if (name == "sources"_s)
				load_sources(node);
			else if (name == "texture"_s) {
				main_mutex->lock();
				_texture_info_map += {node->xml_attributes()["name"_s]->xml_as<cwstr_t>(), node };
				main_mutex->unlock();
			}
		}
		return this;
	});
}

bool d3d11_texture_loader::load_sources(xml::ixml_node_t sources)
{
	if (!sources->xml_has_children())
		return false;

	for(xml::ixml_node_t file : sources->xml_children())
	{
		auto name = file->xml_name()->xml_as<cwstr_t>();
		if (name != "file"_s)
			continue;
		main_mutex->lock();
		_source_map += {file->xml_attributes()["sid"_s]->xml_as<cwstr_t>(), file->xml_value()->xml_as<cwstr_t>()};
		main_mutex->unlock();
	}

	return true;
}

textures::itexture_t d3d11_texture_loader::load_texture(xml::ixml_node_t node)
{
	if (node.is_empty())
		return null;
	d3d11_texture_t tex = new d3d11_texture();
	if (!tex->load(this, node))
		return null;
	main_mutex->lock();
	_textures += {node->xml_attributes()["name"_s]->xml_as<cwstr_t>(), tex.get()};
	main_mutex->unlock();
	return tex.get();
}

textures::itexture_t d3d11_texture_loader::load_texture(string name)
{
	{	core::async::scope_locker<core::async::mutex_ptr_t> lock = main_mutex;
		if (!_textures.is_empty())
		{
			auto it = _textures->find(name);
			if (it.is_valid())
			{
				d3d11_texture_t tex = it->value.lock();
				if (!tex.is_empty())
					return tex.get();
			}
		}
	}
	xml::ixml_node_t node;
	{	core::async::scope_locker<core::async::mutex_ptr_t> lock = main_mutex;
		if (_texture_info_map.is_empty())
			return null;
		auto it = _texture_info_map->find(name);
		if (!it.is_valid())
			return null;
		node = it->value;
	}

	if (node.is_empty())
		return null;
	return load_texture(node);
}


core::async::iasync_t<textures::itexture_t>  d3d11_texture_loader::load_texture_async(xml::ixml_node_t node)
{
	return _work_thead->run_async<textures::itexture_t>([=](core::async::iasync<textures::itexture_t>* async)->textures::itexture_t
	{
		if (node.is_empty())
			return null;
		d3d11_texture_t tex = new d3d11_texture();
		if (!tex->load(this, node))
			return null;
		main_mutex->lock();
		_textures += {node->xml_attributes()["name"_s]->xml_as<cwstr_t>(), tex.get()};
		main_mutex->unlock();
		return tex.get();
	});
}

core::async::iasync_t<textures::itexture_t>  d3d11_texture_loader::load_texture_async(string name)
{
	//return core::async::async_task<textures::itexture_t>::run_async([=](core::async::iasync<textures::itexture_t>* async, var_args_t args)->textures::itexture_t
	return _work_thead->run_async<textures::itexture_t>([=](core::async::iasync<textures::itexture_t>* async)->textures::itexture_t
	{
		{	core::async::scope_locker<core::async::mutex_ptr_t> lock = main_mutex;
			if (!_textures.is_empty())
			{
				auto it = _textures->find(name);
				if (it.is_valid())
				{
					d3d11_texture_t tex = it->value.lock();
					if (!tex.is_empty())
						return tex.get();
				}
			}
		}
		xml::ixml_node_t node;
		{	core::async::scope_locker<core::async::mutex_ptr_t> lock = main_mutex;
			if (_texture_info_map.is_empty())
				return null;
			auto it = _texture_info_map->find(name);
			if (!it.is_valid())
				return null;
			node = it->value;
		}

		if (node.is_empty())
			return null;
		return load_texture(node);
	});
}

textures::itexture_t d3d11_texture_loader::find_texture(cstr_t name)const
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = main_mutex;
	if (_textures.is_empty())
		return null;
	auto it = _textures->find(name);
	return it.is_valid() ? it->value.lock().get() : null; //return it.is_valid() ? it->value().lock<d3d11_texture>().get() : null;
}

textures::itexture_t d3d11_texture_loader::find_texture(cwstr_t name)const
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = main_mutex;
	if (_textures.is_empty())
		return null;
	auto it = _textures->find(name);
	return it.is_valid() ? it->value.lock().get() : null; //return it.is_valid() ? it->value().lock<d3d11_texture>().get() : null;
}

#endif
