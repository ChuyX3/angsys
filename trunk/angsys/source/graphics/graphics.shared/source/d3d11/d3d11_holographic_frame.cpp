#include "pch.h"
#include "d3d11/d3d11_driver.h"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

#if DIRECTX11_VRX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;

using namespace Windows;
using namespace Windows::Foundation;
using namespace Windows::Graphics;
using namespace Windows::Graphics::Holographic;

//#define MY_TYPE ang::graphics::d3d11::holographic_frame
//#include <ang/inline/object_wrapper_specialization.inl>
//#undef MY_TYPE

holographic_frame::holographic_frame(d3d11_driver* driver)
	: m_parent_driver(driver)
{
	
}

holographic_frame::~holographic_frame()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::d3d11::holographic_frame);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::d3d11::holographic_frame, object, isurface);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::d3d11::holographic_frame, object, isurface);

bool holographic_frame::create(HolographicSpace^ space)
{
	m_camera_added_token = space->CameraAdded += ref new TypedEventHandler<HolographicSpace ^, HolographicSpaceCameraAddedEventArgs ^>(
		[this](HolographicSpace^ sender, HolographicSpaceCameraAddedEventArgs^ args)
	{
		this->on_camera_added(sender, args);
	});

	m_camera_removed_token = space->CameraRemoved += ref new TypedEventHandler<HolographicSpace ^, HolographicSpaceCameraRemovedEventArgs ^>(
		[this](HolographicSpace^ sender, HolographicSpaceCameraRemovedEventArgs^ args)
	{
		this->on_camera_removed(sender, args);
	});
	return true;
}

void holographic_frame::on_camera_added(HolographicSpace^ sender, HolographicSpaceCameraAddedEventArgs^ args)
{
	d3d11_driver_t driver = m_parent_driver.lock();
	Deferral^ deferral = args->GetDeferral();
	HolographicCamera^ camera = args->Camera;
	driver->create_task<void>([this, deferral, camera](core::async::iasync<void> task, d3d11_driver_t driver)
	{
		driver->execute_on_thread_safe([&]() 
		{
			holographic_camera_t cam = new holographic_camera(driver);
			if (cam->create(camera))
			{
				m_camera_map[camera->Id] = cam;
			}
			
		});
		deferral->Complete();
	});
}

void holographic_frame::on_camera_removed(HolographicSpace^ sender, HolographicSpaceCameraRemovedEventArgs^ args)
{
	d3d11_driver_t driver = m_parent_driver.lock();
	//args->Camera
	driver->execute_on_thread_safe([&]()
	{
		holographic_camera_t camera;
		if (m_camera_map->remove(args->Camera->Id, camera)) {
			camera->clear();
			camera = null;
		}
	});
}


bool holographic_frame::close()
{
	
	return true;
}

void holographic_frame::update()
{
		
}

void holographic_frame::swap_buffers(bool)
{
	
}

collections::ienum_ptr<collections::tuple<icamera_t, iframe_buffer_t>> holographic_frame::cameras()const
{
	return m_cameras.get();
}

#endif
