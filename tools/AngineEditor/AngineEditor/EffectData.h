#pragma once
namespace AngineEditor
{
	using namespace ang;

	class CTechnique;
	class CEffectLibrary;
	typedef object_wrapper<CTechnique> CTechniquePtr;
	typedef object_wrapper<CEffectLibrary> CEffectLibraryPtr;

	class CTechnique : public object
	{
	private:
		string name;
		string vsEntry;
		string psEntry;
		string vsSource;
		string psSource;

		collections::vector<graphics::reflect::attribute_desc> inputLayout;
		collections::vector<graphics::reflect::variable_desc> vsUniforms;
		collections::vector<graphics::reflect::variable_desc> psUniforms;
		collections::vector<collections::pair<string, string>> vsCompileConfig;
		collections::vector<collections::pair<string, string>> psCompileConfig;
		collections::vector<graphics::textures::tex_wrap_mode_t> psSamplers;

	public:


		CTechnique();
		CTechnique(const CTechnique& other);
		CTechnique(CTechnique&& other);

		 ANG_DECLARE_INTERFACE();

		 bool Load(xml::xml_node_t);
		 bool Save(xml::xml_builder_t);

		 CTechnique& operator = (const CTechnique& other);
		 CTechnique& operator = (CTechnique&& other);

		 cstr_t Name()const { return name; }
		 cstr_t VsEntry()const { return vsEntry; }
		 cstr_t PsEntry()const { return psEntry; }
		 cstr_t VsSource()const { return vsSource; }
		 cstr_t PsSource()const { return psSource; }
		 static_array<graphics::reflect::attribute_desc> InputLayout()const { return inputLayout; }
		 static_array<graphics::reflect::variable_desc> VsUniforms()const { return vsUniforms; }
		 static_array<graphics::reflect::variable_desc> PsUniforms()const { return psUniforms; }
		 static_array<collections::pair<string, string>> VsCompileConfig()const { return vsCompileConfig; }
		 static_array<collections::pair<string, string>> PsCompileConfig()const { return psCompileConfig; }
		 static_array<graphics::textures::tex_wrap_mode_t> PsSamplers()const { return psSamplers; }

		 void Name(cstr_t value) { name = value; }
		 void VsEntry(cstr_t value) { vsEntry = value; }
		 void PsEntry(cstr_t value) { psEntry = value; }
		 void VsSource(cstr_t value) { vsSource = value; }
		 void PsSource(cstr_t value) { psSource = value; }
		 void InputLayout( static_array<graphics::reflect::attribute_desc> value) { inputLayout = value; }
		 void VsUniforms(static_array<graphics::reflect::variable_desc> value) { vsUniforms = value; }
		 void PsUniforms(static_array<graphics::reflect::variable_desc> value) { psUniforms = value; }
		 void VsCompileConfig(static_array<collections::pair<string, string>> value) { vsCompileConfig = value; }
		 void PsCompileConfig( static_array<collections::pair<string, string>> value) { psCompileConfig = value; }
		 void PsSamplers(static_array<graphics::textures::tex_wrap_mode_t> value) { psSamplers = value; }

	private:
		bool LoadVertexShader(xml::xml_node_t);
		bool LoadPixelShader(xml::xml_node_t);
		bool LoadVsConstBuffer(xml::xml_node_t);
		bool LoadPsConstBuffer(xml::xml_node_t);
		bool LoadPsSamplers(xml::xml_node_t);

	private:
		virtual~CTechnique();
	};

	class CEffectLibrary : public object
	{
	private:
		core::async::mutex_t main_mutex;
		graphics::effects::ieffect_library_t _effect_library;
		collections::map<string, wstring> _source_map;
		collections::vector<CTechniquePtr> _techniques;

	public:
		CEffectLibrary();

		ANG_DECLARE_INTERFACE();

		bool Load(xml::xml_node_t);
		bool Save(xml::xml_builder_t);

		bool LoadSources(xml::xml_node_t);
		bool LoadTechnique(xml::xml_node_t);

		inline wstring FindFile(cstr_t sid)const {
			core::async::scope_locker lock = main_mutex;
			if (_source_map.is_empty())
				return "";
			auto it = _source_map->find(sid);
			return it.is_valid() ? it->value() : L"";
		}

		inline wstring FindFile(cwstr_t sid)const {
			core::async::scope_locker lock = main_mutex;
			if (_source_map.is_empty())
				return "";
			auto it = _source_map->find(sid);
			return it.is_valid() ? it->value() : L"";
		}

		CTechniquePtr FindTechnique(cstr_t sid)const {
			core::async::scope_locker lock = main_mutex;
			if (!_techniques.is_empty()) for (auto it = _techniques->begin(); it.is_valid(); ++it)
				if (it->Name() == sid)
					return *it;
			return null;
		}

		CTechniquePtr FindTechnique(cwstr_t sid)const {
			core::async::scope_locker lock = main_mutex;
			if (!_techniques.is_empty()) for (auto it = _techniques->begin(); it.is_valid(); ++it)
				if (it->Name() == sid)
					return *it;
			return null;
		}

	private:
		virtual~CEffectLibrary();
	};

}
