#include "pch.h"
#include "d3d11/d3d11_driver.h"
#include <comdef.h>

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

#if DIRECTX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;
using namespace ang::graphics::effects;

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
		desc.SemanticName = (castr_t)attribute.semantic().to_string();
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
					total += align_up(size, temp) - temp;
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

bool d3d11::d3d_reflect_shader_input(collections::vector<graphics::reflect::attribute_desc>& attributes, ID3D11ShaderReflection* vertexShaderReflection)
{
	using namespace reflect;

	D3D11_SHADER_DESC shaderDesc;
	vertexShaderReflection->GetDesc(&shaderDesc);
	graphics::reflect::attribute_desc attribute;

	for (uint i = 0; i < shaderDesc.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;

		vertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

		attribute.semantic(var_semantic_t::parse(castr_t(paramDesc.SemanticName)));
		attribute.semantic_index(paramDesc.SemanticIndex);

		if (paramDesc.Mask == 1)
			attribute.var_class(var_class::scalar);
		else if (paramDesc.Mask <= 3)
			attribute.var_class(var_class::vec2);
		else if (paramDesc.Mask <= 7)
			attribute.var_class(var_class::vec3);
		else if (paramDesc.Mask <= 15)
			attribute.var_class(var_class::vec4);

		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			attribute.var_type(var_type::u32);
		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			attribute.var_type(var_type::s32);
		else  if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			attribute.var_type(var_type::f32);

		attributes += attribute;
	}

	return true;
}

bool d3d11::d3d_reflect_shader_variable(reflect::varying_desc_t& field, ID3D11ShaderReflectionVariable* varInfo)
{
	using namespace reflect;

	D3D11_SHADER_VARIABLE_DESC varDesc;
	varInfo->GetDesc(&varDesc);
	field.var_name(castr_t(varDesc.Name));
	field.position(varDesc.StartOffset);
	
	auto type = varInfo->GetType();
	D3D11_SHADER_TYPE_DESC typeDesc;
	type->GetDesc(&typeDesc);

	switch (typeDesc.Class)
	{
	case D3D_SVC_STRUCT: {
		field.var_class(var_class::scalar);
		field.var_type(var_type::block);
		reflect::uniform_fields_t fields;

		for (index i = 0; i < typeDesc.Members; i++) {
			reflect::varying_desc_t child;
			d3d_reflect_shader_variable(child, varInfo->GetBuffer()->GetVariableByIndex(i));
			field.fields(ang::move(fields), false);
		}
		return true;
	}

	case D3D_SVC_SCALAR: field.var_class(var_class::scalar); break;
	case D3D_SVC_VECTOR: switch (typeDesc.Columns * typeDesc.Rows) {
	case 2: field.var_class(var_class::vec2); break;
	case 3: field.var_class(var_class::vec3); break;
	case 4: field.var_class(var_class::vec4); break;
	} break;
	case D3D_SVC_MATRIX_ROWS:
	case D3D_SVC_MATRIX_COLUMNS: switch (typeDesc.Columns * typeDesc.Rows) {
	case 4: field.var_class(var_class::mat2); break;
	case 9: field.var_class(var_class::mat3); break;
	case 16: field.var_class(var_class::mat4); break;
	} break;
	}

	switch (typeDesc.Type)
	{
	case D3D_SVT_INT: field.var_type(var_type::s32); break;
	case D3D_SVT_UINT: field.var_type(var_type::u32); break;
	case D3D_SVT_BUFFER: 
		field.var_type(var_type::buffer); 
		break;
	default:field.var_type(var_type::f32); break;
	}
}

bool d3d11::d3d_reflect_shader_uniforms(reflect::uniform_fields_t uniforms, ID3D11ShaderReflection* vertexShaderReflection)
{
	using namespace reflect;

	ID3D11ShaderReflectionConstantBuffer* cb = nullptr;
	D3D11_SHADER_DESC reflectDesc;
	vertexShaderReflection->GetDesc(&reflectDesc);
	for (index i = 0; i < reflectDesc.ConstantBuffers; ++i)
	{
		cb = vertexShaderReflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC desc;
		cb->GetDesc(&desc);
		reflect::varying_desc_t cbuffer;
		cbuffer.var_class(var_class::scalar);
		cbuffer.var_type(var_type::block);
		cbuffer.array_count(1);
		reflect::uniform_fields_t fields;
		for (index j = 0; j < desc.Variables; j++)
		{
			reflect::varying_desc_t field;
			d3d_reflect_shader_variable(field, cb->GetVariableByIndex(j));
			fields += ang::move(field);
		}
		cbuffer.fields(ang::move(fields), false);
		uniforms += ang::move(cbuffer);
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////

d3d11_shaders::d3d11_shaders()
{
}

d3d11_shaders::~d3d11_shaders()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::d3d11::d3d11_shaders);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::d3d11::d3d11_shaders, object, ishaders, resources::iresource);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::d3d11::d3d11_shaders, object, ishaders, resources::iresource);

resources::iresource_t d3d11_shaders::resource()const { return const_cast<d3d11_shaders*>(this); }

resources::resource_type_t d3d11_shaders::resource_type()const { return resources::resource_type::shaders; }

string d3d11_shaders::resource_sid()const { return (cstr_t)m_resource_sid; }

effects::ieffect_t d3d11_shaders::to_effect() { return null; }

effects::ishaders_t d3d11_shaders::to_shaders() { return const_cast<d3d11_shaders*>(this); }

textures::itexture_t d3d11_shaders::to_texture() { return null; }

iframe_buffer_t d3d11_shaders::to_frame_buffer() { return null; }

buffers::iindex_buffer_t d3d11_shaders::to_index_buffer() { return null; }

buffers::ivertex_buffer_t d3d11_shaders::to_vertex_buffer() { return null; }

bool d3d11_shaders::load(d3d11_driver_t driver, string vertex, string pixel, string sid, string_ptr_t log)
{
	string error;
	if (vertex.is_empty() || pixel.is_empty())
		return false;

	if (!(error = load_vertex_shader(driver, vertex)).is_empty()) {
		if (!log.is_empty())
			*log = error.get();
		return false;
	}

	if (!(error = load_pixel_shader(driver, pixel)).is_empty()) {
		if (!log.is_empty())
			*log = error.get();
		return false;
	}

	m_resource_sid = sid;

	return true;
}

bool d3d11_shaders::load(d3d11_driver_t driver, shader_info_t const& vertex, shader_info_t const& pixel, string sid, string_ptr_t log)
{
	string error;

	if (!(error = load_vertex_shader(driver, vertex)).is_empty()) {
		if (!log.is_empty())
			*log = error.get();
		return false;
	}

	if (!(error = load_pixel_shader(driver, pixel)).is_empty()) {
		if (!log.is_empty())
			*log = error.get();
		return false;
	}
	m_resource_sid = sid;
	return true;
}

string d3d11_shaders::load_vertex_shader(d3d11_driver_t driver, string shader_info)
{
	astring entry = "main"_s;
	string filename = null;
	astring config = null;
	astring code = null;
	collections::ienum_ptr<string> configs;
	collections::vector<D3D11_INPUT_ELEMENT_DESC> desc_list;
	ID3DBlob* compiled_code;
	HRESULT hr;

	auto list = shader_info->split(":"_r);
	int c = 0;
	for (string const& str : list) {
		switch (c)
		{
		case 0: filename = str; break;
		case 1:  if(!str.is_empty()) entry = (cstr_t)str; break;
		case 2: config = (cstr_t)str; break;
		}
		c++;
	}
	m_input_layout->clear();
	m_d3d_vs_const_buffers->clear();

	if (filename.is_empty())
		return "d3d11_shaders::load_vertex_shader: invalid parameters"_r;

	core::files::input_text_file_t file;
	driver->get_file_system()->open(filename, &file); //file = new core::files::input_text_file(_filename);
	if (!file->is_valid())
		return "d3d11_shaders::load_vertex_shader: invalid file name"_r;

	astring driver_type = "HLSL"_s;
	collections::vector<D3D_SHADER_MACRO> macros;
	macros += {(castr_t)driver_type, null};
	if (!config.is_empty())
	{
		configs = ang::move(config->split("-D"));
		for (string& value : configs) {
			macros += { cstr_t(value).cstr<text::encoding::ascii>(), null};
		}
	}
	macros += D3D_SHADER_MACRO{ null, null };
	ID3DBlob* error;
	if (FAILED(hr = D3DCompile(code->cstr(), code->length()
		, NULL, macros.is_empty() ? NULL : macros->data(), NULL, (castr_t)entry
		, "vs_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &compiled_code, &error
	))) {
		castr_t log = (const char*)error->GetBufferPointer();
		return log;
	}

	reflect::uniform_fields_t uniforms = null;
	com_wrapper<ID3D11ShaderReflection> vertexShaderReflection = null;
	if (FAILED(hr = D3DReflect(
		compiled_code->GetBufferPointer(),
		compiled_code->GetBufferSize(),
		__uuidof(ID3D11ShaderReflection),
		(void**)(ID3D11ShaderReflection**)&vertexShaderReflection
	))) {
		_com_error error(hr, null);
		cwstr_t log = error.ErrorMessage();
		return log;
	}

	d3d_reflect_shader_input(m_input_layout, vertexShaderReflection);	
	d3d_reflect_shader_uniforms(uniforms, vertexShaderReflection);
	for (reflect::varying_desc_t& desc : uniforms)
		load_vs_const_buffer(driver, desc);
	
	
	d3d_load_shader_input(m_input_layout, desc_list);


	driver->D3D11Device()->CreateVertexShader(compiled_code->GetBufferPointer(), compiled_code->GetBufferSize(), NULL, &m_d3d_vertex_shader);

	if (desc_list->is_empty() ||
		(FAILED(hr = driver->D3D11Device()->CreateInputLayout(desc_list->data(), desc_list->size(), compiled_code->GetBufferPointer(), compiled_code->GetBufferSize(), &m_d3d_input_layout))))
	{	
		m_d3d_vertex_shader = null;
		compiled_code->Release();

		_com_error error(hr, null);
		cwstr_t log = error.ErrorMessage();
		return log;
	}
	compiled_code->Release();
	return null;
}

string d3d11_shaders::load_vertex_shader(d3d11_driver_t driver, shader_info_t const& shader_info)
{
	astring entry = null;
	collections::ienum_ptr<string> configs;
	ID3DBlob* compiled_code = null;
	ID3DBlob* error = null;
	HRESULT hr = S_OK;

	collections::vector<D3D11_INPUT_ELEMENT_DESC> desc_list;

	entry = !shader_info.entry.is_empty()? shader_info.entry.get() : astring("main"_s).get();

	if (shader_info.code.is_empty())
	{
		if (shader_info.file.is_empty())
			return "d3d11_shaders::load_vertex_shader: invalid parameters"_r;

		core::files::input_text_file_t file;
		driver->get_file_system()->open(shader_info.file, &file); //file = new core::files::input_text_file(_filename);
		if (!file->is_valid())
			return "d3d11_shaders::load_vertex_shader: invalid file name"_r;

		if (!file->map([&](ibuffer_view_t code)
		{
			astring driver_type = "HLSL"_s;
			collections::vector<D3D_SHADER_MACRO> macros;
			macros += {(castr_t)driver_type, null};		
			if (!shader_info.compile_config.is_empty())
			{
				configs = shader_info.compile_config->split("-D");
				for (string& value : configs) {
					macros += {cstr_t(value).cstr<text::encoding::ascii>(), null};
				}		
			}
			macros += D3D_SHADER_MACRO{ null, null };
			if (FAILED(hr = D3DCompile(code->buffer_ptr(), code->buffer_size()
				, NULL, macros.is_empty() ? NULL : macros->data(), NULL, (castr_t)entry
				, "vs_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &compiled_code, &error
			))) return false;
		})) {
			castr_t log = (const char*)error->GetBufferPointer();
			return log;
		}
	}
	else
	{
		astring driver_type = "HLSL"_s;
		collections::vector<D3D_SHADER_MACRO> macros;
		macros += {(castr_t)driver_type, null};
		if (!shader_info.compile_config.is_empty())
		{
			configs = shader_info.compile_config->split("-D");
			for (string& value : configs) {
				macros += {cstr_t(value).cstr<text::encoding::ascii>(), null};
			}			
		}
		macros += D3D_SHADER_MACRO{ null, null };
		if (FAILED(hr = D3DCompile(shader_info.code->cstr(), shader_info.code->length()
			, NULL, macros.is_empty() ? NULL : macros->data(), NULL, (castr_t)entry
			, "vs_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &compiled_code, &error
		))) {
			castr_t log = (const char*)error->GetBufferPointer();
			return log;
		}
	}

	reflect::uniform_fields_t uniforms = null;
	com_wrapper<ID3D11ShaderReflection> vertexShaderReflection = null;
	if (FAILED(hr = D3DReflect(
		compiled_code->GetBufferPointer(),
		compiled_code->GetBufferSize(),
		__uuidof(ID3D11ShaderReflection),
		(void**)(ID3D11ShaderReflection**)&vertexShaderReflection
	))) {
		_com_error error(hr, null);
		cwstr_t log = error.ErrorMessage();
		return log;
	}

	d3d_reflect_shader_input(m_input_layout, vertexShaderReflection);
	d3d_reflect_shader_uniforms(uniforms, vertexShaderReflection);
	for (reflect::varying_desc_t& desc : uniforms)
		load_vs_const_buffer(driver, desc);


	d3d_load_shader_input(m_input_layout, desc_list);


	driver->D3D11Device()->CreateVertexShader(compiled_code->GetBufferPointer(), compiled_code->GetBufferSize(), NULL, &m_d3d_vertex_shader);

	if (desc_list->is_empty() ||
		(FAILED(hr = driver->D3D11Device()->CreateInputLayout(desc_list->data(), desc_list->size(), compiled_code->GetBufferPointer(), compiled_code->GetBufferSize(), &m_d3d_input_layout))))
	{
		m_d3d_vertex_shader = null;
		compiled_code->Release();

		_com_error error(hr, null);
		cwstr_t log = error.ErrorMessage();
		return log;
	}

	compiled_code->Release();
	return null;
}

string d3d11_shaders::load_pixel_shader(d3d11_driver_t driver, string shader_info)
{
	astring entry = "main"_s;
	string filename = null;
	astring config = null;
	astring code = null;
	collections::ienum_ptr<string> configs;
	collections::vector<D3D11_INPUT_ELEMENT_DESC> desc_list;
	ID3DBlob* compiled_code;
	HRESULT hr;

	auto list = shader_info->split(":"_r);
	int c = 0;
	for (string const& str : list) {
		switch (c)
		{
		case 0: filename = str; break;
		case 1:  if (!str.is_empty()) entry = (cstr_t)str; break;
		case 2: config = (cstr_t)str; break;
		}
		c++;
	}

	m_d3d_ps_const_buffers->clear();

	if (filename.is_empty())
		return "d3d11_shaders::load_pixel_shader: invalid parameters"_r;

	core::files::input_text_file_t file;
	driver->get_file_system()->open(filename, &file);
	if (!file->is_valid())
		return "d3d11_shaders::load_pixel_shader: invalid file name"_r;

	astring driver_type = "HLSL"_s;
	collections::vector<D3D_SHADER_MACRO> macros;
	macros += {(castr_t)driver_type, null};
	if (!config.is_empty())
	{
		configs = ang::move(config->split("-D"));
		for (string& value : configs) {
			macros += { cstr_t(value).cstr<text::encoding::ascii>(), null};
		}
	}
	macros += D3D_SHADER_MACRO{ null, null };
	ID3DBlob* error;
	if (FAILED(hr = D3DCompile(code->cstr(), code->length()
		, NULL, macros.is_empty() ? NULL : macros->data(), NULL, (castr_t)entry
		, "ps_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &compiled_code, &error
	))) {
		castr_t log = (const char*)error->GetBufferPointer();
		return log;
	}

	reflect::uniform_fields_t uniforms = null;
	com_wrapper<ID3D11ShaderReflection> pixelShaderReflection = null;
	if (FAILED(hr = D3DReflect(
		compiled_code->GetBufferPointer(),
		compiled_code->GetBufferSize(),
		__uuidof(ID3D11ShaderReflection),
		(void**)(ID3D11ShaderReflection**)&pixelShaderReflection
	))) {
		_com_error error(hr, null);
		cwstr_t log = error.ErrorMessage();
		return log;
	}

	d3d_reflect_shader_uniforms(uniforms, pixelShaderReflection);
	for (reflect::varying_desc_t& desc : uniforms)
		load_ps_const_buffer(driver, desc);

	driver->D3D11Device()->CreatePixelShader(compiled_code->GetBufferPointer(), compiled_code->GetBufferSize(), NULL, &m_d3d_pixel_shader);

	compiled_code->Release();
	return null;
}

string d3d11_shaders::load_pixel_shader(d3d11_driver_t driver, shader_info_t const& shader_info)
{
	astring entry = null;
	collections::ienum_ptr<string> configs;
	ID3DBlob* compiled_code = null;
	ID3DBlob* error = null;
	HRESULT hr = S_OK;

	collections::vector<D3D11_INPUT_ELEMENT_DESC> desc_list;

	entry = !shader_info.entry.is_empty() ? shader_info.entry.get() : astring("main"_s).get();

	if (shader_info.code.is_empty())
	{
		if (shader_info.file.is_empty())
			return "d3d11_shaders::load_pixel_shader: invalid parameters"_r;

		core::files::input_text_file_t file;
		driver->get_file_system()->open(shader_info.file, &file);
		if (!file->is_valid())
			return "d3d11_shaders::load_pixel_shader: invalid file name"_r;

		if (!file->map([&](ibuffer_view_t code)
		{
			astring driver_type = "HLSL"_s;
			collections::vector<D3D_SHADER_MACRO> macros;
			macros += {(castr_t)driver_type, null};
			if (!shader_info.compile_config.is_empty())
			{
				configs = shader_info.compile_config->split("-D");
				for (string& value : configs) {
					macros += {cstr_t(value).cstr<text::encoding::ascii>(), null};
				}			
			}
			macros += D3D_SHADER_MACRO{ null, null };
			if (FAILED(hr = D3DCompile(code->buffer_ptr(), code->buffer_size()
				, NULL, macros.is_empty() ? NULL : macros->data(), NULL, (castr_t)entry
				, "ps_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &compiled_code, &error
			))) return false;
		})) {
			castr_t log = (const char*)error->GetBufferPointer();
			return log;
		}
	}
	else
	{
		astring driver_type = "HLSL"_s;
		collections::vector<D3D_SHADER_MACRO> macros;
		macros += {(castr_t)driver_type, null};
		if (!shader_info.compile_config.is_empty())
		{
			configs = shader_info.compile_config->split("-D");
			for (string& value : configs) {
				macros += {cstr_t(value).cstr<text::encoding::ascii>(), null};
			}
		}
		macros += D3D_SHADER_MACRO{ null, null };
		if (FAILED(hr = D3DCompile(shader_info.code->cstr(), shader_info.code->length()
			, NULL, macros.is_empty() ? NULL : macros->data(), NULL, (castr_t)entry
			, "ps_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &compiled_code, &error
		))) {
			castr_t log = (const char*)error->GetBufferPointer();
			return log;
		}
	}

	reflect::uniform_fields_t uniforms = null;
	com_wrapper<ID3D11ShaderReflection> pixelShaderReflection = null;
	if (FAILED(hr = D3DReflect(
		compiled_code->GetBufferPointer(),
		compiled_code->GetBufferSize(),
		__uuidof(ID3D11ShaderReflection),
		(void**)(ID3D11ShaderReflection**)&pixelShaderReflection
	))) {
		_com_error error(hr, null);
		cwstr_t log = error.ErrorMessage();
		return log;
	}

	d3d_reflect_shader_uniforms(uniforms, pixelShaderReflection);
	for (reflect::varying_desc_t& desc : uniforms)
		load_ps_const_buffer(driver, desc);

	driver->D3D11Device()->CreatePixelShader(compiled_code->GetBufferPointer(), compiled_code->GetBufferSize(), NULL, &m_d3d_pixel_shader);

	compiled_code->Release();
	return null;
}

bool d3d11_shaders::load_vs_const_buffer(d3d11_driver_t driver, graphics::reflect::varying_desc& uniforms)
{
	com_wrapper<ID3D11Buffer> buffer;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = uniforms.get_size_in_bytes();
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	//D3D11_SUBRESOURCE_DATA InitData;
	HRESULT hr = driver->D3D11Device()->CreateBuffer(&bd, NULL, &buffer);
	if (FAILED(hr))
		return false;

	m_d3d_vs_const_buffers += buffer;
	m_vs_uniforms += ang::move(uniforms);
	return false;
}

bool d3d11_shaders::load_ps_const_buffer(d3d11_driver_t driver, graphics::reflect::varying_desc& uniforms)
{
	com_wrapper<ID3D11Buffer> buffer;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = uniforms.get_size_in_bytes();
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	//D3D11_SUBRESOURCE_DATA InitData;
	HRESULT hr = driver->D3D11Device()->CreateBuffer(&bd, NULL, &buffer);
	if (FAILED(hr))
		return false;

	m_d3d_ps_const_buffers += buffer;
	m_ps_uniforms += ang::move(uniforms);
	return false;
}

//bool d3d11_shaders::load_ps_samplers(d3d11_driver_t driver, dom::xml::xml_node_t samplers)
//{
//	if (!samplers->xml_has_children())
//		return false;
//
//	D3D11_SAMPLER_DESC sampDesc;
//	ZeroMemory(&sampDesc, sizeof(sampDesc));
//	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
//	sampDesc.MinLOD = 0;
//	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
//	sampDesc.MaxAnisotropy = 1;
//
//	for(dom::xml::xml_node_t sampler : samplers->xml_children())
//	{
//		if ((text::raw_cstr)sampler->xml_name() == "sampler"_s)
//		{
//			auto att = sampler->xml_attributes();
//			auto _mode = att["wrap"_s]->xml_as<textures::tex_wrap_mode_t>();
//			//auto _type = att["type"];
//			D3D11_TEXTURE_ADDRESS_MODE mode = _mode.get() == textures::tex_wrap_mode::clamp ? D3D11_TEXTURE_ADDRESS_CLAMP
//				: _mode.get() == textures::tex_wrap_mode::mirrored ? D3D11_TEXTURE_ADDRESS_MIRROR
//				: D3D11_TEXTURE_ADDRESS_WRAP;
//			sampDesc.AddressU = mode;
//			sampDesc.AddressV = mode;
//			sampDesc.AddressW = mode;
//			sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//=_type...
//			ID3D11SamplerState* sampler_state;
//			if (SUCCEEDED(driver->driver()->D3D11Device()->CreateSamplerState(&sampDesc, &sampler_state)))
//			{
//				m_d3d_ps_samplers += sampler_state;
//				sampler_state->Release();
//			}
//		}
//	}
//	return true;
//}

bool d3d11_shaders::use_shaders(d3d11_driver_t driver)
{
	driver->D3D11Context()->IASetInputLayout(m_d3d_input_layout);
	driver->D3D11Context()->VSSetShader(m_d3d_vertex_shader, NULL, 0);
	driver->D3D11Context()->PSSetShader(m_d3d_pixel_shader, NULL, 0);

	if (!m_d3d_vs_const_buffers.is_empty() && !m_d3d_vs_const_buffers->is_empty())
		driver->D3D11Context()->VSSetConstantBuffers(0, m_d3d_vs_const_buffers->counter(), (ID3D11Buffer**)m_d3d_vs_const_buffers->data());
	else
		driver->D3D11Context()->VSSetConstantBuffers(0, 0, null);
	
	if (!m_d3d_ps_const_buffers.is_empty() && !m_d3d_ps_const_buffers->is_empty())
		driver->D3D11Context()->PSSetConstantBuffers(0, m_d3d_ps_const_buffers->counter(), (ID3D11Buffer**)m_d3d_ps_const_buffers->data());
	else
		driver->D3D11Context()->PSSetConstantBuffers(0, 0, null);

	if (!m_d3d_ps_samplers.is_empty() && !m_d3d_ps_samplers->is_empty())
		driver->D3D11Context()->PSSetSamplers(0, m_d3d_ps_samplers->counter(), (ID3D11SamplerState**)m_d3d_ps_samplers->data());
	else 
		driver->D3D11Context()->PSSetSamplers(0, 0, null);
	
	return true;
}

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

array_view<string> d3d11_shaders::textures()const
{
	return m_d3d_textures.is_empty() ? array_view<string>() : (array_view<string>)m_d3d_textures;
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

reflect::varying d3d11_shaders::map_vs_uniform(idriver_t, cstr_t name)
{
	if (m_vs_uniforms.is_empty())
		return reflect::varying();
	index i = 0;
	for (auto it = m_vs_uniforms->begin(); it.is_valid(); ++it, ++i)
	{
		if ((cstr_t)it->var_name() == name)
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

reflect::varying d3d11_shaders::map_ps_uniform(idriver_t, cstr_t name)
{
	if (m_ps_uniforms.is_empty())
		return reflect::varying();
	index i = 0;
	for (auto it = m_ps_uniforms->begin(); it.is_valid(); ++it, ++i)
	{
		if ((cstr_t)it->var_name() == name)
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


//////////////////////////////////////////////////////////////////

#endif
