#include "pch.h"
#include "d3d9/driver.h"

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d9;

d3d9_driver::d3d9_driver()
{
}

d3d9_driver::~d3d9_driver()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::d3d9::d3d9_driver);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::d3d9::d3d9_driver, object, idriver);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::d3d9::d3d9_driver, object, idriver);

bool d3d9_driver::init_driver()
{
	LPDIRECT3D9 d3d;

	HWND hWnd = CreateWindowW(L"static", L"", WS_OVERLAPPED, 0, 0, 20, 20, NULL, NULL, GetModuleHandle(NULL), NULL);

	d3d = Direct3DCreate9(D3D_SDK_VERSION); 

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp)); 
	d3dpp.Windowed = TRUE; 
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;

	// create a device class using this information and information from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_device);

	CloseWindow(hWnd);

	return false;
}

void d3d9_driver::close_driver()
{

}

