#include "pch.h"
#include "d3d11/driver.h"

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;


d3d11_index_buffer::d3d11_index_buffer()
	: _index_count(0)
{

}

d3d11_index_buffer::~d3d11_index_buffer()
{
	close();
}

ANG_IMPLEMENT_CLASSNAME(ang::graphics::d3d11::d3d11_index_buffer);
ANG_IMPLEMENT_OBJECTNAME(ang::graphics::d3d11::d3d11_index_buffer);

bool d3d11_index_buffer::is_child_of(type_name_t name)
{
	return name == type_name<d3d11_index_buffer>()
		|| object::is_child_of(name)
		|| buffers::iindex_buffer::is_child_of(name)
		|| buffers::igpu_buffer::is_child_of(name);
}

bool d3d11_index_buffer::is_kind_of(type_name_t name)const
{
	return name == type_name<d3d11_index_buffer>()
		|| object::is_kind_of(name)
		|| buffers::iindex_buffer::is_kind_of(name)
		|| buffers::igpu_buffer::is_kind_of(name);
}

bool d3d11_index_buffer::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<d3d11_index_buffer>())
	{
		*out = static_cast<d3d11_index_buffer*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (buffers::iindex_buffer::query_object(name, out))
	{
		return true;
	}
	else if (buffers::igpu_buffer::query_object(name, out))
	{
		return true;
	}
	return false;
}

buffers::buffer_type_t d3d11_index_buffer::buffer_type()const { return buffers::buffer_type::index_buffer; }

buffers::buffer_usage_t d3d11_index_buffer::buffer_usage()const { return _usage; }

buffers::buffer_bind_flag_t d3d11_index_buffer::buffer_bind_flag()const { return buffers::buffer_bind_flag::index_buffer; }

ibuffer_t d3d11_index_buffer::map(idriver_t, bool writeMode)
{
	return null;
}

bool d3d11_index_buffer::unmap(idriver_t, ibuffer_t)
{
	return false;
}

reflect::var_type_t d3d11_index_buffer::data_type()const { return _index_type; }

wsize d3d11_index_buffer::counter()const { return _index_count; }

wsize d3d11_index_buffer::size_in_bytes()const
{
	reflect::variable_desc desc(_index_type, reflect::var_class::scalar, null, _index_count);
	return desc.get_size_in_bytes();;
}

bool d3d11_index_buffer::create(d3d11_driver_t driver, buffers::buffer_usage_t usage, reflect::var_type_t type, wsize count, static_array<byte> init_data)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	if (type.get() < 3 || type.get() > 6)
		return false;

	_index_type = type;
	_index_count = count;
	_usage = usage;// buffers::buffer_usage::dynamic;

	reflect::variable_desc desc(type, reflect::var_class::scalar, null, count);

	bd.ByteWidth = desc.get_size_in_bytes();
	bd.Usage = (D3D11_USAGE)usage.get();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	switch (bd.Usage)
	{
	case D3D11_USAGE_DEFAULT:
		bd.CPUAccessFlags = 0;
		break;
	case D3D11_USAGE_IMMUTABLE:
		bd.CPUAccessFlags = 0;
		if (init_data.data() == null)
			return false;
		break;
	case D3D11_USAGE_DYNAMIC:
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	default:
		break;
	}

	if (init_data.size() < bd.ByteWidth)
		return false;

	D3D11_SUBRESOURCE_DATA InitData;
	D3D11_SUBRESOURCE_DATA* pInitData = null;
	ZeroMemory(&InitData, sizeof(InitData));
	if (init_data.size() > 0)
	{
		pInitData = &InitData;
		InitData.pSysMem = init_data.data();
	}

	if (FAILED(driver->D3D11Device()->CreateBuffer(&bd, pInitData, &d3d_index_buffer)))
		return !close();
	return true;
}

bool d3d11_index_buffer::close()
{
	_index_count = 0;
	_usage = buffers::buffer_usage::def;
	_index_type =  reflect::var_type::none;
	d3d_index_buffer = null;
	return true;
}

void d3d11_index_buffer::use_buffer(d3d11_driver_t driver)
{
	DXGI_FORMAT format;
	switch (_index_type.get())
	{
	case reflect::var_type::s16:
		format = DXGI_FORMAT_R16_SINT;
		break;
	case reflect::var_type::u16:
		format = DXGI_FORMAT_R16_UINT;
		break;
	case reflect::var_type::s32:
		format = DXGI_FORMAT_R32_SINT;
		break;
	case reflect::var_type::u32:
		format = DXGI_FORMAT_R32_UINT;
		break;
	default:
		break;
	}

	driver->D3D11Context()->IASetIndexBuffer(d3d_index_buffer, format, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


d3d11_vertex_buffer::d3d11_vertex_buffer()
	: _vertex_count(0)
	, _stride(0)
{

}

d3d11_vertex_buffer::~d3d11_vertex_buffer()
{
	close();
}

ANG_IMPLEMENT_CLASSNAME(ang::graphics::d3d11::d3d11_vertex_buffer);
ANG_IMPLEMENT_OBJECTNAME(ang::graphics::d3d11::d3d11_vertex_buffer);

bool d3d11_vertex_buffer::is_child_of(type_name_t name)
{
	return name == type_name<d3d11_vertex_buffer>()
		|| object::is_child_of(name)
		|| buffers::ivertex_buffer::is_child_of(name)
		|| buffers::igpu_buffer::is_child_of(name);
}

bool d3d11_vertex_buffer::is_kind_of(type_name_t name)const
{
	return name == type_name<d3d11_vertex_buffer>()
		|| object::is_kind_of(name)
		|| buffers::ivertex_buffer::is_kind_of(name)
		|| buffers::igpu_buffer::is_kind_of(name);
}

bool d3d11_vertex_buffer::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<d3d11_vertex_buffer>())
	{
		*out = static_cast<d3d11_vertex_buffer*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (buffers::ivertex_buffer::query_object(name, out))
	{
		return true;
	}
	else if (buffers::igpu_buffer::query_object(name, out))
	{
		return true;
	}
	return false;
}

buffers::buffer_type_t d3d11_vertex_buffer::buffer_type()const { return buffers::buffer_type::index_buffer; }

buffers::buffer_usage_t d3d11_vertex_buffer::buffer_usage()const { return _usage; }

buffers::buffer_bind_flag_t d3d11_vertex_buffer::buffer_bind_flag()const { return buffers::buffer_bind_flag::index_buffer; }

ibuffer_t d3d11_vertex_buffer::map(idriver_t, bool writeMode)
{
	return null;
}

bool d3d11_vertex_buffer::unmap(idriver_t, ibuffer_t)
{
	return false;
}

static_array<reflect::attribute_desc> d3d11_vertex_buffer::descriptor()const
{
	if (_vertex_desc.is_empty())
		return static_array<reflect::attribute_desc>(null);
	return static_array<reflect::attribute_desc>(_vertex_desc->data(), _vertex_desc->size());
}

wsize d3d11_vertex_buffer::block_counter()const { return _vertex_count; }

wsize d3d11_vertex_buffer::size_in_bytes()const { return _stride *_vertex_count; }

bool d3d11_vertex_buffer::create(d3d11_driver_t driver, buffers::buffer_usage_t usage, static_array<reflect::attribute_desc> vertex_desc, wsize count, static_array<byte> init_data)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	_vertex_desc = new collections::vector_buffer<reflect::attribute_desc>(vertex_desc.size(), vertex_desc.data());

	_vertex_count = count;
	_usage = usage;// buffers::buffer_usage::dynamic;

	_stride = reflect::attribute_desc::get_size_in_bytes(_vertex_desc);

	if (_stride == 0)
		return false;

	bd.ByteWidth = _stride * _vertex_count;
	bd.Usage = (D3D11_USAGE)usage.get();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.StructureByteStride = _stride;

	switch (bd.Usage)
	{
	case D3D11_USAGE_DEFAULT:
		bd.CPUAccessFlags = 0;
		break;
	case D3D11_USAGE_IMMUTABLE:
		bd.CPUAccessFlags = 0;
		if (init_data.data() == null)
			return false;
		break;
	case D3D11_USAGE_DYNAMIC:
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	default:
		break;
	}

	if (init_data.size() < bd.ByteWidth)
		return false;

	D3D11_SUBRESOURCE_DATA InitData;
	D3D11_SUBRESOURCE_DATA* pInitData = null;
	ZeroMemory(&InitData, sizeof(InitData));
	if (init_data.size() > 0)
	{
		pInitData = &InitData;
		InitData.pSysMem = init_data.data();
	}

	if (FAILED(driver->D3D11Device()->CreateBuffer(&bd, pInitData, &d3d_vertex_buffer)))
		return !close();
	return true;
}

bool d3d11_vertex_buffer::close()
{
	
	return true;
}

void d3d11_vertex_buffer::use_buffer(d3d11_driver_t driver)
{
	uint offset = 0;
	driver->D3D11Context()->IASetVertexBuffers(0, 1, &d3d_vertex_buffer, &_stride, &offset);
}