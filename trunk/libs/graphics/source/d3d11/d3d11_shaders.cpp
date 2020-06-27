#include "pch.h"
#include "d3d11/d3d11_driver.h"

#if DIRECTX_SUPPORT

#include <comdef.h>

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;
using namespace ang::graphics::effects;

/////////////////////////////////////////////////////////////////////////

bool d3d11::d3d_load_shader_input(collections::array_view<graphics::reflect::attribute_desc> const& attributes, collections::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout_desc)
{
	input_layout_desc.clear();
	D3D11_INPUT_ELEMENT_DESC desc;
	memset(&desc, 0, sizeof(desc));

	int i = 0;
	wsize temp = 0;
	wsize res = 0;
	static const wsize aligment = 16u;

	for(graphics::reflect::attribute_desc const& att : attributes)
	{
		desc.SemanticName = (const char*)(castr_t)att.semantic().to_string();
		desc.SemanticIndex = att.semantic_index();
		desc.InputSlot = i++;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;
		desc.AlignedByteOffset = 0;

		switch (att.var_type().get())
		{
		case graphics::reflect::var_type::f32:
			switch (att.var_class().get())
			{
			case graphics::reflect::var_class::vec4:
				desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			case graphics::reflect::var_class::vec3:
				desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			case graphics::reflect::var_class::vec2:
				desc.Format = DXGI_FORMAT_R32G32_FLOAT;;
				break;
			case graphics::reflect::var_class::scalar:
				desc.Format = DXGI_FORMAT_R32_FLOAT;
				break;
			default:
				break;
			}
			break;
		case graphics::reflect::var_type::s32:
			switch (att.var_class().get())
			{
			case graphics::reflect::var_class::vec4:
				desc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				break;
			case graphics::reflect::var_class::vec3:
				desc.Format = DXGI_FORMAT_R32G32B32_SINT;
				break;
			case graphics::reflect::var_class::vec2:
				desc.Format = DXGI_FORMAT_R32G32_SINT;
				break;
			case graphics::reflect::var_class::scalar:
				desc.Format = DXGI_FORMAT_R32_SINT;
				break;
			default:
				break;
			}
			break;
		case graphics::reflect::var_type::u32:
			switch (att.var_class().get())
			{
			case graphics::reflect::var_class::vec4:
				desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				break;
			case graphics::reflect::var_class::vec3:
				desc.Format = DXGI_FORMAT_R32G32B32_UINT;
				break;
			case graphics::reflect::var_class::vec2:
				desc.Format = DXGI_FORMAT_R32G32_UINT;
				break;
			case graphics::reflect::var_class::scalar:
				desc.Format = DXGI_FORMAT_R32_UINT;
				break;
			default:
				break;
			}
			break;
		default:
			break;
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
	D3D11_SHADER_INPUT_BIND_DESC resDesc;
	vertexShaderReflection->GetResourceBindingDesc(0, &resDesc);

	for (uint i = 0; i < shaderDesc.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;

		vertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

		if (paramDesc.SemanticName == "SV_InstanceID"_r)
			continue;

		attribute.semantic(var_semantic_t::parse(castr_t(paramDesc.SemanticName, invalid_index)));
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

bool d3d11::d3d_reflect_shader_variable(reflect::varying_desc_t& field, astring name, wsize offset, ID3D11ShaderReflectionType* varInfo)
{
	using namespace reflect;

	field.var_name(name);
	field.position(offset);
	
	D3D11_SHADER_TYPE_DESC typeDesc;
	varInfo->GetDesc(&typeDesc);
	field.array_count(max(1, typeDesc.Elements));

	switch (typeDesc.Class)
	{
	case D3D_SVC_STRUCT: {
		field.var_class(var_class::scalar);
		field.var_type(var_type::block);
		reflect::uniform_fields_t fields;

		for (windex i = 0; i < typeDesc.Members; i++) {
			reflect::varying_desc_t child;
			d3d_reflect_shader_variable(child, castr_t(varInfo->GetMemberTypeName(i), invalid_index), 0, varInfo->GetMemberTypeByIndex(i));
			fields += move(child);
		}
		field.fields(ang::move(fields), true);
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

	

	return true;
}

bool d3d11::d3d_reflect_shader_uniforms(reflect::uniform_fields_t& uniforms, ID3D11ShaderReflection* vertexShaderReflection)
{
	using namespace reflect;

	ID3D11ShaderReflectionConstantBuffer* cb = nullptr;
	D3D11_SHADER_DESC reflectDesc;
	vertexShaderReflection->GetDesc(&reflectDesc);

	for (windex i = 0; i < reflectDesc.ConstantBuffers; ++i)
	{
		cb = vertexShaderReflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC desc;
		cb->GetDesc(&desc);
		reflect::varying_desc_t cbuffer;
		cbuffer.var_class(var_class::scalar);
		cbuffer.var_type(var_type::block);
		cbuffer.array_count(1);
		reflect::uniform_fields_t fields;
		for (windex j = 0; j < desc.Variables; j++)
		{
			D3D11_SHADER_VARIABLE_DESC varDesc;
			reflect::varying_desc_t field;
			auto var = cb->GetVariableByIndex(j);
			var->GetDesc(&varDesc);
			d3d_reflect_shader_variable(field, castr_t(varDesc.Name, invalid_index), varDesc.StartOffset, var->GetType());
			fields += ang::move(field);
		}
		cbuffer.fields(ang::move(fields), false);
		cbuffer.aligment(16);
		uniforms += ang::move(cbuffer);
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////

optional<d3d11_shader_code_t> d3d11_driver::compile_shader(string info, shader_type_t const& type)
{
	auto it = m_compiled_shaders.find(info);
	if (it.is_valid())
		return it->value;
	error err;
	d3d11_shader_code_t code = new d3d11_shader_code();
	if ((err = code->load(this, info, type)).code() != error_code::success)
		return err;	
	m_compiled_shaders[info] = code;
	return code;
}

optional<d3d11_shader_code_t> d3d11_driver::compile_shader(effects::shader_info_t const& info, shader_type_t const& type)
{
	string sinfo = "";
	sinfo->append(info.file);
	
	switch (type)
	{
	case shader_type::vertex:
		sinfo->append(";vs;"_r);
		break;
	case shader_type::pixel:
		sinfo->append(";ps;"_r);
		break;
	case shader_type::geometry:
		sinfo->append(";gs;"_r);
		break;
	default:
		sinfo->append(";;"_r);
		break;
	}
	sinfo->append(info.entry);
	sinfo->append(";"_r);
	sinfo->append(info.compile_config);

	auto it = m_compiled_shaders.find(sinfo);
	if (it.is_valid())
		return it->value;
	error err;
	d3d11_shader_code_t code = new d3d11_shader_code();
	if ((err = code->load(this, info, sinfo, type)).code() != error_code::success)
		return err;
	m_compiled_shaders[sinfo] = code;
	return code;
}

/////////////////////////////////////////////////////////////////////////////////

d3d11_shader_code::d3d11_shader_code()
{

}

void d3d11_shader_code::dispose()
{
	m_shader_info = null;
	m_d3d_input_layout = null;
	m_compiled_code = null;
	m_input_layout = null;
	m_uniforms = null;
}

error d3d11_shader_code::load(d3d11_driver_t driver, string shader_info, shader_type_t const& type)
{
	astring entry = "main"_sv;
	string filename = null;
	astring config = null;
	astring code = null;
	vector<astring> configs;
	vector<D3D11_INPUT_ELEMENT_DESC> desc_list;
	HRESULT hr;

	auto list = shader_info->split(";"_r);
	int c = 0;
	for (string const& str : list) {
		switch (c)
		{
		case 0: filename = str; break;
		case 1: 
			if (str == "vs" && type != shader_type::vertex) return false;
			if (str == "ps" && type != shader_type::pixel) return false;
			if (str == "gs") return false; //TODO:
			break;
		case 2:  if (!str.is_empty()) entry = (cstr_t)str; break;
		case 3: config = (cstr_t)str; break;
		}
		c++;
	}
	m_input_layout.clear();
	m_uniforms.clear();

	if (filename.is_empty())
		return "d3d11_shaders::load_shader: invalid parameters"_sv;

	core::files::input_text_file_t file;
	driver->get_file_system()->open(filename, &file); //file = new core::files::input_text_file(_filename);
	if (!file->is_valid())
		return "d3d11_shaders::load_shader: invalid file name"_sv;

	astring driver_type = "HLSL"_sv;
	vector<D3D_SHADER_MACRO> macros;
	macros += { (castr_t)driver_type, null};
	if (!config.is_empty())
	{
		configs = ang::move(config.split("-D"_sv));
		for (astring& value : configs) {
			macros += { (castr_t)value, null};
		}
	}
	macros += D3D_SHADER_MACRO{ null, null };
	ID3DBlob* error;
	if (FAILED(hr = D3DCompile((castr_t)code, code.length()
		, NULL, macros.is_empty() ? NULL : macros.data(), NULL, (castr_t)entry
		, type == shader_type::vertex ? driver->vs_model() : driver->ps_model()
		, D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &m_compiled_code, &error
	))) {
		castr_t log((const char*)error->GetBufferPointer(), error->GetBufferSize());
		return log;
	}

	com_wrapper<ID3D11ShaderReflection> shaderReflection = null;
	if (FAILED(hr = D3DReflect(
		m_compiled_code->GetBufferPointer(),
		m_compiled_code->GetBufferSize(),
		__uuidof(ID3D11ShaderReflection),
		(void**)(ID3D11ShaderReflection**)&shaderReflection
	))) {
		_com_error error(hr, null);
		castr_t log(error.ErrorMessage(), invalid_index);
		return log;
	}

	d3d_reflect_shader_uniforms(m_uniforms, shaderReflection);

	if (type == shader_type::vertex)
	{
		d3d_reflect_shader_input(m_input_layout, shaderReflection);
		d3d_load_shader_input(m_input_layout, desc_list);

		if (desc_list.is_empty() ||
			(FAILED(hr = driver->D3D11Device()->CreateInputLayout(desc_list.data(), desc_list.size(), m_compiled_code->GetBufferPointer(), m_compiled_code->GetBufferSize(), &m_d3d_input_layout))))
		{
			_com_error error(hr, null);
			castr_t log = castr_t(error.ErrorMessage(), invalid_index);
			return log;
		}
	}
	return error_code::success;
}

error d3d11_shader_code::load(d3d11_driver_t driver, effects::shader_info_t const& shader_info, string sinfo, shader_type_t const& type)
{
	astring entry = null;
	vector<astring> configs;
	ID3DBlob* error = null;
	HRESULT hr = S_OK;
	m_shader_info = sinfo;

	vector<D3D11_INPUT_ELEMENT_DESC> desc_list;
	entry = move(!shader_info.entry.is_empty() ? shader_info.entry : astring("main"_sv));

	if (shader_info.code.is_empty())
	{
		if (shader_info.file.is_empty())
			return "d3d11_shaders::load_vertex_shader: invalid parameters"_sv;

		core::files::input_text_file_t file;
		driver->get_file_system()->open(shader_info.file, &file); //file = new core::files::input_text_file(_filename);
		if (file.is_empty() || !file->is_valid())
			return "d3d11_shaders::load_vertex_shader: invalid file name"_sv;

		if (!file->map([&](ibuffer_view_t code)
		{
			astring driver_type = "HLSL"_sv;
			vector<D3D_SHADER_MACRO> macros = null;
			macros += { (castr_t)driver_type, null };
			if (!shader_info.compile_config.is_empty())
			{
				configs = shader_info.compile_config.split("-D"_sv);
				for (astring& value : configs) {
					macros += { castr_t(value), null };
				}
			}
			macros += D3D_SHADER_MACRO{ null, null };
			if (FAILED(hr = D3DCompile(code->buffer_ptr(), code->buffer_size()
				, NULL, macros.data(), NULL, (castr_t)entry
				, type == shader_type::vertex ? driver->vs_model() : driver->ps_model()
				, D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &m_compiled_code, &error
			))) return false;
			return true;
		})) {
			return error ? castr_t((const char*)error->GetBufferPointer(), error->GetBufferSize()) : "d3d11_shaders::load_vertex_shader: unknown error"_sv;
		}
	}
	else
	{
		astring driver_type = "HLSL"_sv;
		collections::vector<D3D_SHADER_MACRO> macros;
		macros += {(castr_t)driver_type, null};
		if (!shader_info.compile_config.is_empty())
		{
			configs = shader_info.compile_config.split("-D"_sv);
			for (astring& value : configs) {
				macros += {castr_t(value), null};
			}
		}
		macros += D3D_SHADER_MACRO{ null, null };
		if (FAILED(hr = D3DCompile(shader_info.code.cstr(), shader_info.code.length()
			, NULL, macros.data(), NULL, (castr_t)entry
			, type == shader_type::vertex ? driver->vs_model() : driver->ps_model()
			, D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &m_compiled_code, &error
		))) {
			castr_t log = castr_t((const char*)error->GetBufferPointer(), error->GetBufferSize());
			return log;
		}
	}
	com_wrapper<ID3D11ShaderReflection> shaderReflection = null;
	if (FAILED(hr = D3DReflect(
		m_compiled_code->GetBufferPointer(),
		m_compiled_code->GetBufferSize(),
		__uuidof(ID3D11ShaderReflection),
		(void**)(ID3D11ShaderReflection**)&shaderReflection
	))) {
		_com_error error(hr, null);
		castr_t log(error.ErrorMessage(), invalid_index);
		return log;
	}

	d3d_reflect_shader_uniforms(m_uniforms, shaderReflection);

	if (type == shader_type::vertex)
	{
		d3d_reflect_shader_input(m_input_layout, shaderReflection);
		d3d_load_shader_input(m_input_layout, desc_list);

		if (desc_list.is_empty() ||
			(FAILED(hr = driver->D3D11Device()->CreateInputLayout(desc_list.data(), desc_list.size(), m_compiled_code->GetBufferPointer(), m_compiled_code->GetBufferSize(), &m_d3d_input_layout))))
		{
			_com_error error(hr, null);
			castr_t log = castr_t(error.ErrorMessage(), invalid_index);
			return log;
		}
	}
	return error_code::success;
}

string d3d11_shader_code::shader_info()const
{
	return m_shader_info;
}

ID3D11InputLayout* d3d11_shader_code::input_layout()const
{
	return m_d3d_input_layout.get();
}

ID3DBlob* d3d11_shader_code::compiled_code()const
{
	return m_compiled_code.get();
}

array<reflect::attribute_desc> d3d11_shader_code::attributes()const
{
	return array<reflect::attribute_desc>(m_input_layout.begin(), m_input_layout.end());
}

array<reflect::varying_desc> d3d11_shader_code::uniforms()const
{
	return array<reflect::varying_desc>(m_uniforms.begin(), m_uniforms.end());
}

/////////////////////////////////////////////////////////////////////////////////

d3d11_shaders::d3d11_shaders()
{
}

void d3d11_shaders::dispose()
{
	close();
}

resources::iresource_t d3d11_shaders::resource()const { return const_cast<d3d11_shaders*>(this); }

resources::resource_type_t d3d11_shaders::resource_type()const { return resources::resource_type::shaders; }

string d3d11_shaders::resource_sid()const { return (cstr_t)m_resource_sid; }

void d3d11_shaders::resource_sid(cstr_t sid) { m_resource_sid = sid; }

intfptr d3d11_shaders::fast_cast(resources::resource_type_t type) {
	return type == resources::resource_type::shaders ? static_cast<effects::ishaders*>(this) : null;
}

error d3d11_shaders::load(d3d11_driver_t driver, string vertex, string pixel, string sid)
{
	error err;
	if (vertex.is_empty() || pixel.is_empty())
		return error_code::invalid_param;
	if ((err = load_vertex_shader(driver, vertex)).code() != error_code::success) {
		return err;
	}
	if ((err = load_pixel_shader(driver, pixel)).code() != error_code::success) {
		return err;
	}
	m_resource_sid = sid;
	return error_code::success;
}

error d3d11_shaders::load(d3d11_driver_t driver, shader_info_t const& vertex, shader_info_t const& pixel, string sid)
{
	error err;
	if ((err = load_vertex_shader(driver, vertex)).code() != error_code::success) {
		return err;
	}
	if ((err = load_pixel_shader(driver, pixel)).code() != error_code::success) {
		return err;
	}
	m_resource_sid = sid;
	return error_code::success;
}

bool d3d11_shaders::close()
{
	m_resource_sid = null;
	m_input_layout = null;
	m_vs_uniforms = null;
	m_ps_uniforms = null;
	m_d3d_textures = null;

	m_d3d_pixel_shader = null;
	m_d3d_vertex_shader = null;
	m_d3d_input_layout = null;

	m_d3d_vs_const_buffers = null;
	m_d3d_ps_const_buffers = null;
	m_d3d_ps_samplers = null;
	return true;
}

error d3d11_shaders::load_vertex_shader(d3d11_driver_t driver, string shader_info)
{
	auto code = driver->compile_shader(shader_info, shader_type::vertex);
	if (code.failed())
		return code.error();

	driver->D3D11Device()->CreateVertexShader(code->compiled_code()->GetBufferPointer(), code->compiled_code()->GetBufferSize(), NULL, &m_d3d_vertex_shader);
	m_input_layout.copy((array_view<reflect::attribute_desc_t> const&)code->attributes());
	m_d3d_input_layout = code->input_layout();

	for (reflect::varying_desc_t  const& desc : code->uniforms())
		load_vs_const_buffer(driver, desc);

	return error_code::success;
}

error d3d11_shaders::load_vertex_shader(d3d11_driver_t driver, shader_info_t const& shader_info)
{
	auto code = driver->compile_shader(shader_info, shader_type::vertex);
	if (code.failed())
		return code.error();

	driver->D3D11Device()->CreateVertexShader(code->compiled_code()->GetBufferPointer(), code->compiled_code()->GetBufferSize(), NULL, &m_d3d_vertex_shader);
	m_input_layout.copy((array_view<reflect::attribute_desc_t> const&)code->attributes());
	m_d3d_input_layout = code->input_layout();

	for (reflect::varying_desc_t const& desc : code->uniforms())
		load_vs_const_buffer(driver, desc);

	return error_code::success;
}

error d3d11_shaders::load_pixel_shader(d3d11_driver_t driver, string shader_info)
{
	auto code = driver->compile_shader(shader_info, shader_type::pixel);
	if (code.failed())
		return code.error();

	driver->D3D11Device()->CreatePixelShader(code->compiled_code()->GetBufferPointer(), code->compiled_code()->GetBufferSize(), NULL, &m_d3d_pixel_shader);

	for (reflect::varying_desc_t const& desc : code->uniforms())
		load_ps_const_buffer(driver, desc);

	return error_code::success;
}

error d3d11_shaders::load_pixel_shader(d3d11_driver_t driver, shader_info_t const& shader_info)
{
	auto code = driver->compile_shader(shader_info, shader_type::pixel);
	if (code.failed())
		return code.error();

	driver->D3D11Device()->CreatePixelShader(code->compiled_code()->GetBufferPointer(), code->compiled_code()->GetBufferSize(), NULL, &m_d3d_pixel_shader);

	for (reflect::varying_desc_t const& desc : code->uniforms())
		load_ps_const_buffer(driver, desc);

	return error_code::success;
}

bool d3d11_shaders::load_vs_const_buffer(d3d11_driver_t driver, graphics::reflect::varying_desc  const& uniforms)
{
	com_wrapper<ID3D11Buffer> buffer;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = align_up(16, uniforms.get_size_in_bytes());
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	//D3D11_SUBRESOURCE_DATA InitData;
	HRESULT hr = driver->D3D11Device()->CreateBuffer(&bd, NULL, &buffer);
	if (FAILED(hr))
		return false;

	m_d3d_vs_const_buffers += buffer;
	m_vs_uniforms += uniforms;
	return false;
}

bool d3d11_shaders::load_ps_const_buffer(d3d11_driver_t driver, graphics::reflect::varying_desc  const& uniforms)
{
	com_wrapper<ID3D11Buffer> buffer;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = align_up(16, uniforms.get_size_in_bytes());
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	//D3D11_SUBRESOURCE_DATA InitData;
	HRESULT hr = driver->D3D11Device()->CreateBuffer(&bd, NULL, &buffer);
	if (FAILED(hr))
		return false;

	m_d3d_ps_const_buffers += buffer;
	m_ps_uniforms += uniforms;
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

	if (!m_d3d_vs_const_buffers.is_empty())
		driver->D3D11Context()->VSSetConstantBuffers(0, m_d3d_vs_const_buffers.size(), (ID3D11Buffer**)m_d3d_vs_const_buffers.data());
	else
		driver->D3D11Context()->VSSetConstantBuffers(0, 0, null);
	
	if (!m_d3d_ps_const_buffers.is_empty())
		driver->D3D11Context()->PSSetConstantBuffers(0, m_d3d_ps_const_buffers.size(), (ID3D11Buffer**)m_d3d_ps_const_buffers.data());
	else
		driver->D3D11Context()->PSSetConstantBuffers(0, 0, null);

	if (!m_d3d_ps_samplers.is_empty())
		driver->D3D11Context()->PSSetSamplers(0, m_d3d_ps_samplers.size(), (ID3D11SamplerState**)m_d3d_ps_samplers.data());
	else 
		driver->D3D11Context()->PSSetSamplers(0, 0, null);
	
	return true;
}

array_view<reflect::attribute_desc> const& d3d11_shaders::input_layout()const
{
	return m_input_layout;
}

array_view<reflect::varying_desc>const& d3d11_shaders::vs_uniforms_layouts()const
{
	return m_vs_uniforms;
}

array_view<reflect::varying_desc>const& d3d11_shaders::ps_uniforms_layouts()const
{
	return m_ps_uniforms;
}

bool d3d11_shaders::bind_vertex_buffer(idriver_t, buffers::ivertex_buffer_t)
{
	//TODO: adjust input layout
	return false;
}

bool d3d11_shaders::bind_texture(idriver_t, windex, windex)
{
	//nothing to do
	return false;
}

bool d3d11_shaders::bind_texture(idriver_t, cstr_t, windex)
{
	//nothing to do
	return false;
}


reflect::varying d3d11_shaders::map_vs_uniform(idriver_t, windex idx)
{
	if (m_vs_uniforms.is_empty() || m_vs_uniforms.size() <= idx)
		return reflect::varying();

	reflect::varying_desc desc = m_vs_uniforms[idx];
	wsize size = desc.get_size_in_bytes();
	if(size == 0)
		return reflect::varying();

	auto data = memory::aligned16_allocator<byte>().allocate(size);
	array_view<byte> buff = to_array(data, data + size);
	memset(buff.get(), 0, size);
	desc.position(idx);
	return reflect::varying(buff, ang::move(desc));
}

reflect::varying d3d11_shaders::map_vs_uniform(idriver_t, cstr_t name)
{
	if (m_vs_uniforms.is_empty())
		return reflect::varying();
	windex i = 0;
	for (auto const& var : m_vs_uniforms)
	{
		if (var.var_name() == name)
		{
			reflect::varying_desc desc = var;
			wsize size = desc.get_size_in_bytes();
			if (size == 0)
				return reflect::varying();
			auto data = memory::aligned16_allocator<byte>().allocate(size);
			array_view<byte> buff = to_array(data, data + size);
			memset(buff.get(), 0, size);
			desc.position(i);
			return reflect::varying(buff, desc);
		}
		i++;
	}
	return reflect::varying();
}

reflect::varying d3d11_shaders::map_ps_uniform(idriver_t, windex idx)
{
	if (m_ps_uniforms.is_empty() || m_ps_uniforms.size() <= idx)
		return reflect::varying();

	reflect::varying_desc desc = m_ps_uniforms[idx];
	wsize size = desc.get_size_in_bytes();
	if (size == 0)
		return reflect::varying();
	auto data = memory::aligned16_allocator<byte>().allocate(size);
	array_view<byte> buff = to_array(data, data + size);
	memset(buff.get(), 0, size);
	desc.position(idx);
	return reflect::varying(buff, desc);
}

reflect::varying d3d11_shaders::map_ps_uniform(idriver_t, cstr_t name)
{
	if (m_ps_uniforms.is_empty())
		return reflect::varying();
	windex i = 0;
	for (auto const& var : m_ps_uniforms)
	{
		if (var.var_name() == name)
		{
			reflect::varying_desc desc = var;
			wsize size = desc.get_size_in_bytes();
			if (size == 0)
				return reflect::varying();
			auto data = memory::aligned16_allocator<byte>().allocate(size);
			array_view<byte> buff = to_array(data, data + size);
			memset(buff.get(), 0, size);
			desc.position(i);
			return reflect::varying(buff, desc);
		}
		i++;
	}
	return reflect::varying();
}

void d3d11_shaders::unmap_vs_uniform(idriver_t _driver, reflect::varying& var)
{
	auto driver = static_cast<d3d11_driver*>(_driver.get());
	auto desc = var.descriptor();
	windex idx = desc.position();
	auto data = var.raw_data();
	if (m_vs_uniforms.is_empty() || m_vs_uniforms.size() <= idx || data.get() == null)
		throw exception(error_code::invalid_param);
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
	windex idx = desc.position();
	auto data = var.raw_data();
	if (m_ps_uniforms.is_empty() || m_ps_uniforms.size() <= idx || data.get() == null)
		throw exception(error_code::invalid_param);
	driver->execute_on_thread_safe([&]() {
		driver->D3D11Context()->UpdateSubresource(m_d3d_ps_const_buffers[idx].get(), 0, NULL, data.get(), 0, 0);
	});
	memory::aligned16_allocator<byte>().deallocate(data.get());
	var = reflect::varying();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
