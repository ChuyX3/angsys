#include "pch.h"
#include "d3d11/d3d11_driver.h"

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
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::d3d11::d3d11_texture, object, textures::itexture, resources::iresource);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::d3d11::d3d11_texture, object, textures::itexture, buffers::igpu_buffer, resources::iresource);

void d3d11_texture::clear()
{
	m_d3d_shader_view = null;
	m_d3d_raw_resource = null;
	m_resource_sid = null;
	m_bind_flags = buffers::buffer_bind_flag::none;
	m_usage = buffers::buffer_usage::def;
	m_tex_type = textures::tex_type::null;
	m_tex_format = DXGI_FORMAT_UNKNOWN;
	m_tex_dimentions = { 0,0,0 };
}

resources::iresource_t d3d11_texture::resource()const { return const_cast<d3d11_texture*>(this); }

resources::resource_type_t d3d11_texture::resource_type()const { return resources::resource_type::texture; }

string d3d11_texture::resource_sid()const { return (cstr_t)m_resource_sid; }

void d3d11_texture::resource_sid(cstr_t sid) { m_resource_sid = sid; }

effects::ieffect_t d3d11_texture::to_effect() { return null; }

effects::ishaders_t d3d11_texture::to_shaders() { return null; }

textures::itexture_t d3d11_texture::to_texture() { return const_cast<d3d11_texture*>(this); }

iframe_buffer_t d3d11_texture::to_frame_buffer() { return null; }

buffers::iindex_buffer_t d3d11_texture::to_index_buffer() { return null; }

buffers::ivertex_buffer_t d3d11_texture::to_vertex_buffer() { return null; }


bool d3d11_texture::create(d3d11_driver_t driver, textures::tex_type_t type, textures::tex_format_t color_format, buffers::buffer_usage_t usage, buffers::buffer_bind_flag_t flags, size3d<uint> dimentions, string sid)
{
	if (color_format == textures::tex_format::null)
		return false;
	clear();

	HRESULT hr;
	DXGI_FORMAT format = d3d11::traslate_format_dx(color_format);
	D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc;
	ZeroMemory(&srvdesc, sizeof(srvdesc));
	srvdesc.Format = format;

	switch (type)
	{
	case textures::tex_type::tex1D: {
		D3D11_TEXTURE1D_DESC desc;
		com_wrapper<ID3D11Texture1D> texture;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = (uint)dimentions.width;
		desc.Format = format;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Usage = (D3D11_USAGE)usage.get(); //D3D11_USAGE_DEFAULT
		desc.BindFlags = flags; //D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		srvdesc.Texture1D.MipLevels = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		srvdesc.Texture1D.MostDetailedMip = 0;

		hr = driver->D3D11Device()->CreateTexture1D(&desc, null, &texture);
		if (FAILED(hr))
			return false;
		m_d3d_raw_resource = texture.get();
	}
	case textures::tex_type::tex2D: {
		D3D11_TEXTURE2D_DESC desc;
		com_wrapper<ID3D11Texture2D> texture;
		ZeroMemory(&desc, sizeof(desc));
		desc.Height = (uint)dimentions.height;
		desc.Width = (uint)dimentions.width;
		desc.Format = format;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = (D3D11_USAGE)usage.get(); //D3D11_USAGE_DEFAULT
		desc.BindFlags = flags; //D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvdesc.Texture2D.MipLevels = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		srvdesc.Texture2D.MostDetailedMip = 0;

		hr = driver->D3D11Device()->CreateTexture2D(&desc, null, &texture);
		if (FAILED(hr))
			return false;	
		m_d3d_raw_resource = texture.get();
	}
	case textures::tex_type::tex3D: {
		D3D11_TEXTURE3D_DESC desc;
		com_wrapper<ID3D11Texture3D> texture;
		ZeroMemory(&desc, sizeof(desc));
		desc.Height = (uint)dimentions.height;
		desc.Width = (uint)dimentions.width;
		desc.Depth = (uint)dimentions.depth;
		desc.Format = format;
		desc.MipLevels = 1;
		desc.Usage = (D3D11_USAGE)usage.get(); //D3D11_USAGE_DEFAULT
		desc.BindFlags = flags; //D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		srvdesc.Texture3D.MipLevels = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		srvdesc.Texture3D.MostDetailedMip = 0;

		hr = driver->D3D11Device()->CreateTexture3D(&desc, null, &texture);
		if (FAILED(hr))
			return false;
		m_d3d_raw_resource = texture.get();
	}
	default:
		return false;
	}

	if (flags.is_active<buffers::buffer_bind_flag::shader_resource>())
	{
		hr = driver->D3D11Device()->CreateShaderResourceView(m_d3d_raw_resource, &srvdesc, &m_d3d_shader_view);
		if (FAILED(hr))
		{
			m_d3d_shader_view = null;
			clear();
			return false;
		}
	}
	m_resource_sid = sid;
	return true;
}

bool d3d11_texture::attach(com_wrapper<ID3D11Resource> resource, d3d11_driver_t driver, string sid)
{
	clear();
	if (resource.is_empty())
		return false;
	m_d3d_raw_resource = resource.get();

	D3D11_RESOURCE_DIMENSION dim;
	resource->GetType(&dim);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc;
	ZeroMemory(&srvdesc, sizeof(srvdesc));
		
	switch (dim)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D: {
		m_tex_type = textures::tex_type::tex1D;
		D3D11_TEXTURE1D_DESC tex_desc;
		static_cast<ID3D11Texture1D*>(m_d3d_raw_resource.get())->GetDesc(&tex_desc);
		m_tex_format = tex_desc.Format;
		m_usage = (buffers::buffer_usage)tex_desc.Usage;
		m_bind_flags = (buffers::buffer_bind_flag)tex_desc.BindFlags;
		m_tex_dimentions = { tex_desc.Width, 1, 1 };
	} break;
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D: {
		m_tex_type = textures::tex_type::tex2D;
		D3D11_TEXTURE2D_DESC tex_desc;
		static_cast<ID3D11Texture2D*>(m_d3d_raw_resource.get())->GetDesc(&tex_desc);
		m_tex_format = tex_desc.Format;
		m_usage = (buffers::buffer_usage)tex_desc.Usage;
		m_bind_flags = (buffers::buffer_bind_flag)tex_desc.BindFlags;
		m_tex_dimentions = { tex_desc.Width, tex_desc.Height, 1 };

	} break;
	case D3D11_RESOURCE_DIMENSION_TEXTURE3D: {
		m_tex_type = textures::tex_type::tex3D;
		D3D11_TEXTURE3D_DESC tex_desc;
		static_cast<ID3D11Texture3D*>(m_d3d_raw_resource.get())->GetDesc(&tex_desc);
		m_tex_format = tex_desc.Format;
		m_usage = (buffers::buffer_usage)tex_desc.Usage;
		m_bind_flags = (buffers::buffer_bind_flag)tex_desc.BindFlags;
		m_tex_dimentions = { tex_desc.Width, tex_desc.Height, tex_desc.Depth };
	} break;
	}

	if (!driver.is_empty() && m_bind_flags.is_active<buffers::buffer_bind_flag::shader_resource>())
	{
		HRESULT hr = driver->D3D11Device()->CreateShaderResourceView(resource, NULL, &m_d3d_shader_view);
		if (FAILED(hr)) {
			m_d3d_shader_view = null;
			clear();
			return false;
		}
	}	
	m_resource_sid = sid;
	return true;
}

bool d3d11_texture::attach(com_wrapper<ID3D11ShaderResourceView> view, string sid)
{
	clear();
	
	if (view.is_empty())
		return false;
	
	D3D11_RESOURCE_DIMENSION dim;
	m_d3d_shader_view = view;
	view->GetResource(&m_d3d_raw_resource);
	m_d3d_raw_resource->GetType(&dim);
	switch (dim)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D: {
			m_tex_type = textures::tex_type::tex1D;
			D3D11_TEXTURE1D_DESC desc;
			static_cast<ID3D11Texture1D*>(m_d3d_raw_resource.get())->GetDesc(&desc);
			m_tex_format = desc.Format;
			m_usage = (buffers::buffer_usage)desc.Usage;
			m_bind_flags = (buffers::buffer_bind_flag)desc.BindFlags;
			m_tex_dimentions = { desc.Width, 1, 1 };
		}
		break;
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D: {
			m_tex_type = textures::tex_type::tex2D;
			D3D11_TEXTURE2D_DESC desc;
			static_cast<ID3D11Texture2D*>(m_d3d_raw_resource.get())->GetDesc(&desc);
			m_tex_format = desc.Format;
			m_usage = (buffers::buffer_usage)desc.Usage;
			m_bind_flags = (buffers::buffer_bind_flag)desc.BindFlags;
			m_tex_dimentions = { desc.Width, desc.Height, 1 };
		}
		break;
	case D3D11_RESOURCE_DIMENSION_TEXTURE3D: {
			m_tex_type = textures::tex_type::tex3D;
			D3D11_TEXTURE3D_DESC desc;
			static_cast<ID3D11Texture3D*>(m_d3d_raw_resource.get())->GetDesc(&desc);
			m_tex_format = desc.Format;
			m_usage = (buffers::buffer_usage)desc.Usage;
			m_bind_flags = (buffers::buffer_bind_flag)desc.BindFlags;
			m_tex_dimentions = { desc.Width, desc.Height, desc.Depth };
		}
		break;
	default: 
		clear();
		return false;
	}
	m_resource_sid = sid;
	return true;
}

//bool d3d11_texture::load(d3d11_driver_t loader, array<wstring> files, textures::tex_type_t type)
//{
//	m_texture_name = filename.get();
//
//	if (node->xml_has_children())
//	{
//		bool res = true;
//		d3d11_driver_t driver = loader->driver();
//		auto att = node->xml_attributes();
//		m_tex_type = att["type"_s]->xml_as<textures::tex_type_t>();
//
//		collections::vector<core::files::input_binary_file_t> files;
//
//		for (dom::xml::xml_node_t tex_node : node->xml_children())
//		{
//			if ((cstr_t)tex_node->xml_name() != "texture"_s)
//				continue;
//			wstring filename = tex_node->xml_value();
//			auto _filename = loader->find_file((cstr_t)filename);
//			core::files::input_binary_file_t file;
//			if (_filename.is_empty())  loader->get_file_system()->open(filename, &file); // file = new core::files::input_binary_file(filename);
//			else loader->get_file_system()->open(_filename, &file); //file = new core::files::input_binary_file(_filename);
//			if (!file->is_valid())
//				continue;
//			files += file;
//		}
//
//		tex_file_info_t info;
//
//		if (files.is_empty() || !d3d11_texture_loader::load_texture(
//			driver
//			, files
//			, info
//			, &m_d3d_raw_resource
//			, &m_d3d_shader_view
//			, m_tex_type.get() == textures::tex_type::texCube))
//			return false;
//		
//		m_usage = (buffers::buffer_usage)D3D11_USAGE_DEFAULT;
//		m_bind_flags = (buffers::buffer_bind_flag)(D3D11_BIND_SHADER_RESOURCE | (info.autoGenMips ? D3D11_BIND_RENDER_TARGET : 0));
//
//		m_tex_dimentions.width = info.width;
//		m_tex_dimentions.height = max(info.height, 1);
//		m_tex_dimentions.depth = max(info.depth, 1);
//		m_tex_format = (DXGI_FORMAT)info.format;
//
//		return true;
//	}
//	else
//	{
//		bool res = true;
//		d3d11_driver_t driver = loader->driver();
//		auto att = node->xml_attributes();
//		m_tex_type = att["type"_s]->xml_as<textures::tex_type_t>();
//
//		wstring filename = node->xml_value();
//
//		auto _filename = loader->find_file((cstr_t)filename);
//		core::files::input_binary_file_t file;
//		if (_filename.is_empty())loader->get_file_system()->open(filename, &file); // file = new core::files::input_binary_file(filename);
//		else loader->get_file_system()->open(_filename, &file); //file = new core::files::input_binary_file(_filename);
//
//		tex_file_info_t info;
//
//		if (file.is_empty() || !file->is_valid()|| !d3d11_texture_loader::load_texture(
//			driver
//			, file
//			, info
//			, &m_d3d_raw_resource
//			, &m_d3d_shader_view
//			, m_tex_type.get() == textures::tex_type::texCube))
//			return false;
//		
//		m_usage = (buffers::buffer_usage)D3D11_USAGE_DEFAULT;
//		m_bind_flags = (buffers::buffer_bind_flag)(D3D11_BIND_SHADER_RESOURCE | (info.autoGenMips ? D3D11_BIND_RENDER_TARGET : 0));
//
//		m_tex_dimentions.width = info.width;
//		m_tex_dimentions.height = max(info.height, 1);
//		m_tex_dimentions.depth = max(info.depth, 1);
//		m_tex_format = (DXGI_FORMAT)info.format;
//
//		return true;
//	}
//}

bool d3d11_texture::load(d3d11_driver_t driver, string filename, textures::tex_type_t type, string sid)
{
	m_resource_sid = filename.get();
	bool res = true;

	m_tex_type = type;

	core::files::input_binary_file_t file;
	driver->get_file_system()->open(filename, &file);

	tex_file_info_t info;

	if (file.is_empty() || !file->is_valid() || !d3d11_texture_loader::load_texture(
		driver
		, file
		, info
		, &m_d3d_raw_resource
		, &m_d3d_shader_view
		, m_tex_type.get() == textures::tex_type::texCube))
		return false;

	m_usage = (buffers::buffer_usage)D3D11_USAGE_DEFAULT;
	m_bind_flags = (buffers::buffer_bind_flag)(D3D11_BIND_SHADER_RESOURCE | (info.autoGenMips ? D3D11_BIND_RENDER_TARGET : 0));

	m_tex_dimentions.width = info.width;
	m_tex_dimentions.height = max(info.height, 1u);
	m_tex_dimentions.depth = max(info.depth, 1u);
	m_tex_format = (DXGI_FORMAT)info.format;

	m_resource_sid = sid;

	return true;
}


bool d3d11_texture::use_texture(d3d11_driver_t driver, index idx)const
{
	driver->D3D11Context()->PSSetShaderResources(idx, 1, (ID3D11ShaderResourceView**)&m_d3d_shader_view);
	return true;
}

textures::tex_type_t d3d11_texture::tex_type()const { 
	return m_tex_type;
}

textures::tex_format_t d3d11_texture::tex_format()const { 
	return traslate_format_dx(m_tex_format); 
}

size3d<uint> d3d11_texture::tex_dimentions()const { 
	return m_tex_dimentions;
}

buffers::buffer_type_t d3d11_texture::buffer_type()const {
	return buffers::buffer_type::texture;
}

buffers::buffer_usage_t d3d11_texture::buffer_usage()const {
	return m_usage;
}

buffers::buffer_bind_flag_t d3d11_texture::buffer_bind_flag()const
{
	return m_bind_flags;
}

ibuffer_t d3d11_texture::map(idriver_t, bool writeMode)
{
	return null;
}

bool d3d11_texture::unmap(idriver_t, ibuffer_t)
{
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
