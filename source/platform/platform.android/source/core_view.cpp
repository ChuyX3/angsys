#include "pch.h"
#include "ang/platform/android/android_platform.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::android;



namespace ang
{
	namespace platform
	{
		namespace android
		{
			class core_context
				: public object
				, public icore_context
			{
			public:
				core_context() {}
				virtual~core_context() {}

			public:
				ANG_DECLARE_INTERFACE();

				virtual pointer get_core_context_handle()const override { return nullptr; }
				virtual bool bind_graphic_native_surface(pointer) override { return false; };
			};
		}
	}
}

ANG_IMPLEMENT_CLASSNAME(ang::platform::android::core_context)
ANG_IMPLEMENT_OBJECTNAME(ang::platform::android::core_context)

bool core_context::is_kind_of(type_name_t name)const
{
	return (name == core_context::class_name()
		|| object::is_kind_of(name)
		|| icore_context::is_kind_of(name));
}

bool core_context::is_child_of(type_name_t name)
{
	return (name == core_context::class_name()
		|| object::is_child_of(name)
		|| icore_context::is_child_of(name));
}


bool core_context::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == nullptr)
		return false;
	if (name == core_context::class_name())
	{
		*out = static_cast<core_context*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (icore_context::query_object(name, out))
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////


core_view::core_view(activity* a)
	: _activity(a)
	, _native_window(null)
	, _client_area(0,0)
	, _scale_factor(1,1)
{

}

core_view::~core_view()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::platform::android::core_view)
ANG_IMPLEMENT_OBJECTNAME(ang::platform::android::core_view)

bool core_view::is_kind_of(type_name_t name)const
{
	return (name == core_view::class_name()
		|| object::is_kind_of(name)
		|| icore_view::is_kind_of(name));
}

bool core_view::is_child_of(type_name_t name)
{
	return (name == core_view::class_name()
		|| object::is_child_of(name)
		|| icore_view::is_child_of(name));
}


bool core_view::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == nullptr)
		return false;
	if (name == core_view::class_name())
	{
		*out = static_cast<core_view*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (icore_view::query_object(name, out))
	{
		return true;
	}
	return false;
}


imessage_reciever_t core_view::get_listener()const
{
	activity_t a;
	return (a = _activity.lock()).is_empty() ? null : a->get_listener();
}

pointer core_view::get_core_view_handle()const
{
	return _native_window;
}

icore_context_t core_view::get_core_context()const
{
	return new core_context();
}

graphics::size<float> core_view::get_core_view_size()const
{
	return _client_area;
}

graphics::size<float> core_view::get_core_view_scale_factor()const
{
	return _scale_factor;
}


void core_view::native_window(ANativeWindow* wnd)
{
	_native_window = wnd;
}
void core_view::client_area(foundation::size<float> sz)
{
	_client_area = sz;
}

void core_view::scale_factor(foundation::size<float> sf)
{
	_scale_factor = sf;
}

