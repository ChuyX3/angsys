#include "pch.h"
#include "d3d11/d3d11_driver.h"

#if DIRECTX11_VRX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;
using namespace winrt::Windows::Graphics::Holographic;

holographic_camera::holographic_camera(holographic_driver* driver, HolographicCamera const& camera)
	: m_parent_driver(driver)
	, m_holographic_camera(camera)
	, m_back_buffer(new d3d11_texture())
	, m_is_stereo(camera.IsStereo())
{
	m_viewport.left = 0;
	m_viewport.top = 0;
	m_viewport.front = 0;
	m_viewport.back = 1.0f;
	m_viewport.width(m_holographic_camera.RenderTargetSize().Width);
	m_viewport.height(m_holographic_camera.RenderTargetSize().Height);
}

void holographic_camera::dispose()
{
	m_holographic_camera = null;
	m_frame_buffer = null;
}

bool holographic_camera::update_resources(HolographicCameraRenderingParameters const& parameters, HolographicCameraPose const& pose)
{
	holographic_driver_t driver = m_parent_driver.lock();

	winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface surface = parameters.Direct3D11BackBuffer();

	com_wrapper<ID3D11Texture2D> back_buffer;
	surface.as<::Windows::Graphics::DirectX::Direct3D11::IDirect3DDxgiInterfaceAccess>()
		->GetInterface(__uuidof(ID3D11Texture2D), (void**)back_buffer.addres_for_init());
		
	D3D11_TEXTURE2D_DESC desc;
	back_buffer->GetDesc(&desc);

	if (m_frame_buffer.is_empty())
	{
		d3d11_texture_t back_buffers[1] = { m_back_buffer = new d3d11_texture() };
		d3d11_texture_t depth_stencil = new d3d11_texture();
		m_back_buffer->attach(back_buffer.get());
		auto dim = m_back_buffer->tex_dimentions();
		depth_stencil->create(driver.get()
			, textures::tex_type::tex2D
			, textures::tex_format::D16
			, buffers::buffer_usage::def
			, buffers::buffer_bind_flag::depth_stencil
			, size3d<uint>{ dim.width, dim.height, m_is_stereo ? 2u : 1u });

		m_frame_buffer = new d3d11_frame_buffer(driver.get());
		m_frame_buffer->create(back_buffers, depth_stencil);
	}
	else if(m_back_buffer->D3D11Resource() != back_buffer.get())
	{
		m_back_buffer = new d3d11_texture();
		m_back_buffer->attach(back_buffer.get());
		m_frame_buffer->update(m_back_buffer);
	}
	
	return true;
}

bool holographic_camera::update_transform(winrt::Windows::Perception::Spatial::SpatialCoordinateSystem const& coordinateSystem,
	HolographicCameraPose const& pose)
{
	HolographicStereoTransform viewTransform;
	HolographicStereoTransform projectionTransform;

	// The system changes the viewport on a per-frame basis for system optimizations.
	auto viewport = pose.Viewport();
	m_viewport.left = viewport.X;
	m_viewport.top = viewport.Y;
	m_viewport.front = 0;
	m_viewport.back = 1.0f;
	m_viewport.width(viewport.Width);
	m_viewport.height(viewport.Height);
	
	// The projection transform for each frame is provided by the HolographicCameraPose.
	projectionTransform = pose.ProjectionTransform();

	memcpy(&m_proj_matrix[0], &projectionTransform.Left, sizeof(maths::float4x4));
	memcpy(&m_proj_matrix[1], &projectionTransform.Right, sizeof(maths::float4x4));

	// Get a container object with the view and projection matrices for the given
	// pose in the given coordinate system.
	auto viewTransformContainer = pose.TryGetViewTransform(coordinateSystem);

	bool viewTransformAcquired = viewTransformContainer != nullptr;
	if (viewTransformAcquired)
	{
		viewTransform = viewTransformContainer.Value();
		memcpy(&m_view_matrix[0], &viewTransform.Left, sizeof(maths::float4x4));
		memcpy(&m_view_matrix[1], &viewTransform.Right, sizeof(maths::float4x4));
	}
	else
	{
		m_view_matrix[0] = maths::matrix::identity();
		m_view_matrix[1] = maths::matrix::identity();
	}
	return true;
}


d3d11_frame_buffer_t holographic_camera::frame_buffer()const
{
	return m_frame_buffer;
}

box<float> holographic_camera::viewport()const
{
	return m_viewport;
}

maths::float4x4 holographic_camera::view_matrix(windex idx)const
{
	return m_view_matrix[m_is_stereo ? max(idx, 1) : 0];
}

maths::float4x4 holographic_camera::projection_matrix(windex idx)const
{
	return m_proj_matrix[m_is_stereo ? max(idx, 1) : 0];
}


#endif
