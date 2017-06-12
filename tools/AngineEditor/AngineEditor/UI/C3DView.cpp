// UI/C3DView.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/C3DView.h"
#include <Tempus.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// C3DView

namespace ang
{

	namespace Drawing
	{
		class DeviceContextHandle
			: public Object
			, public Platform::ICoreContext
		{
		protected:
			HWND hWnd = nullptr;
			HDC hDC = nullptr;

		public:
			DeviceContextHandle() {}
			DeviceContextHandle(DeviceContextHandle && dc) {
				hWnd = dc.hWnd;
				hDC = dc.hDC;
				dc.hWnd = nullptr;
				dc.hDC = nullptr;
			}
			DeviceContextHandle(DeviceContextHandle const& dc) { 
				Create(dc.hWnd);
			}
			DeviceContextHandle(HWND hwnd) {
				Create(hwnd);
			}
			virtual~DeviceContextHandle() {
				Close();
			}

			IMPLEMENT_INTERFACE();
			Bool Create(HWND hwnd) {
				if (hDC != nullptr || !IsWindow(hwnd))
					return false;
				hWnd = hwnd;
				hDC = GetDC(hwnd);
				return true;
			}

			Bool Close() {
				if (hDC == nullptr)
					return false;
				ReleaseDC(hWnd, hDC);
				hWnd = nullptr;
				hDC = nullptr;
				return true;
			}
			
			virtual Handle GetCoreContextHandle()const override {
				return hDC;
			}

			virtual Bool BindGraphicNativeSurface(Handle)override {
				return false;
			}

			DeviceContextHandle& operator = (DeviceContextHandle && dc) {
				hWnd = dc.hWnd;
				hDC = dc.hDC;
				dc.hWnd = nullptr;
				dc.hDC = nullptr;
				return *this;
			}
			DeviceContextHandle& operator = (DeviceContextHandle const& dc) {
				Create(dc.hWnd);
				return *this;
			}

		};

		_IMPLEMENT_CLASSNAME(ang::Drawing::DeviceContextHandle);
		_IMPLEMENT_OBJECTNAME(ang::Drawing::DeviceContextHandle);
		Bool DeviceContextHandle::IsKindOf(TypeName className)const
		{
			if (ang::AString::CompareA(className, type_name<DeviceContextHandle>()) == 0
				|| Object::IsKindOf(className)
				|| ang::AString::CompareA(className, type_name<ICoreContext>()) == 0)
				return true;
			return false;
		}

		Bool DeviceContextHandle::QueryObject(TypeName className, void** out)
		{
			if (out == nullptr)
				return false;
			if (ang::AString::CompareA(className, type_name<DeviceContextHandle>()) == 0)
			{
				*out = static_cast<DeviceContextHandle*>(this);
				return true;
			}
			else if (Object::QueryObject(className, out))
			{
				return true;
			}
			else if (ang::AString::CompareA(className, type_name<ICoreContext>()) == 0)
			{
				*out = static_cast<ICoreContext*>(this);
				return true;
			}
			return false;
		}
	}

}



IMPLEMENT_DYNCREATE(C3DView, CView)

C3DView::C3DView()
{
	coreContext = nullptr;
	driver = nullptr;
	context = nullptr;
	surface = nullptr;
	mainThread = nullptr;
	renderThread = nullptr;
	renderAsyncAction = nullptr;
	rotation = { 0,0,0 };
	m_bIsTrachingPointer = FALSE;
	m_bIsLPointerPressed = FALSE;
	m_bIsRPointerPressed = FALSE;
}

C3DView::~C3DView()
{
}


ang::Handle C3DView::GetCoreViewHandle()const
{
	return GetSafeHwnd();
}

ang::Platform::ICoreContext* C3DView::GetCoreContext()const
{
	return coreContext.Get();
}

ang::Drawing::BaseSize C3DView::GetCoreViewSize()const
{
	CRect rc;
	GetClientRect(&rc);
	if (rc.Width() == 0)
	{
		rc.right = 100;
	}

	if (rc.Height() == 0)
	{
		rc.bottom = 100;
	}

	return ang::Drawing::Size((float)rc.Width(), (float)rc.Height());
}

ang::Drawing::BaseSize C3DView::GetCoreViewScaleFactor()const
{
	return ang::Drawing::Size(1.0f, 1.0f);
}

BEGIN_MESSAGE_MAP(C3DView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEHWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()


// C3DView drawing

void C3DView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	//if (!driver.IsPointerEmpty())
	//{
	//	context->ClearRenderTarget({ 0.1f,0.2f,0.5f,1 });


	//	surface->SwapBuffers(nullptr);
	//}
	//

	// TODO: add draw code here
}


// C3DView diagnostics

#ifdef _DEBUG
void C3DView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void C3DView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// C3DView message handlers


int C3DView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	coreContext = new ang::Drawing::DeviceContextHandle(GetSafeHwnd());

	return 0;
}

void C3DView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	mainThread = ang::Core::Threading::Thread::CurrentThread();
	BeginRendering();
}

void C3DView::OnDestroy()
{
	StopRendering();
	coreContext = nullptr;
	CView::OnDestroy();
}

void C3DView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	ang::Core::Threading::ScopeLocker locker(mutex);
	if (!driver.IsPointerEmpty())
	{
		driver->UpdateSurface(this, surface);

		ang::Float3 pos(0, 1.0f, 2.0f);
		ang::Float3 rot(3.141592f / 6, 0, 0);
		if (Camera)
		{
			pos = Camera->Position();
			rot = Camera->Rotation();
		}

		Camera->Load({
			new ang::Vector3(pos),
			new ang::Vector3(rot),
			new ang::Vector3(0, 0, 0),
			new ang::Vector4(0.5f, float(cx) / float(cy) , 0.01f, 500.0f),
		});
	}
	
}

BOOL C3DView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void C3DView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bIsRPointerPressed)
		return CView::OnLButtonDown(nFlags, point);;

	CRect rc;
	GetClientRect(&rc);

	if (rc.PtInRect(point))
	{
		m_ptnClickDownPoint = point;
		m_bIsLPointerPressed = TRUE;
		SetCapture();
	}

	CView::OnLButtonDown(nFlags, point);
}

void C3DView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bIsLPointerPressed)
	{
		m_bIsLPointerPressed = FALSE;
		ReleaseCapture();
	}
	CView::OnLButtonUp(nFlags, point);
}

void C3DView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_bIsLPointerPressed)
		return CView::OnRButtonDown(nFlags, point);

	CRect rc;
	GetClientRect(&rc);

	if (rc.PtInRect(point))
	{
		m_ptnClickDownPoint = point;
		m_bIsRPointerPressed = TRUE;
		SetCapture();
	}
	CView::OnRButtonDown(nFlags, point);
}

void C3DView::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_bIsRPointerPressed)
	{
		m_bIsRPointerPressed = FALSE;
		ReleaseCapture();
	}

	CView::OnRButtonUp(nFlags, point);
}

void C3DView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnMButtonDown(nFlags, point);
}


void C3DView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnMButtonUp(nFlags, point);
}


void C3DView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bIsLPointerPressed)
	{
		auto res = m_ptnClickDownPoint - point;
		m_ptnClickDownPoint = point;
		auto size = GetCoreViewSize();
		if (!Camera.IsPointerEmpty()) Camera->UpdatePosition(ang::Float3(5.0f *(float)res.cx / size.width, -5.0f *(float)res.cy / size.height, 0));
	}

	if (m_bIsRPointerPressed)
	{
		auto res = m_ptnClickDownPoint - point;
		m_ptnClickDownPoint = point;
		auto size = GetCoreViewSize();
		if (!Camera.IsPointerEmpty()) Camera->UpdateRotation(ang::Float3(100.0f * (float)res.cy / size.height, 100.0f * (float)res.cx / size.width, 0));
	}
	CView::OnMouseMove(nFlags, point);
}


BOOL C3DView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default


	if (!Camera.IsPointerEmpty()) Camera->UpdatePosition(ang::Float3(0, 0, -float(zDelta) / 100.0f));


	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void C3DView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// This feature requires Windows Vista or greater.
	// The symbol _WIN32_WINNT must be >= 0x0600.
	// TODO: Add your message handler code here and/or call default

	CView::OnMouseHWheel(nFlags, zDelta, pt);
}

void C3DView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void C3DView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

ang::Bool C3DView::BeginRendering()
{
	ang::Core::Threading::ScopeLocker locker(mutex);

	if (!renderAsyncAction.IsPointerEmpty())
		return false;

	ang::Core::Threading::AsyncTask<void>::RunAsync(ang::Core::Threading::ThreadPriority::High, nullptr, this, &C3DView::RenderProc);
	cond.While(mutex, [&]()->ang::Bool { return renderAsyncAction.Get() == nullptr; });
	return true;

}

ang::Bool C3DView::StopRendering()
{
	ang::Core::Threading::ScopeLocker locker(mutex);
	if (renderAsyncAction.IsPointerEmpty())
		return false;
	renderAsyncAction->Cancel();
	cond.While(mutex, [&]()->ang::Bool { return !renderAsyncAction.IsPointerEmpty(); });
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void C3DView::RenderProc(ang::Core::Threading::IAsyncAction<void>* task, ang::Core::Events::ArgsList*)
{
	renderThread = ang::Core::Threading::Thread::CurrentThread();
	{ang::Core::Threading::ScopeLocker locker(mutex);
		renderAsyncAction = task;
		cond.Signal(mutex);
	}

	if (!InitDriver())
	{
		ang::Core::Threading::ScopeLocker locker(mutex);
		renderAsyncAction = nullptr;
		cond.Signal(mutex);
		return;
	}

	ang::Core::Tempus::DeltaTime delta;
	delta.Reset();
	while (task->Status() == ang::Core::Threading::AsyncActionStatus::Running)
	{
		ang::Core::Threading::ScopeLocker locker(mutex);
		delta.Update();
		Update(delta.Delta());
		Draw();
	}

	DestroyDriver();

	{ang::Core::Threading::ScopeLocker locker(mutex);
		renderAsyncAction = nullptr;
		cond.Signal(mutex);
	}
}

ang::Bool C3DView::InitDriver()
{
	using namespace ang;
	Core::Threading::ScopeLocker locker(mutex);
	if (!Drawing::DriverFactory::CreateGraphDriver(Drawing::GraphDriverType::DirectX, this, &driver))
		return false;
	context = driver->Context();
	surface = driver->Surface();
	context->BindRenderTarget(surface->RenderTarget());

	Camera = new Engine::Scenes::StaticCamera();

	Engine::Resources::Utils::ModelSource model;

	Streams::TextFileInputStream file;
	Streams::BinaryFileOutputStream file_out;
	file.Open(".\\resources\\models\\cube_model.xml");
	xml::XmlDocument xmlDoc;
	xmlDoc.Load(file);
	model.Load(xmlDoc.XmlRoot());
	if (file_out.Open(".\\resources\\models\\cube_model.bin"))
	{
		model.Save(file_out);
		file_out.Close();
	}
	xmlDoc.Clean();
	file.Close();

	//Streams::BinaryFileInputStream file;
	//file.Open(".\\resources\\models\\cube_model.bin");
	//model.Load(file);
	//file.Close();

	auto size = GetCoreViewSize();

	Camera->Load({
		new Vector3(0, 1.0f, 2.0f),
		new Vector3(3.141592f / 6, 0, 0),
		new Vector3(0, 0, 0),
		new Vector4(0.8f, size.width / size.height , 0.01f, 500.0f),
	});

	Streams::BinaryFileInputStream vsf;
	vsf.Open(".\\resources\\shaders\\solid_object.fx;vs;main;-DHLSL-DVSHADER.cso");

	Streams::BinaryFileInputStream psf;
	psf.Open(".\\resources\\shaders\\solid_object.fx;ps;main;-DHLSL-DPSHADER.cso");

	Drawing::_ShaderArgs vsa = { true, true, Drawing::ShaderType::Vertex, "solid_object.fx;main;-DHLSL-DPSHADER", vsf.BufferInterface() };
	Drawing::_ShaderArgs psa = { true, true, Drawing::ShaderType::Pixel, "solid_object.fx;main;-DHLSL-DPSHADER", psf.BufferInterface() };

	driver->CreateShaders(&vsa, &psa, &Shaders);

	auto section = model.Section(0);

	IntfPtr<Drawing::IRasterizerState> state;
	driver->CreateRasterizerState(Drawing::FrontFace::CounterClockwise, Drawing::CullMode::Front, &state);
	driver->CreateIndexBuffer(Drawing::ShaderVarType::U16, section.indexCount, section.indexBuffer, &IndexBuffer);
	driver->CreateVertexBuffer(model.VertexBufferLayout(), model.VertexBufferBlockCount(), model.VertexBuffer(), &VertexBuffer);


	context->BindShaders(Shaders);
	context->BindVertexBuffer(VertexBuffer);
	context->BindIndexBuffer(IndexBuffer);
	context->BindRasterizerState(state);

	return true;
}

ang::Bool C3DView::DestroyDriver()
{
	using namespace ang;
	Core::Threading::ScopeLocker locker(mutex);
	Camera = nullptr;
	
	surface = nullptr;
	context = nullptr;
	driver = nullptr;
	return true;
}

void C3DView::Update(ang::Float delta)
{
	using namespace ang;

}

void C3DView::Draw()
{
	using namespace ang;
	context->BindToCurrentThread();

	context->BindRenderTarget(surface->RenderTarget());
	context->ClearRenderTarget({ 0.0f,0.2f,0.4f,1 });

	auto name = surface->ObjectName();


	Float4X4 wold = Mathematics::ScaleMatrix(1, 1, 1)
		* Mathematics::RotationMatrixY(rotation.y)
		* Mathematics::RotationMatrixX(rotation.x)
		* Mathematics::RotationMatrixZ(rotation.z);
	Float4X4 view = Camera->ViewMatrix();
	Float4X4 proj = Camera->ProjectionMatrix();

	if (driver->DriverType() == Drawing::GraphDriverType::DirectX)
	{
		Mathematics::Transpose(wold);
		Mathematics::Transpose(view);
		Mathematics::Transpose(proj);
	}

	Shaders->MapUniforms(0);

	Shaders->Uniform("uWorldMatrix").As<Float4X4>() = wold;
	Shaders->Uniform("uViewMatrix").As<Float4X4>() = view;
	Shaders->Uniform("uProjMatrix").As<Float4X4>() = proj;

	Shaders->UnmapUniforms();

	context->DrawIndices(Drawing::Primitive::Line);

	surface->SwapBuffers();
}

