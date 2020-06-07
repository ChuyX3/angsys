#include "pch.h"
#include "d3d11/d3d11_driver.h"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

#if DIRECTX11_VRX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;

using namespace winrt::Windows::Graphics::Holographic;

holographic_driver::holographic_driver()
	: base()
	, m_holographic_space(null)
	, m_vr_support(false)
{
	
}

graph_driver_type_t holographic_driver::graph_driver_type()const
{
	return graph_driver_type::DirectX11_VRX;
}

bool holographic_driver::vr_extension_support()const { return m_vr_support; }

bool holographic_driver::init_driver(platform::icore_view_t wnd, long64)
{
	HRESULT hr = S_OK;

#if WINDOWS_PLATFORM == WINDOWS_STORE_PLATFORM
	
	auto cwnd = winrt::from_com<winrt::Windows::UI::Core::CoreWindow>((IUnknown*)wnd->core_view_handle());
	m_holographic_space = winrt::Windows::Graphics::Holographic::HolographicSpace::CreateForCoreWindow(cwnd);
#else
	// Use WinRT factory to create the holographic space.
	using namespace winrt::Windows::Graphics::Holographic;

	auto hWnd = reinterpret_cast<HWND>(wnd->core_view_handle());
	winrt::com_ptr<IHolographicSpaceInterop> holographicSpaceInterop = winrt::get_activation_factory<HolographicSpace, IHolographicSpaceInterop>();
	winrt::com_ptr<ABI::Windows::Graphics::Holographic::IHolographicSpace> spHolographicSpace;
	winrt::check_hresult(holographicSpaceInterop->CreateForWindow(hWnd, __uuidof(ABI::Windows::Graphics::Holographic::IHolographicSpace), winrt::put_abi(spHolographicSpace)));

	if (!spHolographicSpace)
	{
		winrt::check_hresult(E_FAIL);
	}

	m_holographic_space = spHolographicSpace.as<HolographicSpace>();
#endif

	LUID id =
	{
		m_holographic_space.PrimaryAdapterId().LowPart,
		m_holographic_space.PrimaryAdapterId().HighPart
	};

	if (!d3d11_driver::init_driver(wnd, reinterpret_cast<long64&>(id)))
		return false;

	com_wrapper<IDXGIDevice3> dxgiDevice;
	winrt::com_ptr<::IInspectable> interop_device;
	D3D11Device()->QueryInterface(dxgiDevice.addres_for_init());
	CreateDirect3D11DeviceFromDXGIDevice(dxgiDevice.get(), interop_device.put());
	m_d3d_interop_device = interop_device.as<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice>();
	m_holographic_space.SetDirect3D11Device(m_d3d_interop_device);

	D3D11_FEATURE_DATA_D3D11_OPTIONS3 options{};
	D3D11Device()->CheckFeatureSupport(D3D11_FEATURE_D3D11_OPTIONS3, &options, sizeof(options));
	m_vr_support = options.VPAndRTArrayIndexFromAnyShaderFeedingRasterizer ? true : false;

	return true;
}

void holographic_driver::close_driver()
{
	m_d3d_interop_device = null;
	m_holographic_space = null;

	d3d11_driver::close_driver();
}

isurface_t holographic_driver::create_surface(platform::icore_view_t view)const
{
	if (m_vr_support)
	{
		try {
			holographic_scene_t frame = new holographic_scene(const_cast<holographic_driver*>(this));
			if (!frame->create(m_holographic_space))
				return null;
			return frame.get();
		}
		catch (...) {
			return d3d11_driver::create_surface(view);
		}
	}
	else
	{
		return d3d11_driver::create_surface(view);
	}
}

#endif//DIRECTX11_HGX_SUPPORT
