#include "pch.h"
#include "d3d11/d3d11_driver.h"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

#if DIRECTX11_VRX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;

//#define MY_TYPE ang::graphics::d3d11::holographic_camera
//#include <ang/inline/object_wrapper_specialization.inl>
//#undef MY_TYPE

holographic_camera::holographic_camera(d3d11_driver* driver)
	: m_parent_driver(driver)
{
	
}

holographic_camera::~holographic_camera()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::d3d11::holographic_camera);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::d3d11::holographic_camera, object, icamera);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::d3d11::holographic_camera, object, icamera);

void holographic_camera::clear()
{

}

bool holographic_camera::create(Windows::Graphics::Holographic::HolographicCamera^ camera)
{

}

box<float> holographic_camera::viewport()const
{
	return box<float>();
}

maths::float4x4 holographic_camera::view_projection_matrix()const
{
	return maths::float4x4();
}



#endif
