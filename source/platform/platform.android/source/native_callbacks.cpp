#include "pch.h"
#include "ang/platform/android/android_platform.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::android;

extern "C" {
	void NativeCallBack_OnDestroy(ANativeActivity* nativeActivity)
	{
		ANG_LOGI("Running: NativeCallBack_OnDestroy");
		reinterpret_cast<activity*>(nativeActivity->instance)
			->post_msg(new events::message(events::android_msg_enum::exit_app))
			.wait(core::async::async_action_status::completed);
		ANG_LOGI("Ending: NativeCallBack_OnDestroy");
	}

	void NativeCallBack_OnStart(ANativeActivity* nativeActivity)
	{
		ANG_LOGI("Running: NativeCallBack_OnStart");
		reinterpret_cast<activity*>(nativeActivity->instance)
			->post_msg(new events::message(events::android_msg_enum::start_app))
			.wait(core::async::async_action_status::completed);
		ANG_LOGI("Ending: NativeCallBack_OnStart");
	}

	void NativeCallBack_OnResume(ANativeActivity* nativeActivity)
	{
		ANG_LOGI("Running: NativeCallBack_OnResume");
		reinterpret_cast<activity*>(nativeActivity->instance)
			->post_msg(new events::message(events::android_msg_enum::resume))
			.wait(core::async::async_action_status::completed);
		ANG_LOGI("Ending: NativeCallBack_OnResume");
	}

	void* NativeCallBack_OnSaveInstanceState(ANativeActivity* nativeActivity, size_t* size)
	{
		ANG_LOGI("Running: NativeCallBack_OnSaveInstanceState");
		*size = 0;
		ANG_LOGI("Ending: NativeCallBack_OnSaveInstanceState");
		return nullptr;
	}

	void NativeCallBack_OnPause(ANativeActivity* nativeActivity)
	{
		ANG_LOGI("Running: NativeCallBack_OnPause");
		reinterpret_cast<activity*>(nativeActivity->instance)
			->post_msg(new events::message(events::android_msg_enum::pause))
			.wait(core::async::async_action_status::completed);
		ANG_LOGI("Ending: NativeCallBack_OnPause");
	}

	void NativeCallBack_OnStop(ANativeActivity* nativeActivity)
	{
		ANG_LOGI("Running: NativeCallBack_OnStop");
		reinterpret_cast<activity*>(nativeActivity->instance)
			->post_msg(new events::message(events::android_msg_enum::stop))
			.wait(core::async::async_action_status::completed);
		ANG_LOGI("Ending: NativeCallBack_OnStop");
	}

	void NativeCallBack_OnConfigurationChanged(ANativeActivity* nativeActivity)
	{
		ANG_LOGI("Running: NativeCallBack_OnConfigurationChanged");
		reinterpret_cast<activity*>(nativeActivity->instance)
			->post_msg(new events::message(events::android_msg_enum::config_change))
			.wait(core::async::async_action_status::completed);
		ANG_LOGI("Ending: NativeCallBack_OnConfigurationChanged");
	}

	void NativeCallBack_OnLowMemory(ANativeActivity* nativeActivity)
	{
		ANG_LOGI("Running: NativeCallBack_OnLowMemory");
		reinterpret_cast<activity*>(nativeActivity->instance)
			->post_msg(new events::message(events::android_msg_enum::low_memory))
			.wait(core::async::async_action_status::completed);
		ANG_LOGI("Ending: NativeCallBack_OnLowMemory");
	}

	void NativeCallBack_OnWindowFocusChanged(ANativeActivity* nativeActivity, int focused)
	{
		ANG_LOGI("Running: NativeCallBack_OnWindowFocusChanged");
		reinterpret_cast<activity*>(nativeActivity->instance)
			->post_msg(new events::message(events::android_msg_enum::activate, (pointer)focused))
			.wait(core::async::async_action_status::completed);
		ANG_LOGI("Ending: NativeCallBack_OnWindowFocusChanged");
	}

	void NativeCallBack_OnNativeWindowCreated(ANativeActivity* nativeActivity, ANativeWindow* nativeWindow)
	{
		ANG_LOGI("Running: NativeCallBack_OnNativeWindowCreated");
		reinterpret_cast<activity*>(nativeActivity->instance)
			->post_msg(new events::message(events::android_msg_enum::created, nativeWindow))
			.wait(core::async::async_action_status::completed);
		ANG_LOGI("Ending: NativeCallBack_OnNativeWindowCreated");
	}

	void NativeCallBack_OnNativeWindowDestroyed(ANativeActivity* nativeActivity, ANativeWindow* nativeWindow)
	{
		ANG_LOGI("Running: NativeCallBack_OnNativeWindowDestroyed");
		reinterpret_cast<activity*>(nativeActivity->instance)
			->post_msg(new events::message(events::android_msg_enum::destroyed, nativeWindow))
			.wait(core::async::async_action_status::completed);
		ANG_LOGI("Ending: NativeCallBack_OnNativeWindowDestroyed");
	}

	void NativeCallBack_OnNativeWindowResized(ANativeActivity* nativeActivity, ANativeWindow* nativeWindow)
	{
		ANG_LOGI("Running: NativeCallBack_OnNativeWindowResized");
		reinterpret_cast<activity*>(nativeActivity->instance)
			->post_msg(new events::message(events::android_msg_enum::size, nativeWindow))
			.wait(core::async::async_action_status::completed);
		ANG_LOGI("Ending: NativeCallBack_OnNativeWindowResized");
	}

	void NativeCallBack_OnNativeWindowRedrawNeeded(ANativeActivity* nativeActivity, ANativeWindow* nativeWindow)
	{
		ANG_LOGI("Running: NativeCallBack_OnNativeWindowRedrawNeeded");
		//auto _activity = reinterpret_cast<activity*>(nativeActivity->instance);
		//events::message_t msg = new events::message(events::android_msg_enum::WindowsRedrawNeeded, nativeWindow);
		//core::async::iasync_t<dword> async = _activity->post_msg(msg);
		//if (async)async->wait(core::async::async_action_status::completed, -1);
		ANG_LOGI("Ending: NativeCallBack_OnNativeWindowRedrawNeeded");
	}

	void NativeCallBack_OnContentRectChanged(ANativeActivity* nativeActivity, const ARect* rect)
	{
		ANG_LOGI("Running: NativeCallBack_OnContentRectChanged");
		//auto _activity = reinterpret_cast<activity*>(nativeActivity->instance);
		//events::message_t msg = new events::message(events::android_msg_enum::WindowsRedrawNeeded
		//	, new Drawing::Rect(rect->left, rect->top, rect->right, rect->bottom));
		//core::async::iasync_t<dword> async = _activity->post_msg(msg);
		//if (async)async->wait(core::async::async_action_status::completed, -1);
		ANG_LOGI("Ending: NativeCallBack_OnContentRectChanged");
	}

	void NativeCallBack_OnInputQueueCreated(ANativeActivity* nativeActivity, AInputQueue* inputQueue)
	{
		ANG_LOGI("Running: NativeCallBack_OnInputQueueCreated");
		reinterpret_cast<activity*>(nativeActivity->instance)
			->post_msg(new events::message(events::android_msg_enum::input_device_crerated, inputQueue))
			.wait(core::async::async_action_status::completed);
		ANG_LOGI("Ending: NativeCallBack_OnInputQueueCreated");
	}

	void NativeCallBack_OnInputQueueDestroyed(ANativeActivity* nativeActivity, AInputQueue* inputQueue)
	{
		ANG_LOGI("Running: NativeCallBack_OnInputQueueDestroyed");
		reinterpret_cast<activity*>(nativeActivity->instance)
			->post_msg(new events::message(events::android_msg_enum::input_device_destroyed, inputQueue))
			.wait(core::async::async_action_status::completed);
		ANG_LOGI("Ending: NativeCallBack_OnInputQueueDestroyed");
	}
};
