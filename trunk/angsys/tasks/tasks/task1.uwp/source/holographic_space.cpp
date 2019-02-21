#include "pch.h"
#include "engine.h"

using namespace ang;
using namespace ang::graphics;

holographic_space::holographic_space(Windows::Graphics::Holographic::HolographicSpace^hs)
	: m_space(hs)
{
	m_locator = Windows::Perception::Spatial::SpatialLocator::GetDefault();

	m_locatabilityChangedToken = m_locator->LocatabilityChanged += 
		ref new Windows::Foundation::TypedEventHandler<Windows::Perception::Spatial::SpatialLocator ^, Platform::Object ^>(
			[this](Windows::Perception::Spatial::SpatialLocator ^ sender, Platform::Object ^ args)
	{
		this->OnLocatabilityChanged(sender, args);
	});

	m_cameraAddedToken = m_space->CameraAdded += 
		ref new Windows::Foundation::TypedEventHandler<Windows::Graphics::Holographic::HolographicSpace ^, Windows::Graphics::Holographic::HolographicSpaceCameraAddedEventArgs ^>(
			[this](Windows::Graphics::Holographic::HolographicSpace ^sender,  Windows::Graphics::Holographic::HolographicSpaceCameraAddedEventArgs ^ args)
	{
		this->OnCameraAdded(sender, args);
	});
	m_cameraRemovedToken = m_space->CameraRemoved +=
		ref new Windows::Foundation::TypedEventHandler<Windows::Graphics::Holographic::HolographicSpace ^, Windows::Graphics::Holographic::HolographicSpaceCameraRemovedEventArgs ^>(
			[this](Windows::Graphics::Holographic::HolographicSpace ^sender, Windows::Graphics::Holographic::HolographicSpaceCameraRemovedEventArgs ^args)
	{
		this->OnCameraRemoved(sender, args);
	});

	m_referenceFrame = m_locator->CreateStationaryFrameOfReferenceAtCurrentLocation();

}

holographic_space::~holographic_space()
{
	if (m_space != null)
	{
		m_space->CameraAdded -= m_cameraAddedToken;
		m_space->CameraRemoved -= m_cameraRemovedToken;
	}

	if (m_locator != null)
	{
		m_locator->LocatabilityChanged -= m_locatabilityChangedToken;
	}

	m_space = null;
	m_locator = null;

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(holographic_space);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::holographic_space, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(holographic_space, object);

Windows::Graphics::Holographic::HolographicSpace^ holographic_space::space()const
{
	return m_space;
}

void holographic_space::OnLocatabilityChanged(Windows::Perception::Spatial::SpatialLocator ^sender, Platform::Object ^args)
{

}

void holographic_space::OnCameraAdded(Windows::Graphics::Holographic::HolographicSpace ^sender, Windows::Graphics::Holographic::HolographicSpaceCameraAddedEventArgs ^args)
{
	
}


void ang::graphics::holographic_space::OnCameraRemoved(Windows::Graphics::Holographic::HolographicSpace ^sender, Windows::Graphics::Holographic::HolographicSpaceCameraRemovedEventArgs ^args)
{
	
}
