#include "stdafx.h"
#include "ProjectManager.h"
#include "UI/MainFrm.h"

using namespace AngineEditor;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


wstring GetRelativeFilename(cwstr_t currentDirectoryPath, cwstr_t absoluteFilenamePath)
{
//	ang::UInt afMarker = 0, rfMarker = 0;

	wstring relativeFilename;

	cwstr_t currentDirectory = currentDirectoryPath;
	cwstr_t absoluteFilename = absoluteFilenamePath;


	// make sure the names are not too long or too short
	if (currentDirectory.size() > MAX_PATH || currentDirectory.size() < 3 + 1 
		|| absoluteFilename.size() > MAX_PATH || absoluteFilename.size() < 3 + 1)
	{
		return null;
	}

	// Handle DOS names that are on different drives:
	if (currentDirectory[0u] != absoluteFilename[0u])
	{
		return absoluteFilename;
	}

	index idx = strings::algorithms::string_compare_until(currentDirectory, absoluteFilename);

	if (idx >= currentDirectory.size() && (absoluteFilename[idx] == '\\' || absoluteFilename[idx - 1] == '\\'))
	{
		// the whole current directory name is in the file name,
		// so we just trim off the current directory name to get the
		// current file name.
		if (absoluteFilename[idx] == '\\')
		{
			// a directory name might have a trailing slash but a relative
			// file name should not have a leading one...
			idx++;
		}
		return cwstr_t(&absoluteFilename[idx], absoluteFilename.size() - idx);
	}

	// The file is not in a child directory of the current directory, so we
	// need to step back the appropriate number of parent directories by
	// using "..\"s.  First find out how many levels deeper we are than the
	// common directory
	index afMarker = idx;
	index levels = 1;

	// count the number of directory levels we have to go up to get to the
	// common directory

	idx = currentDirectory.find(L"\\", idx);
	while (idx != invalid_index)
	{
		idx = idx + 1;
		if(idx < currentDirectory.size())
			levels++;
		idx = currentDirectory.find(L"\\", idx);
	}

	// move the absolute filename marker back to the start of the directory name
			// that it has stopped in.
	idx = absoluteFilename.find_rev(L"\\", afMarker);
	
	if (idx == invalid_index)
		return null;
	afMarker = idx;

	// check that the result will not be too long
	if (levels * 3 + absoluteFilename.size() - afMarker > MAX_PATH)
	{
		return null;
	}

	// add the appropriate number of "..\"s.
	for (index i = 0; i < levels; i++)
	{
		relativeFilename += "..\\";
	}

	// copy the rest of the filename into the result string
	relativeFilename += cwstr_t(&absoluteFilename[afMarker], absoluteFilename.size() - afMarker);

	return relativeFilename;
}


CProjectManager::CProjectManager()
{
	xmlDoc = new xml::xml_document();
}


CProjectManager::~CProjectManager()
{
	xmlDoc = nullptr;
	files.clean();
}

ANG_IMPLEMENT_BASIC_INTERFACE(AngineEditor::CProjectManager, ang::object);

bool CProjectManager::LoadDocument(cstr_t xmlCode)
{
	try
	{
		xmlDoc->parse(xmlCode);
		LoadFiles();
	}
	catch (exception_t)
	{
		return false;
	}
	return true;
}

bool CProjectManager::LoadDocument(cwstr_t xmlCode)
{
	try
	{
		xmlDoc->parse(xmlCode);
		LoadFiles();
	}
	catch (exception_t)
	{
		return false;
	}
	return true;
}

bool CProjectManager::LoadDocument(cmstr_t xmlCode)
{
	try
	{
		xmlDoc->parse(xmlCode);
		LoadFiles();
	}
	catch (exception_t)
	{
		return false;
	}
	return true;
}


bool CProjectManager::SaveDocument(wstring& xmlCode)const
{
	xmlDoc->xml_tree()->xml_print(xmlCode, xml::xml_format::fix_entity + xml::xml_format::wrap_text_space);
	return true;
}

xml::xml_document_t CProjectManager::XmlDocument()
{
	return xmlDoc;
}

bool CProjectManager::LoadFiles()
{
	strProjectPath = xmlDoc->xml_tree()->find_first("project_dir")->xml_value().as<cwstr_t>();
	auto it = xmlDoc->xml_tree()->find_first("filters");
	if (it.is_valid())
	{
		filesystem::folder_t folder = new filesystem::folder();

		if (!folder->load((xml::xml_node*)it, strProjectPath))
			return false;
		filters = folder;

		auto  mainFrm = CMainFrame::GetMainFrame();
		if (mainFrm != NULL)
		{
			mainFrm->GetFileViewPanel()->FillFileView(filters);
		}
		return true;
	}

	return false;
}

//bool CProjectManager::LoadEffectData(xml::xml_node_t element, EffectData& out)const
//{
//	if (element == nullptr || !element->XmlHasChildren() || ang::StringOperation::CompareWA(element->xml_name().as<cwstr_t>(), "effect") != 0)
//		return false;
//
//	out.Profiles.Size(3);
//	out.Name = element->XmlAttributeFinder()["name"].as<ang::cwstr_t>();
//
//	for (auto profile = element->XmlChildren()->Begin(); profile.is_valid(); ++profile)
//	{
//		if (ang::StringOperation::CompareWA(profile->xml_name().as<cwstr_t>(), "profile") != 0)
//			continue;
//
//		auto idx = (int)profile->XmlAttributeFinder()["type"].as<Drawing::GraphDriverType>() - 1;
//		if (idx < 0 || idx > 2)
//			continue;
//
//		EffectProfileData& profileData = out.Profiles[idx];	
//		profileData.DriverType = Drawing::GraphDriverType((int)Drawing::GraphDriverType::OpenGL + idx);
//		profileData.is_valid = true;
//
//		for (auto technique = profile->XmlChildren()->Begin(); technique.is_valid(); ++technique)
//		{
//			if (ang::StringOperation::CompareWA(technique->xml_name().as<cwstr_t>(), "technique") != 0)
//				continue;
//			
//			profileData.Techniques += TechniqueData();
//			TechniqueData& techniqueData = *profileData.Techniques.End();
//			techniqueData.Name = technique->XmlAttributeFinder()["name"].as<ang::cwstr_t>();
//
//			for (auto pass = technique->XmlChildren()->Begin(); pass.is_valid(); ++pass)
//			{
//				if (ang::StringOperation::CompareWA(pass->xml_name().as<cwstr_t>(), "pass") != 0)
//					continue;
//
//				techniqueData.Passes += PassData();
//				PassData& passData = *techniqueData.Passes.End();
//
//				for (auto shader = pass->XmlChildren()->Begin(); shader.is_valid(); ++shader)
//				{
//					if (ang::StringOperation::CompareWA(shader->xml_name().as<cwstr_t>(), "shader") != 0)
//						continue;
//		
//					ShaderData* shaderData = nullptr;
//					auto st = shader->XmlAttributeFinder()["type"].as<Drawing::ShaderType>();
//					if (st == Drawing::ShaderType::Vertex)
//						shaderData = &passData.VertexShader;
//					else if (st == Drawing::ShaderType::Fragment)
//						shaderData = &passData.FragmentShader;
//					else
//						continue;
//
//					shaderData->FileID = shader->XmlAttributeFinder()["file"].as<cwstr_t>();
//
//					for (auto shader_element = shader->XmlChildren()->Begin(); shader_element.is_valid(); ++shader_element)
//					{
//						if (StringOperation::CompareWA(shader_element->xml_name().as<cwstr_t>(), "entry_point") == 0)
//						{
//							auto value = shader_element->xml_value();
//							shaderData->EntryPoint = value.Value.get()? value.as<cwstr_t>() : L"main";
//						}
//						else if (StringOperation::CompareWA(shader_element->xml_name().as<cwstr_t>(), "compile_config") == 0)
//						{
//							AString compileConfig = shader_element->xml_value().as<cwstr_t>();
//							auto configlist = compileConfig.Split("-D");
//							for (auto i = configlist.Begin(); i.is_valid(); ++i)
//							{
//								Storage::Pair<AString, AString> pair;
//								auto config = i->Split('=');
//								if (config.Size() > 0)
//								{
//									pair.KeyValue() = config[0];
//									if (config.Size() > 1)
//									{
//										pair.Datum() = config[1];
//									}
//									shaderData->CompileConfigs += pair;
//								}
//							}
//						}
//						else if (StringOperation::CompareWA(shader_element->xml_name().as<cwstr_t>(), "vertex_inputs") == 0)
//						{
//							
//						}
//						else if (StringOperation::CompareWA(shader_element->xml_name().as<cwstr_t>(), "const_buffer") == 0)
//						{
//
//						}
//						else if (StringOperation::CompareWA(shader_element->xml_name().as<cwstr_t>(), "samplers") == 0)
//						{
//
//						}
//					}//end shader configs iteration
//				}//end shader iteration
//			}//end pass iteration
//		}//end technique iteration
//	}//end profile iteration
//
//	return true;
//}
//
//
//bool CProjectManager::SaveEffectData(xml::xml_builder_t builer, EffectData const& data)const
//{
//	if (builer == nullptr)
//		return false;
//	builer->BeginElement("effect");
//	{
//		builer->Attribute("name", data.Name);
//	
//		for (auto i = 0; i < 3; ++i)
//		{
//			EffectProfileData const& profile = data.Profiles[i];
//			if (profile.is_valid)
//			{
//				builer->BeginElement("profile");
//				{
//		
//					builer->Attribute("count"
//						, profile.DriverType == Drawing::GraphDriverType::OpenGL ? "GLSL"
//						: profile.DriverType == Drawing::GraphDriverType::OpenGLES ? "GLES"
//						: profile.DriverType == Drawing::GraphDriverType::DirectX ? "HLSL"
//						: "NULL");
//
//					builer->Attribute("count", Unsigned32(profile.Techniques.Counter()).ToAString());
//					
//			
//
//				}builer->EndElement();
//			}
//		}
//
//		
//	}builer->EndElement();
//	return true;
//}
////
//bool CProjectManager::RunChildProcess()
//{
//	if (childProcess.IsCreated())
//		return false;
//
//	_ChildProcessArgs args = {
//		TEXT("angine_test.exe"),
//		TEXT("\"angine_test.exe\" \".\\resources\\engine.xml\" \".\\resources\\\""),
//		AngineEditor::CProjectManager::Instance()->ProjectPath()
//	};
//
//	return childProcess.Create(&args);;
//}
//
//bool CProjectManager::IsChildProcessRunning()
//{
//	return childProcess.IsRunning();
//}
//
//bool CProjectManager::StopChildProcess()
//{
//	if (!childProcess.IsCreated())
//		return false;
//
//	return childProcess.Close();
//}

//
//bool CProjectManager::BeginRendering(Platform::ICoreView* view)
//{
//	ang::Core::Threading::ScopeLocker locker(mutex);
//
//	if (!renderAsyncAction.IsPointerEmpty())
//		return false;
//
//	if (!InitDriver(view))
//		return false;
//
//	ang::Core::Threading::AsyncTask<void>::RunAsync(ang::Core::Threading::ThreadPriority::High, nullptr, this, &CProjectManager::RenderProc);
//	cond.While(mutex, [&]()->bool { return renderAsyncAction.Get() == nullptr; });
//	return true;
//
//}
//
//bool CProjectManager::StopRendering()
//{
//	ang::Core::Threading::ScopeLocker locker(mutex);
//	if (renderAsyncAction.IsPointerEmpty())
//		return false;
//	renderAsyncAction->Cancel();
//	cond.While(mutex, [&]()->bool { return !renderAsyncAction.IsPointerEmpty(); });
//	return true;
//}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//void CProjectManager::RenderProc(ang::Core::Threading::IAsyncAction<void>* task, ang::Core::Events::ArgsList*)
//{
//	renderThread = ang::Core::Threading::Thread::CurrentThread();
//	{ang::Core::Threading::ScopeLocker locker(mutex);
//	renderAsyncAction = task;
//	cond.Signal(mutex);
//	}
//
//	LoadResources();
//
//	ang::Core::Tempus::DeltaTime delta;
//	delta.Reset();
//	while (task->Status() == ang::Core::Threading::AsyncActionStatus::Running)
//	{
//		ang::Core::Threading::ScopeLocker locker(mutex);
//		delta.Update();
//		Update(delta.Delta());
//		Draw();
//	}
//
//	ReleaseResources();
//
//	{ang::Core::Threading::ScopeLocker locker(mutex);
//	renderAsyncAction = nullptr;
//	cond.Signal(mutex);
//	}
//}
//
//bool CProjectManager::InitDriver(Platform::ICoreView* view)
//{
//	using namespace ang;
//
//	if (driver.IsPointerEmpty())
//	{
//		if (!Drawing::DriverFactory::CreateGraphDriver(Drawing::GraphDriverType::DirectX, view, &driver))
//			return false;
//		context = driver->Context();
//		surface = driver->Surface();
//		context->BindRenderTarget(surface->RenderTarget());
//		resourceManager = new Engine::Resources::ResourceManager(driver);
//	}
//	else
//	{
//		context = driver->Context();
//		surface = nullptr;
//		if (!driver->CreateSurface(view, &surface))
//			return false;
//		driver->MakeCurrent(surface);
//		context->BindRenderTarget(surface->RenderTarget());
//	}
//
//	return true;
//}
//
//bool CProjectManager::DestroyDriver()
//{
//	using namespace ang;
//	Core::Threading::ScopeLocker locker(mutex);
//	camera = nullptr;
//
//	surface = nullptr;
//	context = nullptr;
//	driver = nullptr;
//	return true;
//}
//
//void CProjectManager::Update(ang::Float delta)
//{
//	using namespace ang;
//
//}
//
//void CProjectManager::Draw()
//{
//	using namespace ang;
//	context->BindToCurrentThread();
//
//	context->BindRenderTarget(surface->RenderTarget());
//	context->ClearRenderTarget({ 0.0f,0.2f,0.4f,1 });
//
//
//	surface->SwapBuffers();
//}
//
//void CProjectManager::LoadResources()
//{
//	//resourceManager->Load();
//
//}
//
//void CProjectManager::ReleaseResources()
//{
//
//}