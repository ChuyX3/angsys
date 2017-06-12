#include "stdafx.h"
#include "EffectData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace AngineEditor;

CTechnique::CTechnique() {}

CTechnique::CTechnique(const CTechnique& other) {
	name = other.name;
	vsEntry = other.vsEntry;
	psEntry = other.psEntry;
	vsSource = other.vsSource;
	psSource = other.psSource;
	inputLayout = new collections::vector_buffer<graphics::reflect::attribute_desc>(other.inputLayout.get());
	vsUniforms = new collections::vector_buffer<graphics::reflect::variable_desc>(other.vsUniforms.get());
	psUniforms = new collections::vector_buffer<graphics::reflect::variable_desc>(other.vsUniforms.get());
	vsCompileConfig = new collections::vector_buffer<collections::pair<string, string>>(other.vsCompileConfig.get());
	psCompileConfig = new collections::vector_buffer<collections::pair<string, string>>(other.psCompileConfig.get());
	psSamplers = new collections::vector_buffer<graphics::textures::tex_wrap_mode_t>(other.psSamplers.get());
}

CTechnique::CTechnique(CTechnique&& other) {
	name = ang::move(other.name);
	vsEntry = ang::move(other.vsEntry);
	psEntry = ang::move(other.psEntry);
	vsSource = ang::move(other.vsSource);
	psSource = ang::move(other.psSource);
	inputLayout = ang::move(other.inputLayout);
	vsUniforms = ang::move(other.vsUniforms);
	psUniforms = ang::move(other.vsUniforms);
	vsCompileConfig = ang::move(other.vsCompileConfig);
	psCompileConfig = ang::move(other.psCompileConfig);
	psSamplers = ang::move(other.psSamplers);
}

CTechnique::~CTechnique() {}

ANG_IMPLEMENT_BASIC_INTERFACE(AngineEditor::CTechnique, ang::object)

CTechnique& CTechnique::operator = (const CTechnique& other) {
	name = other.name;
	vsEntry = other.vsEntry;
	psEntry = other.psEntry;
	vsSource = other.vsSource;
	psSource = other.psSource;
	inputLayout = new collections::vector_buffer<graphics::reflect::attribute_desc>(other.inputLayout.get());
	vsUniforms = new collections::vector_buffer<graphics::reflect::variable_desc>(other.vsUniforms.get());
	psUniforms = new collections::vector_buffer<graphics::reflect::variable_desc>(other.vsUniforms.get());
	vsCompileConfig = new collections::vector_buffer<collections::pair<string, string>>(other.vsCompileConfig.get());
	psCompileConfig = new collections::vector_buffer<collections::pair<string, string>>(other.psCompileConfig.get());
	psSamplers = new collections::vector_buffer<graphics::textures::tex_wrap_mode_t>(other.psSamplers.get());
	return *this;
}

CTechnique& CTechnique::operator = (CTechnique&& other) {
	name = ang::move(other.name);
	vsEntry = ang::move(other.vsEntry);
	psEntry = ang::move(other.psEntry);
	vsSource = ang::move(other.vsSource);
	psSource = ang::move(other.psSource);
	inputLayout = ang::move(other.inputLayout);
	vsUniforms = ang::move(other.vsUniforms);
	psUniforms = ang::move(other.vsUniforms);
	vsCompileConfig = ang::move(other.vsCompileConfig);
	psCompileConfig = ang::move(other.psCompileConfig);
	psSamplers = ang::move(other.psSamplers);
	return *this;
}


bool CTechnique::Load(xml::xml_node_t node)
{
	if (node.is_empty() || !node->xml_has_children())
		return false;
	bool res = true;
	name = node->xml_attributes()["name"];
	xml::foreach(node->xml_children(), [&](xml::xml_node_t node)
	{
		auto name = node->xml_name().as<cwstr_t>();
		if (name == "vertex_shader")
			res &= LoadVertexShader(node);
		else if (name == "pixel_shader")
			res &= LoadPixelShader(node);
	});

	return res;
}


bool CTechnique::LoadVertexShader(xml::xml_node_t vertex)
{
	if (!vertex->xml_has_children())
		return false;
	vsEntry = "main";
	string config;
	xml::foreach(vertex->xml_children(), [&](xml::xml_node_t node)
	{
		auto name = node->xml_name().as<cwstr_t>();
		if (name == "uniforms"_s)
			LoadVsConstBuffer(node);
		else if (name == "input_layout"_s)
			graphics::reflect::attribute_desc::load(node, inputLayout);
		else if (name == "entry"_s)
			vsEntry = node->xml_value();
		else if (name == "code"_s)
			vsSource = node->xml_value();
		else if (name == "compile_config"_s)
		{
			config = node->xml_value();
			auto configs = ang::move(config->split("-D"));
			foreach(configs, [&](string& _value) {
				cstr_t value = _value;
				auto idx = value.find("=", 0);
				if (idx != invalid_index)
				{
					vsCompileConfig += {null, null};
					value.sub_string(vsCompileConfig->end()->key_value(), 0, idx);
					value.sub_string(vsCompileConfig->end()->value(), idx + 1, value.size() - idx);
				}
				else
				{
					vsCompileConfig += {_value, null};
				}
			});
		}
			
	});

	return true;
}

bool CTechnique::LoadPixelShader(xml::xml_node_t pixel)
{
	if (!pixel->xml_has_children())
		return false;
	psEntry = "main";
	string config;
	xml::foreach(pixel->xml_children(), [&](xml::xml_node_t node)
	{
		auto name = node->xml_name().as<cwstr_t>();
		if (name == "uniforms"_s)
			LoadPsConstBuffer(node);
		else if (name == "samplers"_s)
			LoadPsSamplers(node);
		else if (name == "entry"_s)
			psEntry = node->xml_value();
		else if (name == "code"_s)
			psSource = node->xml_value();
		else if (name == "compile_config"_s)
		{
			config = node->xml_value();
			auto configs = ang::move(config->split("-D"));
			foreach(configs, [&](string& _value) {
				cstr_t value = _value;
				auto idx = value.find("=", 0);
				if (idx != invalid_index)
				{
					psCompileConfig += {null, null};
					value.sub_string(psCompileConfig->end()->key_value(), 0, idx);
					value.sub_string(psCompileConfig->end()->value(), idx + 1, value.size() - idx);
				}
				else
				{
					psCompileConfig += {_value, null};
				}
			});
		}
	});

	return true;
}

bool CTechnique::LoadVsConstBuffer(xml::xml_node_t node)
{
	graphics::reflect::variable_desc uniforms;
	if (!uniforms.load(node, 16U))
		return false;
	vsUniforms += {};
	*vsUniforms->end() = ang::move(uniforms);
	return true;
}

bool CTechnique::LoadPsConstBuffer(xml::xml_node_t node)
{
	graphics::reflect::variable_desc uniforms;
	if (!uniforms.load(node, 16U))
		return false;
	psUniforms += {};
	*psUniforms->end() = ang::move(uniforms);
	return true;
}

bool CTechnique::LoadPsSamplers(xml::xml_node_t samplers)
{
	if (!samplers->xml_has_children())
		return false;

	foreach(samplers->xml_children(), [&](xml::xml_node_t sampler)
	{
		if ((cwstr_t)sampler->xml_name() == "sampler"_s)
		{
			psSamplers += sampler->xml_attributes()["wrap"].as<graphics::textures::tex_wrap_mode_t>();
		}
	});
	return true;
}

/////////////////////////////////////////////////////////////////////


CEffectLibrary::CEffectLibrary()
{
	main_mutex = new core::async::mutex();
}

CEffectLibrary::~CEffectLibrary()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(AngineEditor::CEffectLibrary, ang::object)

bool CEffectLibrary::Load(xml::xml_node_t library)
{
	if (library.is_empty() || !library->xml_has_children())
		return false;

	xml::foreach(library->xml_children(), [&](xml::xml_node_t node)
	{
		auto name = node->xml_name().as<cwstr_t>();
		if (name == "sources"_s)
			LoadSources(node);
		else if (name == "technique"_s)
			LoadTechnique(node);//hack load_effect(driver, node);
	});
	return true;
}

bool CEffectLibrary::Save(xml::xml_builder_t builder)
{

}

bool CEffectLibrary::LoadSources(xml::xml_node_t node)
{
	if (!node->xml_has_children())
		return false;

	xml::foreach(node->xml_children(), [&](xml::xml_node_t file)
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

bool CEffectLibrary::LoadTechnique(xml::xml_node_t node)
{
	if (!node->xml_has_children())
		return null;
	CTechniquePtr technique = new CTechnique();
	if (!technique->Load(node))
		return false;
	main_mutex->lock();
	_techniques += technique;
	main_mutex->unlock();
	return true;
}