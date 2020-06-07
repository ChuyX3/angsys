#include "pch.h"
#include "d3d11/d3d11_driver.h"


#if DIRECTX11_VRX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;

using namespace winrt::Windows::Graphics::Holographic;
using namespace winrt::Windows::Perception;
using namespace winrt::Windows::Perception::Spatial;

//#define MY_TYPE ang::graphics::d3d11::holographic_scene
//#include <ang/inline/object_wrapper_specialization.inl>
//#undef MY_TYPE

holographic_scene::holographic_scene(holographic_driver* driver)
	: m_parent_driver(driver)
	, m_holographic_space(null)
	, m_stage_frame_of_reference(null)
	, m_stationary_frame_of_reference(null)
	, m_current_frame(null)
{
	
}

void holographic_scene::dispose()
{
	close();
}

bool holographic_scene::create(HolographicSpace const& space)
{
	core::async::scope_locker<core::async::mutex> lock = m_mutex;

	m_holographic_space = space;
	m_camera_added_token = m_holographic_space.CameraAdded(ang::bind(this, &holographic_scene::on_camera_added));
	m_camera_removed_token = m_holographic_space.CameraRemoved(ang::bind(this, &holographic_scene::on_camera_removed));

	m_stage_frame_of_reference = SpatialStageFrameOfReference::Current();

	// Create a fallback frame of reference 1.75 meters under the HMD when we start-up
	m_stationary_frame_of_reference = SpatialLocator::GetDefault().CreateStationaryFrameOfReferenceAtCurrentLocation(
		winrt::Windows::Foundation::Numerics::float3{ 0.0f, -1.75f, 0.0f });

	m_spatial_stage_current_changed = SpatialStageFrameOfReference::CurrentChanged(
		ang::bind(this, &holographic_scene::on_current_stage_changed));

	return true;
}

void holographic_scene::on_camera_added(HolographicSpace const& sender, HolographicSpaceCameraAddedEventArgs const& args)
{
	holographic_driver_t driver = m_parent_driver.lock();
	winrt::Windows::Foundation::Deferral deferral = args.GetDeferral();
	HolographicCamera camera = args.Camera();
	driver->create_task<void>([this, deferral, camera, driver](core::async::iasync<void> task, d3d11_driver_t)
	{
		driver->execute_on_thread_safe([&]() 
		{
			m_camera_map[camera.Id()] = new holographic_camera(driver.get(), camera);
		});
		deferral.Complete();
	});
}

void holographic_scene::on_camera_removed(HolographicSpace const& sender, HolographicSpaceCameraRemovedEventArgs  const& args)
{
	d3d11_driver_t driver = m_parent_driver.lock();
	//args->Camera
	driver->execute_on_thread_safe([&]()
	{
		holographic_camera_t camera;
		if (m_camera_map.remove(args.Camera().Id(), camera)) {
			//camera->dispose();
			camera = null;
		}
	});
}


void holographic_scene::on_current_stage_changed(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::Foundation::IInspectable const&)
{
	core::async::scope_locker<core::async::mutex> lock = m_mutex;
	m_stage_frame_of_reference = SpatialStageFrameOfReference::Current();
}

SpatialCoordinateSystem holographic_scene::world_coordinate_system()const
{
	core::async::scope_locker<core::async::mutex> lock = m_mutex;
	if (m_stage_frame_of_reference)
	{
		return m_stage_frame_of_reference.CoordinateSystem();
	}
	else
	{
		return m_stationary_frame_of_reference.CoordinateSystem();
	}
}

bool holographic_scene::close()
{
	if (m_holographic_space == null)
		return false;
	m_holographic_space.CameraRemoved(m_camera_removed_token);
	m_holographic_space.CameraAdded(m_camera_added_token);
	SpatialStageFrameOfReference::CurrentChanged(m_spatial_stage_current_changed);
	m_current_frame = null;
	return true;
}

collections::ienum_ptr<collections::tuple<scenes::itransform_t, iframe_buffer_t>> holographic_scene::update()
{
	holographic_driver_t driver = m_parent_driver.lock();
	m_current_frame = m_holographic_space.CreateNextFrame();

	driver->execute_on_thread_safe(
		[&]()
	{
		const HolographicFramePrediction prediction = m_current_frame.CurrentPrediction();
		for (auto const& pose : prediction.CameraPoses())
		{
			try {
				auto params = m_current_frame.GetRenderingParameters(pose);
				holographic_camera_t cam = m_camera_map[pose.HolographicCamera().Id()];
				cam->update_resources(params, pose);
			}
			catch (...) {
			}
		}
	});

	collections::smart_vector<collections::tuple<scenes::itransform_t, iframe_buffer_t>> list;
	driver->execute_on_thread_safe(
		[&]()
	{
		m_current_frame.UpdateCurrentPrediction();
		HolographicFramePrediction prediction = m_current_frame.CurrentPrediction();
		SpatialCoordinateSystem coordinateSystem = world_coordinate_system();


		for (auto const& pose : prediction.CameraPoses())
		{
			try {
				auto params = m_current_frame.GetRenderingParameters(pose);
				holographic_camera_t cam = m_camera_map[pose.HolographicCamera().Id()];
				cam->update_transform(coordinateSystem, pose);
				list += {cam.get(), cam->frame_buffer().get()};
			}
			catch (...) {
			}
		}

	});

	return list.get();
}



bool holographic_scene::swap_buffers(bool)
{
	auto presentResult = m_current_frame.PresentUsingCurrentPrediction();

	return presentResult == HolographicFramePresentResult::Success ? true : false;
}


#endif
