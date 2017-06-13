#pragma once

#include "EffectData.h"

namespace AngineEditor
{
	using namespace ang;
	class CProjectManager;
	typedef object_wrapper<CProjectManager> CProjectManagerPtr;

	namespace Properties
	{
		typedef value_wrapper<collections::pair<string, object>> CProperty;
		typedef object_wrapper<CProperty> CPropertyPtr;


	
	}

	namespace filesystem
	{
		class file;
		class folder;
		typedef object_wrapper<file> file_t;
		typedef object_wrapper<folder> folder_t;

		class file : public object
		{
		private:
			index _name;
			index _ext;
			wstring _full_path;

			collections::map<string, objptr> _properties;

		public:
			file(cwstr_t full);
			file(cwstr_t path, cwstr_t name);
			file(cwstr_t path, cwstr_t name, cwstr_t ext);

			wstring full_path()const;
			wstring path()const;
			wstring name()const;
			wstring ext()const;
			wstring name_ext()const;

			void path(cwstr_t);
			void name(cwstr_t);
			void ext(cwstr_t);

			objptr property(cwstr_t key)const {
				if (_properties.is_empty())
					return null;
				auto it = _properties->find(key);
				return it.is_valid() ? _properties->at(it).value() : null;
			}
			void property(cwstr_t key, objptr value) {
				_properties += {key, value};
			}

		private:
			virtual~file();
		};

		class folder : public object
		{
		private:
			wstring _root;
			wstring _name;
			weak_ptr<folder> _parent;
			collections::vector<file_t> _files;
			collections::vector<folder_t> _folders;
			
		public:
			folder();

			//bool load(xml::xml_node_t);
			bool load(xml::xml_node_t, cwstr_t root);
			bool load(xml::xml_node_t, cwstr_t root, folder_t);
			bool save(xml::xml_builder_t)const;

			wstring path()const;
			wstring full_path()const;

			static_array<file_t> files()const;
			static_array<folder_t> folders()const;

		private:
			
			virtual~folder();
		};
	}


	class CProjectManager
		: public object
		, public singleton<CProjectManagerPtr>
	{
	private:
		friend singleton<CProjectManagerPtr>;
		xml::xml_document_t xmlDoc;
		xml::xml_document_t xmlScenesDoc;
		xml::xml_document_t xmlResourcesDoc;
		
		wstring strProjectPath;
		filesystem::folder_t filters;
		collections::map<string, wstring> files;
		//ChildProcess childProcess;

		graphics::idriver_t driver;
		//ang::IntfPtr<Drawing::ISurface> surface;
		
		//ang::SharedProperty<Engine::Resources::ResourceManager, CProjectManager> resourceManager;
		//ang::SharedProperty<Engine::Scenes::StaticCamera, CProjectManager > camera;


		//ang::core::async::mutex_t mutex;
		//ang::core::async::cond_t cond;
		//ang::core::async::thread_t mainThread;
		//ang::core::async::thread_t renderThread;
		//ang::IntfPtr<ang::Core::Threading::IAsyncAction<void>> renderAsyncAction;

	public:
		CProjectManager();
	

		ANG_DECLARE_INTERFACE();

		bool LoadDocument(cstr_t xmlCode);
		bool LoadDocument(cwstr_t xmlCode);
		bool LoadDocument(cmstr_t xmlCode);
		bool SaveDocument(wstring& xmlCode)const;

		xml::xml_document_t XmlDocument();
		inline const wstring& ProjectPath()const { return strProjectPath; }
		inline graphics::idriver_t GraphicDriver()const { return driver; }

		//bool LoadEffectData(xml::xml_node_t, EffectData& out)const;
		//bool SaveEffectData(xml::XmlBuilder*, EffectData const& data)const;

		collections::map<string, wstring> FileMap()const { return files.get(); }


	public:
		//bool BeginRendering(platform::icore_view_t);
		//bool StopRendering();

	private:
		//void RenderProc(ang::Core::Threading::IAsyncAction<void>* task, ang::Core::Events::ArgsList*);
		//bool InitDriver(Platform::ICoreView*);
		//void Update(ang::Float);
		//void Draw();

		void LoadResources();
		void ReleaseResources();

		bool DestroyDriver();

	private:
		bool LoadFiles();

		virtual~CProjectManager();
	};
}


