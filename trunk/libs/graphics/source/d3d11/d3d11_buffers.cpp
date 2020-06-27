#include "pch.h"
#include "d3d11/d3d11_driver.h"

#if DIRECTX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;


d3d11_index_buffer::d3d11_index_buffer()
	: m_index_count(0)
{

}

void d3d11_index_buffer::dispose()
{
	close();
}

resources::iresource_t d3d11_index_buffer::resource()const { return const_cast<d3d11_index_buffer*>(this); }

resources::resource_type_t d3d11_index_buffer::resource_type()const { return resources::resource_type::index_buffer; }

string d3d11_index_buffer::resource_sid()const { return (cstr_t)m_resource_sid; }

void d3d11_index_buffer::resource_sid(cstr_t sid) { m_resource_sid = sid; }

intfptr d3d11_index_buffer::fast_cast(resources::resource_type_t type) {
	return type == resources::resource_type::index_buffer ? static_cast<buffers::iindex_buffer*>(this) : null;
}

buffers::buffer_type_t d3d11_index_buffer::buffer_type()const { return buffers::buffer_type::index_buffer; }

buffers::buffer_usage_t d3d11_index_buffer::buffer_usage()const { return m_usage; }

buffers::buffer_bind_flag_t d3d11_index_buffer::buffer_bind_flag()const { return buffers::buffer_bind_flag::index_buffer; }

ibuffer_t d3d11_index_buffer::map(idriver_t, bool writeMode)
{
	return null;
}

bool d3d11_index_buffer::unmap(idriver_t, ibuffer_t)
{
	return false;
}

reflect::var_type_t d3d11_index_buffer::data_type()const { return m_index_type; }

wsize d3d11_index_buffer::counter()const { return m_index_count; }

wsize d3d11_index_buffer::size_in_bytes()const
{
	reflect::varying_desc desc(m_index_type, reflect::var_class::scalar, null, m_index_count);
	return desc.get_size_in_bytes();;
}

bool d3d11_index_buffer::create(d3d11_driver_t driver, buffers::buffer_usage_t usage, reflect::var_type_t type, wsize count, array_view<byte> init_data, string sid)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	if ((int)type.get() < 3 || (int)type.get() > 6)
		return false;

	m_index_type = type;
	m_index_count = count;
	m_usage = usage;// buffers::buffer_usage::dynamic;

	reflect::varying_desc desc(type, reflect::var_class::scalar, null, count);

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

	if (FAILED(driver->D3D11Device()->CreateBuffer(&bd, pInitData, &m_index_buffer)))
		return !close();

	m_resource_sid = sid;
	return true;
}

bool d3d11_index_buffer::close()
{
	m_index_count = 0;
	m_usage = buffers::buffer_usage::def;
	m_index_type =  reflect::var_type::none;
	m_index_buffer = null;
	return true;
}

void d3d11_index_buffer::use_buffer(d3d11_driver_t driver)
{
	DXGI_FORMAT format;
	switch (m_index_type.get())
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

	driver->D3D11Context()->IASetIndexBuffer(m_index_buffer, format, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


d3d11_vertex_buffer::d3d11_vertex_buffer()
	: m_vertex_count(0)
	, m_stride(0)
{

}

void d3d11_vertex_buffer::dispose()
{
	close();
}

resources::iresource_t d3d11_vertex_buffer::resource()const { return const_cast<d3d11_vertex_buffer*>(this); }

resources::resource_type_t d3d11_vertex_buffer::resource_type()const { return resources::resource_type::vertex_buffer; }

string d3d11_vertex_buffer::resource_sid()const { return (cstr_t)m_resource_sid; }

void d3d11_vertex_buffer::resource_sid(cstr_t sid) { m_resource_sid = sid; }

intfptr d3d11_vertex_buffer::fast_cast(resources::resource_type_t type) {
	return type == resources::resource_type::vertex_buffer ? static_cast<buffers::ivertex_buffer*>(this) : null;
}

buffers::buffer_type_t d3d11_vertex_buffer::buffer_type()const { return buffers::buffer_type::index_buffer; }

buffers::buffer_usage_t d3d11_vertex_buffer::buffer_usage()const { return m_usage; }

buffers::buffer_bind_flag_t d3d11_vertex_buffer::buffer_bind_flag()const { return buffers::buffer_bind_flag::index_buffer; }

ibuffer_t d3d11_vertex_buffer::map(idriver_t, bool writeMode)
{
	return null;
}

bool d3d11_vertex_buffer::unmap(idriver_t, ibuffer_t)
{
	return false;
}

array_view<reflect::attribute_desc>const& d3d11_vertex_buffer::descriptor()const
{
	return m_vertex_desc;
}

wsize d3d11_vertex_buffer::block_counter()const { return m_vertex_count; }

wsize d3d11_vertex_buffer::size_in_bytes()const { return m_stride *m_vertex_count; }

bool d3d11_vertex_buffer::create(d3d11_driver_t driver, buffers::buffer_usage_t usage, array_view<reflect::attribute_desc_t> vertex_desc, wsize count, ibuffer_t vertex_data, string sid)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = (D3D11_USAGE)usage.get();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	switch (bd.Usage)
	{
	case D3D11_USAGE_DEFAULT:
		bd.CPUAccessFlags = 0;
		break;
	case D3D11_USAGE_IMMUTABLE:
		bd.CPUAccessFlags = 0;
		if (vertex_data.is_empty())
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

	uint i = 0;
	m_vertex_count = count;
	m_usage = usage;// buffers::buffer_usage::dynamic;
	reflect::attribute_desc::calculate_positions(vertex_desc);	
	m_stride = reflect::attribute_desc::get_size_in_bytes(vertex_desc);
	bd.ByteWidth = m_vertex_count * m_stride;
	bd.StructureByteStride = m_stride;
	m_vertex_desc = vertex_desc;
	for (windex i = 0; i < vertex_desc.size(); i++)
		m_attribute_by_id[{vertex_desc[i].semantic(), vertex_desc[i].semantic_index()}] = i;	

	D3D11_SUBRESOURCE_DATA InitData;
	D3D11_SUBRESOURCE_DATA* pInitData = null;
	ZeroMemory(&InitData, sizeof(InitData));
	if (!vertex_data.is_empty())
	{
		pInitData = &InitData;
		InitData.pSysMem = vertex_data->buffer_ptr();
	}

	if (FAILED(driver->D3D11Device()->CreateBuffer(&bd, pInitData, &m_vertex_buffer)))
		return !close();

	m_resource_sid = sid;
	return true;
}

bool d3d11_vertex_buffer::close()
{
	m_resource_sid = null;
	m_vertex_count = 0;
	m_stride = 0;
	m_usage = buffers::buffer_usage::def;
	m_vertex_desc = null;
	m_vertex_buffer = null;
	m_attribute_by_id.clear();
	return true;
}

void d3d11_vertex_buffer::use_buffer(d3d11_driver_t driver)
{
	ID3D11Buffer* buffer = m_vertex_buffer.get();
	uint stride = m_stride;
	uint offset = 0;
	driver->D3D11Context()->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
}

void d3d11_vertex_buffer::use_buffer(d3d11_driver_t driver, array_view<reflect::attribute_desc_t> const& desc)
{
	vector<ID3D11Buffer*> buffer;
	vector<uint> stride;
	vector<uint> offset;
	buffer.size(desc.size());
	stride.size(desc.size());
	offset.size(desc.size());
	for (wsize i = 0; i < desc.size(); i++)
	{
		buffer[i] = null;
		stride[i] = 0;
		offset[i] = 0;
		auto it = m_attribute_by_id.find({ desc[i].semantic(), desc[i].semantic_index() });
		if (it.is_valid()) {
			buffer[i] = m_vertex_buffer.get();
			stride[i] = m_stride;
			offset[i] = m_vertex_desc[it->value].position();
		}
	}
	driver->D3D11Context()->IASetVertexBuffers(0, buffer.size(), &buffer[0], &stride[0], &offset[0]);
}

#endif
