#include "pch.h"
#include "d3d11/d3d11_driver.h"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

#if DIRECTX11_VRX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;


holographic_driver::holographic_driver()
	: base()
{

}

holographic_driver::~holographic_driver()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::d3d11::holographic_driver);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::d3d11::holographic_driver, d3d11_driver);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::d3d11::holographic_driver, d3d11_driver);

graph_driver_type_t holographic_driver::graph_driver_type()const
{
	return graph_driver_type::DirectX11_VRX;
}

bool holographic_driver::init_driver(platform::icore_view_t wnd, long64 adapter_id)
{
	HRESULT hr = S_OK;

#if WINDOWS_PLATFORM == WINDOWS_STORE_PLATFORM
	using namespace Windows;
	using namespace Windows::UI;
	using namespace Windows::UI::Core;
	using namespace Windows::Graphics;
	using namespace Windows::Graphics::Holographic;

	auto cwnd = reinterpret_cast<CoreWindow^>(wnd->core_view_handle());
	m_holographic_space = Windows::Graphics::Holographic::HolographicSpace::CreateForCoreWindow(cwnd);

#else
	using namespace ABI::Windows::Foundation;
	using namespace ABI::Windows::Graphics::Holographic;
	using namespace Microsoft::WRL;
	using namespace Microsoft::WRL::Wrappers;

	ComPtr<IHolographicSpaceStatics> holographicSpaceStatics;
	hr = GetActivationFactory(HStringReference(RuntimeClass_Windows_Graphics_Holographic_HolographicSpace).Get(), &holographicSpaceStatics);
	ComPtr<IHolographicSpaceInterop> holographicSpaceInterop;
	if (SUCCEEDED(hr)) {
		hr = holographicSpaceStatics.As(&holographicSpaceInterop);
	}
	ComPtr<ABI::Windows::Graphics::Holographic::IHolographicSpace> spHolographicSpace;
	if (SUCCEEDED(hr)) {
		hr = holographicSpaceInterop->CreateForWindow((HWND)wnd->core_view_handle(), IID_PPV_ARGS(&spHolographicSpace));
		m_holographicSpace = reinterpret_cast<Windows::Graphics::Holographic::HolographicSpace^>(spHolographicSpace.Get());
	}
#endif

	LUID id =
	{
		m_holographic_space->PrimaryAdapterId.LowPart,
		m_holographic_space->PrimaryAdapterId.HighPart
	};

	if (!d3d11_driver::init_driver(wnd, reinterpret_cast<long64&>(id)))
		return false;

	com_wrapper<IDXGIDevice3> dxgiDevice;
	D3D11Device()->QueryInterface((IDXGIDevice3**)&dxgiDevice);	
	m_d3d_interop_device = Windows::Graphics::DirectX::Direct3D11::CreateDirect3DDevice(dxgiDevice.get());
	m_holographic_space->SetDirect3D11Device(m_d3d_interop_device);
	
	return true;
}

void holographic_driver::close_driver()
{
	m_d3d_interop_device = null;
	m_holographic_space = null;

	d3d11_driver::close_driver();
}

isurface_t holographic_driver::create_surface(platform::icore_view_t)const
{
	return null;
}

#endif//DIRECTX11_HGX_SUPPORT
