#include "pch.h"
#include "ang/platform/android/android_platform.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::android;


extern "C" {
	void NativeCallBack_OnDestroy(ANativeActivity* nativeActivity);
	void NativeCallBack_OnStart(ANativeActivity* nativeActivity);
	void NativeCallBack_OnResume(ANativeActivity* nativeActivity);
	void* NativeCallBack_OnSaveInstanceState(ANativeActivity* nativeActivity, size_t* size);
	void NativeCallBack_OnPause(ANativeActivity* nativeActivity);
	void NativeCallBack_OnStop(ANativeActivity* nativeActivity);
	void NativeCallBack_OnConfigurationChanged(ANativeActivity* nativeActivity);
	void NativeCallBack_OnLowMemory(ANativeActivity* nativeActivity);
	void NativeCallBack_OnWindowFocusChanged(ANativeActivity* nativeActivity, int focused);
	void NativeCallBack_OnNativeWindowCreated(ANativeActivity* nativeActivity, ANativeWindow* nativeWindow);
	void NativeCallBack_OnNativeWindowDestroyed(ANativeActivity* nativeActivity, ANativeWindow* nativeWindow);
	void NativeCallBack_OnNativeWindowResized(ANativeActivity* nativeActivity, ANativeWindow* nativeWindow);
	void NativeCallBack_OnNativeWindowRedrawNeeded(ANativeActivity* nativeActivity, ANativeWindow* nativeWindow);
	void NativeCallBack_OnContentRectChanged(ANativeActivity* nativeActivity, const ARect* rect);
	void NativeCallBack_OnInputQueueCreated(ANativeActivity* nativeActivity, AInputQueue* inputQueue);
	void NativeCallBack_OnInputQueueDestroyed(ANativeActivity* nativeActivity, AInputQueue* inputQueue);
};


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

				virtual pointer get_core_context_handle()const { return nullptr; }
				virtual bool bind_graphic_native_surface(pointer) { return false; };
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

activity::activity()
	: _cond(new core::async::cond())
	, _mutex(new core::async::mutex())
	, _main_thread(core::async::thread::current_thread())
	, _input_thread(null)
	, _render_thread(null)
	, _input_async_action(null)
	, _render_async_action(null)
	, _status(core::async::async_action_status::starting)
	, _dispatcher(new dispatcher(this))
	, _native_activity(null)
	, _native_window(null)
	, _config(null)
	, _client_area(0,0,0,0)
	, _scale_factor(1,1)
	, start_app_event(this, [](events::core_msg_t msg)->bool { return msg == events::android_msg_enum::start_app; })
	, exitAppEvent(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::exit_app; })
	, created_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::created; })
	, destroyed_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::destroyed; })
	, update_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::update; })
	, draw_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::draw; })
	, pointer_pressed_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::pointer_pressed; })
	, pointer_released_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::pointer_released; })
	, pointer_moved_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::pointer_moved; })
	, begin_rendering_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::begin_drawing; })
	, end_rendering_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::end_drawing; })
{

}

activity::~activity()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::platform::android::activity)
ANG_IMPLEMENT_OBJECTNAME(ang::platform::android::activity)


bool activity::is_kind_of(type_name_t name)const
{
	return (name == activity::class_name()
		|| object::is_kind_of(name)
		|| icore_app::is_kind_of(name)
		|| icore_view::is_kind_of(name));
}

bool activity::is_child_of(type_name_t name)
{
	return (name == activity::class_name()
		|| object::is_child_of(name)
		|| icore_app::is_child_of(name)
		|| icore_view::is_child_of(name));
}


bool activity::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == nullptr)
		return false;
	if (name == activity::class_name())
	{
		*out = static_cast<activity*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (icore_app::query_object(name, out))
	{
		return true;
	}
	else if (icore_view::query_object(name, out))
	{
		return true;
	}
	return false;
}

pointer activity::get_core_app_handle()const
{
	return _native_activity;
}

icore_view_t activity::get_main_core_view()
{
	return this;
}

input::ikeyboard_t activity::get_keyboard()
{
	return nullptr;
}

imessage_reciever_t activity::get_listener()const
{
	return _dispatcher.get();
}

pointer activity::get_core_view_handle()const
{
	return _native_window;
}

icore_context_t activity::get_core_context()const
{
	return new core_context();
}

graph::size<float> activity::get_core_view_size()const
{
	return{ _client_area.width, _client_area.height() };
}

graph::size<float> activity::get_core_view_scale_factor()const
{
	return _scale_factor;
}

core::async::iasync<dword>* activity::run_async(ANativeActivity* _activity)
{
	if (_input_thread != nullptr)
		return nullptr;

	if (_activity == nullptr)
		return nullptr;

//	AssetsManager::Instance()->handle = activity->assetManager;

	_activity->callbacks->onDestroy = &NativeCallBack_OnDestroy;
	_activity->callbacks->onStart = &NativeCallBack_OnStart;
	_activity->callbacks->onResume = &NativeCallBack_OnResume;
	_activity->callbacks->onSaveInstanceState = &NativeCallBack_OnSaveInstanceState;
	_activity->callbacks->onPause = &NativeCallBack_OnPause;
	_activity->callbacks->onStop = &NativeCallBack_OnStop;
	_activity->callbacks->onConfigurationChanged = &NativeCallBack_OnConfigurationChanged;
	_activity->callbacks->onLowMemory = &NativeCallBack_OnLowMemory;
	_activity->callbacks->onWindowFocusChanged = &NativeCallBack_OnWindowFocusChanged;
	_activity->callbacks->onNativeWindowCreated = &NativeCallBack_OnNativeWindowCreated;
	_activity->callbacks->onNativeWindowDestroyed = &NativeCallBack_OnNativeWindowDestroyed;
	_activity->callbacks->onInputQueueCreated = &NativeCallBack_OnInputQueueCreated;
	_activity->callbacks->onInputQueueDestroyed = &NativeCallBack_OnInputQueueDestroyed;
	_activity->instance = this;

	_native_activity = _activity;
	_native_window = nullptr;
	if (_config == nullptr)
		_config = AConfiguration_new();
	AConfiguration_fromAssetManager(_config, nativeActivity->assetManager);
	_main_thread = core::async::thread::current_thread();

	return core::async::async_task<dword>::run_async(
		core::delegates::function<dword(core::async::iasync<dword>*, var_args_t)>(this, &activity::on_input_dispatcher_event_handler));
}

Core::Threading::IAsyncAction<DWord>* Activity::UpdateActivity(ANativeActivity* _activity)
{
	//auto runningAsync = new ActivityAsyncAction(this, Core::Threading::AsyncActionStatus::Running);
	if (_activity == nullptr)
		return nullptr;

	switch (status)
	{
	case Ang::Core::Threading::AsyncActionStatus::Stopped:
	case Ang::Core::Threading::AsyncActionStatus::Completed:
		status = Ang::Core::Threading::AsyncActionStatus::Stopped;
		return RunAsync(_activity);
		break;
	case Ang::Core::Threading::AsyncActionStatus::Canceled:
		cond.While(mutex, [&]()->Bool { return  status == Ang::Core::Threading::AsyncActionStatus::Completed; });
		return RunAsync(_activity);
		break;

	case Ang::Core::Threading::AsyncActionStatus::Running:
		inputAsyncAction->Suspend();
		cond.While(mutex, [&]()->Bool { return  status == Ang::Core::Threading::AsyncActionStatus::Suspended; });
		break;
	case Ang::Core::Threading::AsyncActionStatus::Suspended:
		break;

	default:
		return nullptr;
		break;
	}

	mutex.Lock();

	AssetsManager::Instance()->handle = _activity->assetManager;

	_activity->callbacks->onDestroy = &NativeCallBack_OnDestroy;
	_activity->callbacks->onStart = &NativeCallBack_OnStart;
	_activity->callbacks->onResume = &NativeCallBack_OnResume;
	_activity->callbacks->onSaveInstanceState = &NativeCallBack_OnSaveInstanceState;
	_activity->callbacks->onPause = &NativeCallBack_OnPause;
	_activity->callbacks->onStop = &NativeCallBack_OnStop;
	_activity->callbacks->onConfigurationChanged = &NativeCallBack_OnConfigurationChanged;
	_activity->callbacks->onLowMemory = &NativeCallBack_OnLowMemory;
	_activity->callbacks->onWindowFocusChanged = &NativeCallBack_OnWindowFocusChanged;
	_activity->callbacks->onNativeWindowCreated = &NativeCallBack_OnNativeWindowCreated;
	_activity->callbacks->onNativeWindowDestroyed = &NativeCallBack_OnNativeWindowDestroyed;
	_activity->callbacks->onInputQueueCreated = &NativeCallBack_OnInputQueueCreated;
	_activity->callbacks->onInputQueueDestroyed = &NativeCallBack_OnInputQueueDestroyed;
	_activity->instance = this;

	nativeActivity = _activity;
	nativeWindow = nullptr;
	if (config == nullptr)
		config = AConfiguration_new();
	AConfiguration_fromAssetManager(config, nativeActivity->assetManager);
	mainThread = Core::Threading::Thread::CurrentThread();
	auto runningAsync = new ActivityAsyncAction(this, Core::Threading::AsyncActionStatus::Running); //wait for running
	if (dispatcher == nullptr)
	{
		dispatcher = new Android::Dispatcher(this);
		ReferenceObj(dispatcher);
		dispatcher->CreatePipe();
	}
	inputAsyncAction->Resume();
	cond.Signal(mutex);
	mutex.Unlock();
	return runningAsync;
}