#include "pch.h"
#include "d3d11/driver.h"

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

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::d3d11::d3d11_texture);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::d3d11::d3d11_texture, object, textures::itexture);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::d3d11::d3d11_texture, object, textures::itexture);

bool d3d11_texture::attach(com_wrapper<ID3D11ShaderResourceView> view)
{
	m_texture_name = nullptr;
	m_tex_format = DXGI_FORMAT_UNKNOWN;
	m_tex_type = textures::tex_type::null;
	m_tex_dimentions = { 0,0 };
	m_tex_dimentions_depth = 0;
	m_d3d_raw_resource = nullptr;
	m_d3d_shader_view = view;
	if (!view.is_empty())
	{
		D3D11_RESOURCE_DIMENSION dim;
		view->GetResource(&m_d3d_raw_resource);
		m_d3d_raw_resource->GetType(&dim);
		switch (dim)
		{
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D: {
				m_tex_type = textures::tex_type::tex1D;
				D3D11_TEXTURE1D_DESC tex_desc;
				static_cast<ID3D11Texture1D*>(m_d3d_raw_resource.get())->GetDesc(&tex_desc);
				m_tex_format = tex_desc.Format;
				m_tex_dimentions = { float(tex_desc.Width), float(1) };
				m_tex_dimentions_depth = 1;
			}
			break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D: {
				m_tex_type = textures::tex_type::tex2D;
				D3D11_TEXTURE2D_DESC tex_desc;
				static_cast<ID3D11Texture2D*>(m_d3d_raw_resource.get())->GetDesc(&tex_desc);
				m_tex_format = tex_desc.Format;
				m_tex_dimentions = { float(tex_desc.Width), float(tex_desc.Height) };
				m_tex_dimentions_depth = 1;
			}
			break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D: {
				m_tex_type = textures::tex_type::tex3D;
				D3D11_TEXTURE3D_DESC tex_desc;
				static_cast<ID3D11Texture3D*>(m_d3d_raw_resource.get())->GetDesc(&tex_desc);
				m_tex_format = tex_desc.Format;
				m_tex_dimentions = { float(tex_desc.Width), float(tex_desc.Height) };
				m_tex_dimentions_depth = tex_desc.Depth;
			}
			break;
		default:
			break;
		}
	}

	return true;
}

bool d3d11_texture::load(d3d11_texture_loader_t loader, dom::xml::xml_node_t node)
{
	if (node.is_empty())
		return false;

	m_texture_name = (text::raw_cstr)node->xml_attributes()["name"_s];

	if (node->xml_has_children())
	{
		bool res = true;
		d3d11_driver_t driver = loader->driver();
		auto att = node->xml_attributes();
		m_tex_type = att["type"_s]->xml_as<textures::tex_type_t>();
		m_tex_dimentions_depth = 1;
		collections::vector<core::files::input_binary_file_t> files;

		for (dom::xml::xml_node_t tex_node : node->xml_children())
		{
			if ((text::raw_cstr)tex_node->xml_name() != "texture"_s)
				continue;
			wstring filename = tex_node->xml_value();
			auto _filename = loader->find_file((text::raw_cstr)filename);
			core::files::input_binary_file_t file;
			if (_filename.is_empty())  loader->get_file_system()->open(filename, &file); // file = new core::files::input_binary_file(filename);
			else loader->get_file_system()->open(_filename, &file); //file = new core::files::input_binary_file(_filename);
			if (!file->is_valid())
				continue;
			files += file;
		}

		tex_file_info_t info;

		if (files.is_empty() || !d3d11_texture_loader::load_texture(
			driver
			, files
			, info
			, &m_d3d_raw_resource
			, &m_d3d_shader_view
			, m_tex_type.get() == textures::tex_type::texCube))
			return false;
		
		m_tex_dimentions.width = (float)info.width;
		m_tex_dimentions.height = (float)info.height;
		m_tex_format = (DXGI_FORMAT)info.format;

		return true;
	}
	else
	{
		bool res = true;
		d3d11_driver_t driver = loader->driver();
		auto att = node->xml_attributes();
		m_tex_type = att["type"_s]->xml_as<textures::tex_type_t>();

		wstring filename = node->xml_value();

		auto _filename = loader->find_file((text::raw_cstr)filename);
		core::files::input_binary_file_t file;
		if (_filename.is_empty())loader->get_file_system()->open(filename, &file); // file = new core::files::input_binary_file(filename);
		else loader->get_file_system()->open(_filename, &file); //file = new core::files::input_binary_file(_filename);

		tex_file_info_t info;

		if (file.is_empty() || !file->is_valid()|| !d3d11_texture_loader::load_texture(
			driver
			, file
			, info
			, &m_d3d_raw_resource
			, &m_d3d_shader_view
			, m_tex_type.get() == textures::tex_type::texCube))
			return false;
		
		m_tex_dimentions.width = (float)info.width;
		m_tex_dimentions.height = (float)info.height;
		m_tex_format = (DXGI_FORMAT)info.format;

		return true;
	}
}

bool d3d11_texture::use_texture(d3d11_driver_t driver, index idx)const
{
	driver->D3D11Context()->PSSetShaderResources(idx, 1, (ID3D11ShaderResourceView**)&m_d3d_shader_view);
	return true;
}


text::istring_view_t d3d11_texture::resource_name()const { return m_texture_name.get(); }
textures::tex_type_t d3d11_texture::tex_type()const { return m_tex_type; }

textures::tex_format_t d3d11_texture::tex_format()const { return traslate_format_dx(m_tex_format); }
graphics::size<float> d3d11_texture::tex_dimentions()const { return m_tex_dimentions; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


d3d11_texture_loader::d3d11_texture_loader(d3d11_driver_t parent)
	: m_mutex(make_shared<core::async::mutex_t>())
	, m_driver(ang::move(parent))
{
	
	m_work_thead = core::async::thread::create_thread();
}

d3d11_texture_loader::~d3d11_texture_loader()
{
	m_work_thead->exit();
	m_mutex->lock();
	m_textures.reset();
	m_texture_info_map.reset();
	m_source_map.reset();
	m_mutex->unlock();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::d3d11::d3d11_texture_loader);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::d3d11::d3d11_texture_loader, object, textures::itexture_loader);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::d3d11::d3d11_texture_loader, object, textures::itexture_loader);

void d3d11_texture_loader::set_file_system(core::files::ifile_system_t fs) { m_fs = fs; }

bool d3d11_texture_loader::load_library(dom::xml::xml_node_t library)
{
	if (library.is_empty() || !library->xml_has_children())
		return false;

	for(dom::xml::xml_node_t node : library->xml_children())
	{
		auto name = node->xml_name()->xml_as<text::raw_cstr>();
		if (name == "sources"_s)
			load_sources(node);
		else if (name == "texture"_s) {
			m_mutex->lock();
			m_texture_info_map += {node->xml_attributes()["name"_s]->xml_as<text::raw_cstr>(), node };
			m_mutex->unlock();
		}
	}
	return true;
}

core::async::iasync<textures::itexture_loader_t> d3d11_texture_loader::load_library_async(dom::xml::xml_node_t library)
{
	return m_work_thead->run_async<textures::itexture_loader_t>([=](core::async::iasync<textures::itexture_loader_t> async)->textures::itexture_loader_t
	{
		if (library.is_empty() || !library->xml_has_children())
		{
			async->cancel();
			return null;
		}

		for(dom::xml::xml_node_t node : library->xml_children())
		{
			auto name = node->xml_name()->xml_as<text::raw_cstr>();
			if (name == "sources"_s)
				load_sources(node);
			else if (name == "texture"_s) {
				m_mutex->lock();
				m_texture_info_map += {node->xml_attributes()["name"_s]->xml_as<text::raw_cstr>(), node };
				m_mutex->unlock();
			}
		}
		return this;
	});
}

bool d3d11_texture_loader::load_sources(dom::xml::xml_node_t sources)
{
	if (!sources->xml_has_children())
		return false;

	for(dom::xml::xml_node_t file : sources->xml_children())
	{
		auto name = file->xml_name()->xml_as<text::raw_cstr>();
		if (name != "file"_s)
			continue;
		m_mutex->lock();
		m_source_map += {file->xml_attributes()["sid"_s]->xml_as<text::raw_cstr>(), file->xml_value()->xml_as<text::raw_cstr>()};
		m_mutex->unlock();
	}

	return true;
}

textures::itexture_t d3d11_texture_loader::load_texture(dom::xml::xml_node_t node)
{
	if (node.is_empty())
		return null;
	d3d11_texture_t tex = new d3d11_texture();
	if (!tex->load(this, node))
		return null;
	m_mutex->lock();
	m_textures += {node->xml_attributes()["name"_s]->xml_as<text::raw_cstr>(), tex.get()};
	m_mutex->unlock();
	return tex.get();
}

textures::itexture_t d3d11_texture_loader::load_texture(text::raw_cstr_t name)
{
	{	core::async::scope_locker<core::async::mutex_ptr_t> lock = m_mutex;
		if (!m_textures.is_empty())
		{
			auto it = m_textures->find(name);
			if (it.is_valid())
			{
				d3d11_texture_t tex = it->value.lock();
				if (!tex.is_empty())
					return tex.get();
			}
		}
	}
	dom::xml::xml_node_t node;
	{	core::async::scope_locker<core::async::mutex_ptr_t> lock = m_mutex;
		if (m_texture_info_map.is_empty())
			return null;
		auto it = m_texture_info_map->find(name);
		if (!it.is_valid())
			return null;
		node = it->value;
	}

	if (node.is_empty())
		return null;
	return load_texture(node);
}


core::async::iasync<textures::itexture_t>  d3d11_texture_loader::load_texture_async(dom::xml::xml_node_t node)
{
	return m_work_thead->run_async<textures::itexture_t>([=](core::async::iasync<textures::itexture_t> async)->textures::itexture_t
	{
		if (node.is_empty())
			return null;
		d3d11_texture_t tex = new d3d11_texture();
		if (!tex->load(this, node))
			return null;
		m_mutex->lock();
		m_textures += {node->xml_attributes()["name"_s]->xml_as<text::raw_cstr>(), tex.get()};
		m_mutex->unlock();
		return tex.get();
	});
}

core::async::iasync<textures::itexture_t>  d3d11_texture_loader::load_texture_async(text::raw_cstr_t _name)
{
	string name = _name;
	return m_work_thead->run_async<textures::itexture_t>([=](core::async::iasync<textures::itexture_t> async)->textures::itexture_t
	{
		dom::xml::xml_node_t node;
		{	core::async::scope_locker<core::async::mutex_ptr_t> lock = m_mutex;
			if (!m_textures.is_empty())
			{
				auto it = m_textures->find(name);
				if (it.is_valid())
				{
					d3d11_texture_t tex = it->value.lock();
					if (!tex.is_empty())
						return tex.get();
				}
			}
		
			if (m_texture_info_map.is_empty())
				return null;
			auto it = m_texture_info_map->find(name);
			if (!it.is_valid())
				return null;
			node = it->value;
		}

		if (node.is_empty())
			return null;
		return load_texture(node);
	});
}

textures::itexture_t d3d11_texture_loader::find_texture(text::raw_cstr_t name)const
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = m_mutex;
	if (m_textures.is_empty())
		return null;
	auto it = m_textures->find(name);
	return it.is_valid() ? it->value.lock().get() : null; //return it.is_valid() ? it->value().lock<d3d11_texture>().get() : null;
}

#endif
