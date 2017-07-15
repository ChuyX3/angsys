#include "pch.h"
#include "d3d11/driver.h"

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

#if DIRECTX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;


/////////////////////////////////////////////////////////////////////////

bool d3d11::d3d_load_shader_input(collections::vector<graphics::reflect::attribute_desc> const& attributes, collections::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout_desc)
{
	if (input_layout_desc.is_empty()) input_layout_desc = new collections::vector_buffer<D3D11_INPUT_ELEMENT_DESC>();
	else input_layout_desc->clean();
	D3D11_INPUT_ELEMENT_DESC desc;
	memset(&desc, 0, sizeof(desc));

	wsize total = 0;
	wsize size = 0;
	wsize temp = 0;
	wsize res = 0;
	static const wsize aligment = 16u;

	foreach(attributes, [&](graphics::reflect::attribute_desc const& attribute)
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
	});
	return true;
}



d3d11_shaders::d3d11_shaders()
{
}

d3d11_shaders::~d3d11_shaders()
{
}

ANG_IMPLEMENT_CLASSNAME(ang::graphics::d3d11::d3d11_shaders);
ANG_IMPLEMENT_OBJECTNAME(ang::graphics::d3d11::d3d11_shaders);

bool d3d11_shaders::is_child_of(type_name_t name)
{
	return name == type_name<d3d11_shaders>()
		|| object::is_child_of(name)
		|| effects::ishaders::is_child_of(name);
}

bool d3d11_shaders::is_kind_of(type_name_t name)const
{
	return name == type_name<d3d11_shaders>()
		|| object::is_kind_of(name)
		|| effects::ishaders::is_kind_of(name);
}

bool d3d11_shaders::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<d3d11_shaders>())
	{
		*out = static_cast<d3d11_shaders*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (effects::ishaders::query_object(name, out))
	{
		return true;
	}
	return false;
}

bool d3d11_shaders::load(d3d11_effect_library_t library, xml::xml_node_t node)
{
	if (node.is_empty() || !node->xml_has_children())
		return false;
	bool res = true;
	_technique_name = node->xml_attributes()["name"];
	foreach(node->xml_children(), [&](xml::xml_node_t node)
	{
		auto name = node->xml_name().as<cwstr_t>();
		if (name == "vertex_shader")
			res &= load_vertex_shader(library, node);
		else if (name == "pixel_shader")
			res &= load_pixel_shader(library, node);
	});

	return res;
}

bool d3d11_shaders::load_vertex_shader(d3d11_effect_library_t library, xml::xml_node_t vertex)
{
	if (!vertex->xml_has_children())
		return false;

	string entry = "main";
	wstring filename;
	string config;
	string code;
	array<string> configs;
	ID3DBlob* compiled_code;

	collections::vector<D3D11_INPUT_ELEMENT_DESC> desc_list = new collections::vector_buffer<D3D11_INPUT_ELEMENT_DESC>();

	foreach(vertex->xml_children(), [&](xml::xml_node_t node)
	{
		auto name = node->xml_name().as<cwstr_t>();
		if (name == "uniforms"_s)
			load_vs_const_buffer(library, node);
		else if (name == "input_layout"_s)
			graphics::reflect::attribute_desc::load(node, _input_layout);
		else if (name == "entry"_s)
			entry = node->xml_value();
		else if (name == "file"_s)
			filename = node->xml_value();
		else if (name == "code"_s)
			code = node->xml_value();
		else if (name == "compile_config"_s)
			config = node->xml_value();
	});

	if (code.is_empty())
	{
		if (filename.is_empty())
			return false;

		auto _filename = library->find_file(filename);
		core::files::input_text_file_t file;
		if (_filename.is_empty()) library->get_file_system()->open(filename, file); //file = new core::files::input_text_file(filename);
		else library->get_file_system()->open(_filename, file); //file = new core::files::input_text_file(_filename);
		if (!file->is_valid())
			return false;

		if (!file->is_valid())
			return false;

		if (!file->read([&](streams::itext_input_stream_t stream)
		{
			collections::vector<D3D_SHADER_MACRO> macros;
			ibuffer_t code = static_cast<streams::text_buffer_input_stream*>(stream.get())->buffer();
			if (!config.is_empty())
			{
				configs = ang::move(config->split("-D"));
				foreach(configs, [&](string& value) {
					macros += {(cstr_t)value, null};
				});
				macros += {null, null};
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
			foreach(configs, [&](string& value) {
				macros += {(cstr_t)value, null};
			});
			macros += {null, null};
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
	}

	d3d_load_shader_input(_input_layout, desc_list);

	library->driver()->D3D11Device()->CreateVertexShader(compiled_code->GetBufferPointer(), compiled_code->GetBufferSize(), NULL, &d3d_vertex_shader);
	HRESULT hr;
	if (desc_list->is_empty() ||
		(hr = library->driver()->D3D11Device()->CreateInputLayout(desc_list->data(), desc_list->size(), compiled_code->GetBufferPointer(), compiled_code->GetBufferSize(), &d3d_input_layout)) != S_OK)
	{
		//desc_list reflection
		d3d_vertex_shader = null;
		compiled_code->Release();
		return false;
	}

	compiled_code->Release();
	return true;
}

bool d3d11_shaders::load_pixel_shader(d3d11_effect_library_t library, xml::xml_node_t pixel)
{
	if (!pixel->xml_has_children())
		return false;

	string entry = "main";
	wstring filename;
	string config;
	string code;
	array<string> configs;
	ID3DBlob* compiled_code;

	foreach(pixel->xml_children(), [&](xml::xml_node_t node)
	{
		auto name = node->xml_name().as<cwstr_t>();
		if (name == "uniforms")
			load_ps_const_buffer(library, node);
		else if (name == "samplers")
			load_ps_samplers(library, node);
		else if (name == "entry")
			entry = node->xml_value();
		else if (name == "file")
			filename = node->xml_value();
		else if (name == "code")
			code = node->xml_value();
		else if (name == "compile_config")
			config = node->xml_value();
	});

	if (code.is_empty())
	{
		if (filename.is_empty())
			return false;

		auto _filename = library->find_file(filename);
		core::files::input_text_file_t file;
		if (_filename.is_empty()) library->get_file_system()->open(filename, file); //file = new core::files::input_text_file(filename);
		else library->get_file_system()->open(_filename, file); //file = new core::files::input_text_file(_filename);
		if (!file->is_valid())
			return false;

		if (!file->read([&](streams::itext_input_stream_t stream)
		{
			collections::vector<D3D_SHADER_MACRO> macros;
			ibuffer_t code = static_cast<streams::text_buffer_input_stream*>(stream.get())->buffer();
			if (!config.is_empty())
			{
				configs = ang::move(config->split("-D"));
				foreach(configs, [&](string& value) {
					macros += {(cstr_t)value, null};
				});
				macros += {null, null};
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
			foreach(configs, [&](string& value) {
				macros += {(cstr_t)value, null};
			});
			macros += {null, null};
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
	}

	library->driver()->D3D11Device()->CreatePixelShader(compiled_code->GetBufferPointer(), compiled_code->GetBufferSize(), NULL, &d3d_pixel_shader);
	compiled_code->Release();
	return true;
}

bool d3d11_shaders::load_vs_const_buffer(d3d11_effect_library_t library, xml::xml_node_t node)
{
	graphics::reflect::variable_desc uniforms;
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

	d3d_vs_const_buffers += buffer;
	_vs_uniforms += ang::move(uniforms);
	return false;
}

bool d3d11_shaders::load_ps_const_buffer(d3d11_effect_library_t library, xml::xml_node_t node)
{
	graphics::reflect::variable_desc uniforms;
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

	d3d_ps_const_buffers += buffer;
	_ps_uniforms += ang::move(uniforms);
	return false;
}

bool d3d11_shaders::load_ps_samplers(d3d11_effect_library_t library, xml::xml_node_t samplers)
{
	if (!samplers->xml_has_children())
		return false;

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampDesc.MaxAnisotropy = 1;

	foreach(samplers->xml_children(), [&](xml::xml_node_t sampler)
	{
		if ((cwstr_t)sampler->xml_name() == "sampler"_s)
		{
			auto att = sampler->xml_attributes();
			auto _mode = att["wrap"].as<textures::tex_wrap_mode_t>();
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
				d3d_ps_samplers += sampler_state;
				sampler_state->Release();
			}
		}
	});
	return true;
}

bool d3d11_shaders::use_shaders(d3d11_driver_t driver)
{
	driver->D3D11Context()->IASetInputLayout(d3d_input_layout);
	driver->D3D11Context()->VSSetShader(d3d_vertex_shader, NULL, 0);
	driver->D3D11Context()->PSSetShader(d3d_pixel_shader, NULL, 0);
	if (!d3d_vs_const_buffers.is_empty())	
		driver->D3D11Context()->VSSetConstantBuffers(0, d3d_vs_const_buffers->counter(), (ID3D11Buffer**)d3d_vs_const_buffers->data());
	else
		driver->D3D11Context()->VSSetConstantBuffers(0, 0, null);
	
	if (!d3d_ps_const_buffers.is_empty())
		driver->D3D11Context()->PSSetConstantBuffers(0, d3d_ps_const_buffers->counter(), (ID3D11Buffer**)d3d_ps_const_buffers->data());
	else
		driver->D3D11Context()->PSSetConstantBuffers(0, 0, null);

	if (!d3d_ps_samplers.is_empty())	
		driver->D3D11Context()->PSSetSamplers(0, d3d_ps_samplers->counter(), (ID3D11SamplerState**)d3d_ps_samplers->data());
	else 
		driver->D3D11Context()->PSSetSamplers(0, 0, null);
	
	return true;
}

string d3d11_shaders::resource_name()const { return _technique_name; }

static_array<reflect::attribute_desc> d3d11_shaders::input_layout()const
{
	return _input_layout.is_empty() ? null : static_array<reflect::attribute_desc> (_input_layout->data(), _input_layout->size());
}

static_array<reflect::variable_desc> d3d11_shaders::vs_uniforms_layouts()const
{
	return _input_layout.is_empty() ? null : static_array<reflect::variable_desc>(_vs_uniforms->data(), _input_layout->size());
}

static_array<reflect::variable_desc> d3d11_shaders::ps_uniforms_layouts()const
{
	return _input_layout.is_empty() ? null : static_array<reflect::variable_desc>(_ps_uniforms->data(), _input_layout->size());
}

reflect::variable d3d11_shaders::map_vs_uniform(idriver_t, index idx)
{
	if (_vs_uniforms.is_empty() || _vs_uniforms->counter() <= idx)
		return reflect::variable();

	reflect::variable_desc desc = _vs_uniforms[idx];
	wsize size = desc.get_size_in_bytes();
	if(size == 0)
		return reflect::variable();

	auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator);
	pointer buff = alloc->memory_alloc(size);
	desc.position(idx);
	memset(buff, 0, size);
	return reflect::variable(static_array<byte>((byte*)buff, size), ang::move(desc));
}

reflect::variable d3d11_shaders::map_vs_uniform(idriver_t, cstr_t name)
{
	if (_vs_uniforms.is_empty())
		return reflect::variable();
	index i = 0;
	for (auto it = _vs_uniforms->begin(); it.is_valid(); ++it, ++i)
	{
		if (it->var_name() == name)
		{
			reflect::variable_desc desc = *it;
			wsize size = desc.get_size_in_bytes();
			if (size == 0)
				return reflect::variable();

			auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator);
			pointer buff = alloc->memory_alloc(size);
			desc.position(i);
			memset(buff, 0, size);
			return reflect::variable(static_array<byte>((byte*)buff, size), desc);
		}
	}
	return reflect::variable();
}

reflect::variable d3d11_shaders::map_vs_uniform(idriver_t, cwstr_t name)
{
	if (_vs_uniforms.is_empty())
		return reflect::variable();
	index i = 0;
	for (auto it = _vs_uniforms->begin(); it.is_valid(); ++it, ++i)
	{
		if (it->var_name() == name)
		{
			reflect::variable_desc desc = *it;
			wsize size = desc.get_size_in_bytes();
			if (size == 0)
				return reflect::variable();

			auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator);
			pointer buff = alloc->memory_alloc(size);
			desc.position(i);
			memset(buff, 0, size);
			return reflect::variable(static_array<byte>((byte*)buff, size), desc);
		}
	}
	return reflect::variable();
}

reflect::variable d3d11_shaders::map_ps_uniform(idriver_t, index idx)
{
	if (_ps_uniforms.is_empty() || _ps_uniforms->counter() <= idx)
		return reflect::variable();

	reflect::variable_desc desc = _ps_uniforms[idx];
	wsize size = desc.get_size_in_bytes();
	if (size == 0)
		return reflect::variable();

	auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator);
	pointer buff = alloc->memory_alloc(size);
	memset(buff, 0, size);
	desc.position(idx);
	return reflect::variable(static_array<byte>((byte*)buff, size), desc);
}

reflect::variable d3d11_shaders::map_ps_uniform(idriver_t, cstr_t name)
{
	if (_ps_uniforms.is_empty())
		return reflect::variable();
	index i = 0;
	for (auto it = _ps_uniforms->begin(); it.is_valid(); ++it, ++i)
	{
		if (it->var_name() == name)
		{
			reflect::variable_desc desc = *it;
			wsize size = desc.get_size_in_bytes();
			if (size == 0)
				return reflect::variable();

			auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator);
			pointer buff = alloc->memory_alloc(size);
			desc.position(i);
			memset(buff, 0, size);
			return reflect::variable(static_array<byte>((byte*)buff, size), desc);
		}
	}
	return reflect::variable();
}

reflect::variable d3d11_shaders::map_ps_uniform(idriver_t, cwstr_t name)
{
	if (_ps_uniforms.is_empty())
		return reflect::variable();
	index i = 0;
	for (auto it = _ps_uniforms->begin(); it.is_valid(); ++it, ++i)
	{
		if (it->var_name() == name)
		{
			reflect::variable_desc desc = *it;
			wsize size = desc.get_size_in_bytes();
			if (size == 0)
				return reflect::variable();

			auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator);
			pointer buff = alloc->memory_alloc(size);
			desc.position(i);
			memset(buff, 0, size);
			return reflect::variable(static_array<byte>((byte*)buff, size), desc);
		}
	}
	return reflect::variable();
}

void d3d11_shaders::unmap_vs_uniform(idriver_t _driver, reflect::variable& var)
{
	auto driver = static_cast<d3d11_driver*>(_driver.get());
	auto desc = var.descriptor();
	index idx = desc.position();
	auto data = var.raw_data();
	if (_vs_uniforms.is_empty() || _vs_uniforms->counter() <= idx || data.get() == null)
		throw exception_t(except_code::invalid_param);

	driver->D3D11Context()->UpdateSubresource(d3d_vs_const_buffers[idx].get(), 0, NULL, data.get(), 0, 0);
	auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator);
	alloc->memory_release(data.get());
	var = reflect::variable();
}

void d3d11_shaders::unmap_ps_uniform(idriver_t _driver, reflect::variable& var)
{
	auto driver = static_cast<d3d11_driver*>(_driver.get());
	auto desc = var.descriptor();
	index idx = desc.position();
	auto data = var.raw_data();
	if (_ps_uniforms.is_empty() || _ps_uniforms->counter() <= idx || data.get() == null)
		throw exception_t(except_code::invalid_param);

	driver->D3D11Context()->UpdateSubresource(d3d_ps_const_buffers[idx].get(), 0, NULL, data.get(), 0, 0);
	auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator);
	alloc->memory_release(data.get());
	var = reflect::variable();
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
	: main_mutex(new core::async::mutex())
	, _driver(parent)
{

}

d3d11_effect_library::~d3d11_effect_library()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::graphics::d3d11::d3d11_effect_library);
ANG_IMPLEMENT_OBJECTNAME(ang::graphics::d3d11::d3d11_effect_library);

bool d3d11_effect_library::is_child_of(type_name_t name)
{
	return name == type_name<d3d11_effect_library>()
		|| object::is_child_of(name)
		|| effects::ieffect_library::is_child_of(name);
}

bool d3d11_effect_library::is_kind_of(type_name_t name)const
{
	return name == type_name<d3d11_effect_library>()
		|| object::is_kind_of(name)
		|| effects::ieffect_library::is_kind_of(name);
}

bool d3d11_effect_library::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<d3d11_effect_library>())
	{
		*out = static_cast<d3d11_effect_library*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (effects::ieffect_library::query_object(name, out))
	{
		return true;
	}
	return false;
}

void d3d11_effect_library::set_file_system(core::files::ifile_system_t fs) { _fs = fs; }

bool d3d11_effect_library::load_library(xml::xml_node_t library)
{
	if (library.is_empty() || !library->xml_has_children())
		return false;

	foreach(library->xml_children(), [&](xml::xml_node_t node)
	{
		auto name = node->xml_name().as<cwstr_t>();
		if (name == "sources"_s)
			load_sources(node);
		else if (name == "technique"_s)
			load_technique(node);//hack load_effect(driver, node);
	});
	return true;
}

core::async::iasync_t<effects::ieffect_library_t> d3d11_effect_library::load_library_async(xml::xml_node_t library)
{
	return core::async::async_task<effects::ieffect_library_t>::run_async([=](core::async::iasync<effects::ieffect_library_t>* async, var_args_t args)->effects::ieffect_library*
	{
		if (library.is_empty() || !library->xml_has_children())
		{
			async->cancel();
			return null;
		}	

		foreach(library->xml_children(), [&](xml::xml_node_t node)
		{
			auto name = node->xml_name().as<cwstr_t>();
			if (name == "sources"_s)
				load_sources(node);
			else if (name == "technique"_s)
				load_technique(node);//hack load_effect(driver, node);
		});
		return this;
	}, { library.get() });
}

bool d3d11_effect_library::load_sources(xml::xml_node_t sources)
{
	if (!sources->xml_has_children())
		return false;

	foreach(sources->xml_children(), [&](xml::xml_node_t file)
	{
		auto name = file->xml_name().as<cwstr_t>();
		if (name != "file"_s)
			return;
		main_mutex->lock();
		_source_map += {file->xml_attributes()["sid"_s].as<cwstr_t>(), file->xml_value().as<cwstr_t>()};
		main_mutex->unlock();
	});

	return true;
}

effects::ieffect_t d3d11_effect_library::load_effect(xml::xml_node_t node)
{
	//TODO:
	return null;
}

core::async::iasync_t<effects::ieffect_t> d3d11_effect_library::load_effect_async(xml::xml_node_t node)
{
	//TODO:
	return null;
}

effects::ishaders_t d3d11_effect_library::load_technique(xml::xml_node_t node)
{
	if (!node->xml_has_children())
		return null;
	d3d11_shaders_t technique = new d3d11_shaders();
	if (!technique->load(this, node))
		return null;
	main_mutex->lock();
	_shaders += {node->xml_attributes()["name"_s].as<cwstr_t>(), technique.get() };
	main_mutex->unlock();
	return technique.get();
}


core::async::iasync_t<effects::ishaders_t> d3d11_effect_library::load_technique_async(xml::xml_node_t node)
{
	return core::async::async_task<effects::ishaders_t>::run_async([=](core::async::iasync<effects::ishaders_t>* async, var_args_t args)->effects::ishaders*
	{
		if (!node->xml_has_children())
			return null;
		d3d11_shaders_t technique = new d3d11_shaders();
		if (!technique->load(this, node))
			return null;
		main_mutex->lock();
		_shaders += {node->xml_attributes()["name"_s].as<cwstr_t>(), technique.get() };
		main_mutex->unlock();
		return technique.get();
	});
}


effects::ieffect_t d3d11_effect_library::find_effect(cstr_t name)const
{
	return null;
}

effects::ieffect_t d3d11_effect_library::find_effect(cwstr_t name)const
{
	return null;
}

effects::ishaders_t d3d11_effect_library::find_technique(cstr_t name)const
{
	if (_shaders.is_empty())
		return null;
	auto it = _shaders->find(name);
	return it.is_valid() ? it->value().get() : null;
}

effects::ishaders_t d3d11_effect_library::find_technique(cwstr_t name)const
{
	if (_shaders.is_empty())
		return null;
	auto it = _shaders->find(name);
	return it.is_valid() ? it->value().get() : null;
}

#endif
