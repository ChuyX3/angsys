#include "pch.h"
#include "d3d11/driver.h"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

#if DIRECTX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;


/////////////////////////////////////////////////////////////////////////

bool d3d11::d3d_load_shader_input(collections::vector<graphics::reflect::attribute_desc> const& attributes, collections::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout_desc)
{
	if (input_layout_desc.is_empty()) input_layout_desc = new collections::vector_buffer<D3D11_INPUT_ELEMENT_DESC>();
	else input_layout_desc->clear();
	D3D11_INPUT_ELEMENT_DESC desc;
	memset(&desc, 0, sizeof(desc));

	wsize total = 0;
	wsize size = 0;
	wsize temp = 0;
	wsize res = 0;
	static const wsize aligment = 16u;

	for(graphics::reflect::attribute_desc const& attribute : attributes)
	{
		desc.SemanticName = attribute.semantic().to_string();
		desc.SemanticIndex = attribute.semantic_index();
		desc.InputSlot = 0;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;

		switch (attribute.var_type().get())
		{
		case graphics::reflect::var_type::f32:
			switch (attribute.var_class().get())
			{
			case graphics::reflect::var_class::vec4:
				desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				size = sizeof(float) * 4;
				break;
			case graphics::reflect::var_class::vec3:
				desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				size = sizeof(float) * 3;
				break;
			case graphics::reflect::var_class::vec2:
				desc.Format = DXGI_FORMAT_R32G32_FLOAT;;
				size = sizeof(float) * 2;
				break;
			case graphics::reflect::var_class::scalar:
				desc.Format = DXGI_FORMAT_R32_FLOAT;
				size = sizeof(float);
				break;
			default:
				break;
			}
			break;
		case graphics::reflect::var_type::s32:
			switch (attribute.var_class().get())
			{
			case graphics::reflect::var_class::vec4:
				desc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				size = sizeof(int) * 4;
				break;
			case graphics::reflect::var_class::vec3:
				desc.Format = DXGI_FORMAT_R32G32B32_SINT;
				size = sizeof(int) * 3;
				break;
			case graphics::reflect::var_class::vec2:
				desc.Format = DXGI_FORMAT_R32G32_SINT;
				size = sizeof(int) * 2;
				break;
			case graphics::reflect::var_class::scalar:
				desc.Format = DXGI_FORMAT_R32_SINT;
				size = sizeof(int);
				break;
			default:
				break;
			}
			break;
		case graphics::reflect::var_type::u32:
			switch (attribute.var_class().get())
			{
			case graphics::reflect::var_class::vec4:
				desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				size = sizeof(uint) * 4;
				break;
			case graphics::reflect::var_class::vec3:
				desc.Format = DXGI_FORMAT_R32G32B32_UINT;
				size = sizeof(uint) * 3;
				break;
			case graphics::reflect::var_class::vec2:
				desc.Format = DXGI_FORMAT_R32G32_UINT;
				size = sizeof(uint) * 2;
				break;
			case graphics::reflect::var_class::scalar:
				desc.Format = DXGI_FORMAT_R32_UINT;
				size = sizeof(uint);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		if (size != 0)
		{
			temp = (total % aligment);
			res = aligment - temp;
			if (res < aligment)
			{
				if (res > size)
					total += reflect::get_memory_size_aligned(temp, size) - temp;
				else if (res < size)
					total += res;
			}
			desc.AlignedByteOffset = total;
			total += size;
		}

		input_layout_desc += desc;
	}
	return true;
}



d3d11_shaders::d3d11_shaders()
{
}

d3d11_shaders::~d3d11_shaders()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::d3d11::d3d11_shaders);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::d3d11::d3d11_shaders, object, effects::ishaders);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::d3d11::d3d11_shaders, object, effects::ishaders);

bool d3d11_shaders::load(d3d11_effect_library_t library, dom::xml::xml_node_t node)
{
	if (node.is_empty() || !node->xml_has_children())
		return false;
	bool res = true;
	m_technique_name = node->xml_attributes()["name"_s]->xml_as<text::raw_cstr>();
	for(dom::xml::xml_node_t node : node->xml_children())
	{
		auto name = node->xml_name()->xml_as<text::raw_cstr>();
		if (name == "vertex_shader"_s)
			res &= load_vertex_shader(library, node);
		else if (name == "pixel_shader"_s)
			res &= load_pixel_shader(library, node);
	}

	return res;
}

bool d3d11_shaders::load_vertex_shader(d3d11_effect_library_t library, dom::xml::xml_node_t vertex)
{
	if (!vertex->xml_has_children())
		return false;

	string entry = "main";
	wstring filename = null;
	string config = null;
	string code = null;
	array<string> configs;
	ID3DBlob* compiled_code;

	collections::vector<D3D11_INPUT_ELEMENT_DESC> desc_list = new collections::vector_buffer<D3D11_INPUT_ELEMENT_DESC>();

	for(dom::xml::xml_node_t node : vertex->xml_children())
	{
		auto name = node->xml_name()->xml_as<text::raw_cstr>();
		if (name == "uniforms"_s)
			load_vs_const_buffer(library, node);
		else if (name == "input_layout"_s)
			graphics::reflect::attribute_desc::load(node, m_input_layout);
		else if (name == "entry"_s)
			entry = node->xml_value()->xml_as<text::raw_cstr>();
		else if (name == "file"_s)
			filename = node->xml_value()->xml_as<text::raw_cstr>();
		else if (name == "code"_s)
			code = node->xml_value()->xml_as<text::raw_cstr>();
		else if (name == "compile_config"_s)
			config = node->xml_value()->xml_as<text::raw_cstr>();
	}

	if (code.is_empty())
	{
		if (filename.is_empty())
			return false;

		auto _filename = library->find_file((text::raw_cstr_t)filename);
		core::files::input_text_file_t file;
		if (_filename.is_empty()) library->get_file_system()->open(filename, &file); //file = new core::files::input_text_file(filename);
		else library->get_file_system()->open(_filename, &file); //file = new core::files::input_text_file(_filename);
		if (!file->is_valid())
			return false;

		if (!file->is_valid())
			return false;

		if (!file->map([&](ibuffer_view_t code)
		{
			collections::vector<D3D_SHADER_MACRO> macros;
			//ibuffer_t code = static_cast<streams::text_buffer_input_stream*>(stream.get())->buffer();
			if (!config.is_empty())
			{
				configs = config->split("-D");
				for(string& value : configs) {
					macros += {(cstr_t)value, null};
				}
				macros += D3D_SHADER_MACRO{null, null};
			}
			ID3DBlob* error;
			HRESULT hr = D3DCompile(code->buffer_ptr(), code->buffer_size()
				, NULL, macros.is_empty() ? NULL : macros->data(), NULL, (cstr_t)entry
				, "vs_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &compiled_code, &error);
			if (FAILED(hr)) {
				d3d11_debug_print(2, "%s\n", error->GetBufferPointer());
				error->Release();
				return false;
			}
			return true;
		}))return false;
	}
	else
	{
		collections::vector<D3D_SHADER_MACRO> macros;
		if (!config.is_empty())
		{
			configs = ang::move(config->split("-D"));
			for(string& value : configs) {
				macros += {(cstr_t)value, null};
			}
			macros += D3D_SHADER_MACRO{null, null};
		}
		ID3DBlob* error;
		HRESULT hr = D3DCompile(code->cstr(), code->length()
			, NULL, macros.is_empty() ? NULL : macros->data(), NULL, (cstr_t)entry
			, "vs_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &compiled_code, &error);
		if (FAILED(hr)) {
			d3d11_debug_print(2, "%s\n", error->GetBufferPointer());
			error->Release();
			return false;
		}
	}

	d3d_load_shader_input(m_input_layout, desc_list);

	library->driver()->D3D11Device()->CreateVertexShader(compiled_code->GetBufferPointer(), compiled_code->GetBufferSize(), NULL, &m_d3d_vertex_shader);
	HRESULT hr;
	if (desc_list->is_empty() ||
		(hr = library->driver()->D3D11Device()->CreateInputLayout(desc_list->data(), desc_list->size(), compiled_code->GetBufferPointer(), compiled_code->GetBufferSize(), &m_d3d_input_layout)) != S_OK)
	{
		//desc_list reflection
		m_d3d_vertex_shader = null;
		compiled_code->Release();
		return false;
	}

	compiled_code->Release();
	return true;
}

bool d3d11_shaders::load_pixel_shader(d3d11_effect_library_t library, dom::xml::xml_node_t pixel)
{
	if (!pixel->xml_has_children())
		return false;

	string entry = "main";
	wstring filename = null;
	string config = null;
	string code = null;
	array<string> configs;
	ID3DBlob* compiled_code;

	for(dom::xml::xml_node_t node : pixel->xml_children())
	{
		auto name = node->xml_name()->xml_as<text::raw_cstr>();
		if (name == "uniforms"_s)
			load_ps_const_buffer(library, node);
		else if (name == "samplers"_s)
			load_ps_samplers(library, node);
		else if (name == "entry"_s)
			entry = node->xml_value()->xml_as<text::raw_cstr>();
		else if (name == "file"_s)
			filename = node->xml_value()->xml_as<text::raw_cstr>();
		else if (name == "code"_s)
			code = node->xml_value()->xml_as<text::raw_cstr>();
		else if (name == "compile_config"_s)
			config = node->xml_value()->xml_as<text::raw_cstr>();
	}

	if (code.is_empty())
	{
		if (filename.is_empty())
			return false;

		auto _filename = library->find_file((text::raw_cstr)filename);
		core::files::input_text_file_t file;
		if (_filename.is_empty()) library->get_file_system()->open(filename, &file); //file = new core::files::input_text_file(filename);
		else library->get_file_system()->open(_filename, &file); //file = new core::files::input_text_file(_filename);
		if (!file->is_valid())
			return false;

		if (!file->map([&](ibuffer_view_t code)
		{
			collections::vector<D3D_SHADER_MACRO> macros;
			if (!config.is_empty())
			{
				configs = ang::move(config->split("-D"));
				for(string& value : configs) {
					macros += {(cstr_t)value, null};
				}
				macros += D3D_SHADER_MACRO{null, null};
			}
			ID3DBlob* error;
			HRESULT hr = D3DCompile(code->buffer_ptr(), code->buffer_size()
				, NULL, macros.is_empty() ? NULL : macros->data(), NULL, (cstr_t)entry
				, "ps_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &compiled_code, &error);
			if (FAILED(hr)) {
				d3d11_debug_print(2, "%s\n", error->GetBufferPointer());
				error->Release();
				return false;
			}
			return true;
		}))return false;
	}
	else
	{
		collections::vector<D3D_SHADER_MACRO> macros;
		if (!config.is_empty())
		{
			configs = ang::move(config->split("-D"));
			for(string& value : configs) {
				macros += {(cstr_t)value, null};
			}
			macros += D3D_SHADER_MACRO{null, null};
		}
		ID3DBlob* error;
		HRESULT hr = D3DCompile(code->cstr(), code->length()
			, NULL, macros.is_empty() ? NULL : macros->data(), NULL, (cstr_t)entry
			, "ps_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &compiled_code, &error);
		if (FAILED(hr)) {
			d3d11_debug_print(2, "%s\n", error->GetBufferPointer());
			error->Release();
			return false;
		}
	}

	library->driver()->D3D11Device()->CreatePixelShader(compiled_code->GetBufferPointer(), compiled_code->GetBufferSize(), NULL, &m_d3d_pixel_shader);
	compiled_code->Release();
	return true;
}

bool d3d11_shaders::load_vs_const_buffer(d3d11_effect_library_t library, dom::xml::xml_node_t node)
{
	graphics::reflect::varying_desc uniforms;
	if (!uniforms.load(node, 16U))
		return false;

	com_wrapper<ID3D11Buffer> buffer;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = uniforms.get_size_in_bytes();
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	//D3D11_SUBRESOURCE_DATA InitData;
	HRESULT hr = library->driver()->D3D11Device()->CreateBuffer(&bd, NULL, &buffer);
	if (FAILED(hr))
		return false;

	m_d3d_vs_const_buffers += buffer;
	m_vs_uniforms += ang::move(uniforms);
	return false;
}

bool d3d11_shaders::load_ps_const_buffer(d3d11_effect_library_t library, dom::xml::xml_node_t node)
{
	graphics::reflect::varying_desc uniforms;
	if (!uniforms.load(node, 16U))
		return false;

	com_wrapper<ID3D11Buffer> buffer;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = uniforms.get_size_in_bytes();
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	//D3D11_SUBRESOURCE_DATA InitData;
	HRESULT hr = library->driver()->D3D11Device()->CreateBuffer(&bd, NULL, &buffer);
	if (FAILED(hr))
		return false;

	m_d3d_ps_const_buffers += buffer;
	m_ps_uniforms += ang::move(uniforms);
	return false;
}

bool d3d11_shaders::load_ps_samplers(d3d11_effect_library_t library, dom::xml::xml_node_t samplers)
{
	if (!samplers->xml_has_children())
		return false;

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampDesc.MaxAnisotropy = 1;

	for(dom::xml::xml_node_t sampler : samplers->xml_children())
	{
		if ((text::raw_cstr)sampler->xml_name() == "sampler"_s)
		{
			auto att = sampler->xml_attributes();
			auto _mode = att["wrap"_s]->xml_as<textures::tex_wrap_mode_t>();
			//auto _type = att["type"];
			D3D11_TEXTURE_ADDRESS_MODE mode = _mode.get() == textures::tex_wrap_mode::clamp ? D3D11_TEXTURE_ADDRESS_CLAMP
				: _mode.get() == textures::tex_wrap_mode::mirrored ? D3D11_TEXTURE_ADDRESS_MIRROR
				: D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressU = mode;
			sampDesc.AddressV = mode;
			sampDesc.AddressW = mode;
			sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//=_type...
			ID3D11SamplerState* sampler_state;
			if (SUCCEEDED(library->driver()->D3D11Device()->CreateSamplerState(&sampDesc, &sampler_state)))
			{
				m_d3d_ps_samplers += sampler_state;
				sampler_state->Release();
			}
		}
	}
	return true;
}

bool d3d11_shaders::use_shaders(d3d11_driver_t driver)
{
	driver->D3D11Context()->IASetInputLayout(m_d3d_input_layout);
	driver->D3D11Context()->VSSetShader(m_d3d_vertex_shader, NULL, 0);
	driver->D3D11Context()->PSSetShader(m_d3d_pixel_shader, NULL, 0);
	if (!m_d3d_vs_const_buffers.is_empty())
		driver->D3D11Context()->VSSetConstantBuffers(0, m_d3d_vs_const_buffers->counter(), (ID3D11Buffer**)m_d3d_vs_const_buffers->data());
	else
		driver->D3D11Context()->VSSetConstantBuffers(0, 0, null);
	
	if (!m_d3d_ps_const_buffers.is_empty())
		driver->D3D11Context()->PSSetConstantBuffers(0, m_d3d_ps_const_buffers->counter(), (ID3D11Buffer**)m_d3d_ps_const_buffers->data());
	else
		driver->D3D11Context()->PSSetConstantBuffers(0, 0, null);

	if (!m_d3d_ps_samplers.is_empty())
		driver->D3D11Context()->PSSetSamplers(0, m_d3d_ps_samplers->counter(), (ID3D11SamplerState**)m_d3d_ps_samplers->data());
	else 
		driver->D3D11Context()->PSSetSamplers(0, 0, null);
	
	return true;
}

text::istring_view_t d3d11_shaders::resource_name()const { return m_technique_name.get(); }

array_view<reflect::attribute_desc> d3d11_shaders::input_layout()const
{
	return m_input_layout.is_empty() ? array_view<reflect::attribute_desc>() : collections::to_array(m_input_layout->data(), m_input_layout->size());
}

array_view<reflect::varying_desc> d3d11_shaders::vs_uniforms_layouts()const
{
	return m_input_layout.is_empty() ? array_view<reflect::varying_desc>() : collections::to_array(m_vs_uniforms->data(), m_input_layout->size());
}

array_view<reflect::varying_desc> d3d11_shaders::ps_uniforms_layouts()const
{
	return m_input_layout.is_empty() ? array_view<reflect::varying_desc>() : collections::to_array(m_ps_uniforms->data(), m_input_layout->size());
}

reflect::varying d3d11_shaders::map_vs_uniform(idriver_t, index idx)
{
	if (m_vs_uniforms.is_empty() || m_vs_uniforms->counter() <= idx)
		return reflect::varying();

	reflect::varying_desc desc = m_vs_uniforms[idx];
	wsize size = desc.get_size_in_bytes();
	if(size == 0)
		return reflect::varying();

	array_view<byte> buff = to_array(memory::aligned16_allocator<byte>().allocate(size), size);
	memset(buff.get(), 0, size);
	desc.position(idx);
	return reflect::varying(buff, ang::move(desc));
}

reflect::varying d3d11_shaders::map_vs_uniform(idriver_t, text::raw_cstr_t name)
{
	if (m_vs_uniforms.is_empty())
		return reflect::varying();
	index i = 0;
	for (auto it = m_vs_uniforms->begin(); it.is_valid(); ++it, ++i)
	{
		if ((text::raw_cstr)it->var_name() == name)
		{
			reflect::varying_desc desc = *it;
			wsize size = desc.get_size_in_bytes();
			if (size == 0)
				return reflect::varying();

			array_view<byte> buff = to_array(memory::aligned16_allocator<byte>().allocate(size), size);
			memset(buff.get(), 0, size);
			desc.position(i);
			return reflect::varying(buff, desc);
		}
	}
	return reflect::varying();
}

reflect::varying d3d11_shaders::map_ps_uniform(idriver_t, index idx)
{
	if (m_ps_uniforms.is_empty() || m_ps_uniforms->counter() <= idx)
		return reflect::varying();

	reflect::varying_desc desc = m_ps_uniforms[idx];
	wsize size = desc.get_size_in_bytes();
	if (size == 0)
		return reflect::varying();

	array_view<byte> buff = to_array(memory::aligned16_allocator<byte>().allocate(size), size);
	memset(buff.get(), 0, size);
	desc.position(idx);
	return reflect::varying(buff, desc);
}

reflect::varying d3d11_shaders::map_ps_uniform(idriver_t, text::raw_cstr_t name)
{
	if (m_ps_uniforms.is_empty())
		return reflect::varying();
	index i = 0;
	for (auto it = m_ps_uniforms->begin(); it.is_valid(); ++it, ++i)
	{
		if ((text::raw_cstr_t)it->var_name() == name)
		{
			reflect::varying_desc desc = *it;
			wsize size = desc.get_size_in_bytes();
			if (size == 0)
				return reflect::varying();

			array_view<byte> buff = to_array(memory::aligned16_allocator<byte>().allocate(size), size);
			memset(buff.get(), 0, size);
			desc.position(i);
			return reflect::varying(buff, desc);
		}
	}
	return reflect::varying();
}

void d3d11_shaders::unmap_vs_uniform(idriver_t _driver, reflect::varying& var)
{
	auto driver = static_cast<d3d11_driver*>(_driver.get());
	auto desc = var.descriptor();
	index idx = desc.position();
	auto data = var.raw_data();
	if (m_vs_uniforms.is_empty() || m_vs_uniforms->counter() <= idx || data.get() == null)
		throw exception_t(except_code::invalid_param);
	driver->execute_on_thread_safe([&]() {
		driver->D3D11Context()->UpdateSubresource(m_d3d_vs_const_buffers[idx].get(), 0, NULL, data.get(), 0, 0); 
	});
	memory::aligned16_allocator<byte>().deallocate(data.get());
	var = reflect::varying();
}

void d3d11_shaders::unmap_ps_uniform(idriver_t _driver, reflect::varying& var)
{
	auto driver = static_cast<d3d11_driver*>(_driver.get());
	auto desc = var.descriptor();
	index idx = desc.position();
	auto data = var.raw_data();
	if (m_ps_uniforms.is_empty() || m_ps_uniforms->counter() <= idx || data.get() == null)
		throw exception_t(except_code::invalid_param);
	driver->execute_on_thread_safe([&]() {
		driver->D3D11Context()->UpdateSubresource(m_d3d_ps_const_buffers[idx].get(), 0, NULL, data.get(), 0, 0);
	});
	memory::aligned16_allocator<byte>().deallocate(data.get());
	var = reflect::varying();
}


//bool d3d11_shaders::update_vs_uniforms(ID3D11DeviceContext2* context, index idx, void* data)
//{
//	if (d3d_vs_const_buffers.is_empty() || d3d_vs_const_buffers->counter() <= idx)
//		return false;
//	context->UpdateSubresource(d3d_vs_const_buffers[idx], 0, null, data, 0, 0);
//	return true;
//}
//
//bool d3d11_shaders::update_ps_uniforms(ID3D11DeviceContext2* context, index idx, void* data)
//{
//	if (d3d_ps_const_buffers.is_empty() || d3d_ps_const_buffers->counter() <= idx)
//		return false;
//	context->UpdateSubresource(d3d_ps_const_buffers[idx], 0, null, data, 0, 0);
//	return true;
//}

/////////////////////////////////////////////////////////////////


d3d11_effect_library::d3d11_effect_library(d3d11_driver_t parent)
	: m_mutex(make_shared<core::async::mutex>())
	, m_driver(ang::move(parent))
{

}

d3d11_effect_library::~d3d11_effect_library()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::d3d11::d3d11_effect_library);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::d3d11::d3d11_effect_library, object, effects::ieffect_library);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::d3d11::d3d11_effect_library, object, effects::ieffect_library);

void d3d11_effect_library::set_file_system(core::files::ifile_system_t fs) { m_fs = fs; }

bool d3d11_effect_library::load_library(dom::xml::xml_node_t library)
{
	if (library.is_empty() || !library->xml_has_children())
		return false;

	for(dom::xml::xml_node_t node : library->xml_children())
	{
		auto name = node->xml_name()->xml_as<text::raw_cstr>();
		if (name == "sources"_s)
			load_sources(node);
		else if (name == "shaders"_s)
			load_shaders(node);//hack load_effect(driver, node);
	}
	return true;
}

core::async::iasync<effects::ieffect_library_t> d3d11_effect_library::load_library_async(dom::xml::xml_node_t library)
{
	return core::async::task::run_async<effects::ieffect_library_t>([=](core::async::iasync<effects::ieffect_library_t> async)->effects::ieffect_library_t
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
			else if (name == "shaders"_s)
				load_shaders(node);//hack load_effect(driver, node);
		}
		return this;
	});
}

bool d3d11_effect_library::load_sources(dom::xml::xml_node_t sources)
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

effects::ieffect_t d3d11_effect_library::load_effect(dom::xml::xml_node_t node)
{
	//TODO:
	return null;
}

core::async::iasync<effects::ieffect_t> d3d11_effect_library::load_effect_async(dom::xml::xml_node_t node)
{
	//TODO:
	return null;
}

effects::ishaders_t d3d11_effect_library::load_shaders(dom::xml::xml_node_t node)
{
	if (!node->xml_has_children())
		return null;
	d3d11_shaders_t shaders = new d3d11_shaders();
	if (!shaders->load(this, node))
		return null;
	m_mutex->lock();
	m_shaders += {node->xml_attributes()["name"_s]->xml_as<text::raw_cstr>(), shaders.get() };
	m_mutex->unlock();
	return shaders.get();
}


core::async::iasync<effects::ishaders_t> d3d11_effect_library::load_shaders_async(dom::xml::xml_node_t node)
{
	return core::async::task::run_async<effects::ishaders_t>([=](core::async::iasync<effects::ishaders_t> async)->effects::ishaders_t
	{
		if (!node->xml_has_children())
			return null;
		d3d11_shaders_t shaders = new d3d11_shaders();
		if (!shaders->load(this, node))
			return null;
		m_mutex->lock();
		m_shaders += {node->xml_attributes()["name"_s]->xml_as<text::raw_cstr>(), shaders.get() };
		m_mutex->unlock();
		return shaders.get();
	});
}


effects::ieffect_t d3d11_effect_library::find_effect(text::raw_cstr_t name)const
{
	return null;
}

effects::ishaders_t d3d11_effect_library::find_shaders(text::raw_cstr name)const
{
	if (m_shaders.is_empty())
		return null;
	auto it = m_shaders->find(name);
	return it.is_valid() ? it->value.get() : null;
}

#endif
