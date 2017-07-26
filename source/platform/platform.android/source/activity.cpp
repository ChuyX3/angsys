#include "pch.h"
#include "ang/platform/android/android_platform.h"
#include "async_msg_task.h"

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


enum looper_msg_type : uint
{
	MAIN_COMMAND = 1,
	INPUT_COMMAND = 2,
	USER_COMMAND = 3,
};


activity::activity()
	: _cond(new core::async::cond())
	, _mutex(new core::async::mutex())
	, _main_thread(core::async::thread::current_thread())
	, _worker_thread(null)
	, _native_activity(null)
	, _config(null)
	, _looper(null)
	, _input_queue(null)
	, start_app_event(this, [](events::core_msg_t msg)->bool { return msg == events::android_msg_enum::start_app; })
	, exit_app_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::exit_app; })
	, created_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::created; })
	, destroyed_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::destroyed; })
	, update_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::update; })
	, draw_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::draw; })
	, pointer_pressed_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::pointer_pressed; })
	, pointer_released_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::pointer_released; })
	, pointer_moved_event(this, [](events::core_msg_t msg)->bool{ return msg == events::android_msg_enum::pointer_moved; })
{
	_core_view = new core_view(this);
	command_handler = core::delegates::function<dword(pointer)>(this, &activity::on_dispatch_command_event);
	input_handler = core::delegates::function<dword(pointer)>(this, &activity::on_dispatch_input_event);
	_msg_pipe[0] = 0;
	_msg_pipe[1] = 0;
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
		|| icore_msg_dispatcher::is_kind_of(name));
}

bool activity::is_child_of(type_name_t name)
{
	return (name == activity::class_name()
		|| object::is_child_of(name)
		|| icore_app::is_child_of(name)
		|| icore_msg_dispatcher::is_child_of(name));
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
	else if (icore_msg_dispatcher::query_object(name, out))
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
	return _core_view.get();
}

input::ikeyboard_t activity::get_keyboard()
{
	return nullptr;
}

imessage_reciever_t activity::get_listener()const
{
	return const_cast<activity*>(this);
}


void activity::send_msg(events::message_t msg)
{
	if (is_worker_thread())
	{
		core::async::scope_locker::lock(main_mutex(), [&]()
		{
			on_pre_dispatch_message(msg);
			main_mutex()->unlock();
			on_command_event_handler(msg);
			main_mutex()->lock();
			on_post_dispatch_message(msg);
		});
		return;
	}

	core::async::iasync_t<dword> task = post_msg(msg);
	task.wait(core::async::async_action_status::completed);
	return;
}

core::async::iasync_t<dword> activity::post_msg(events::message_t msg)
{
	return core::async::scope_locker::lock(main_mutex(), [&]() -> async_msg_task_t
	{
		ANG_LOGI("%s: Posting Message", class_name().cstr());
		async_msg_task_t task = new async_msg_task(this, main_cond(), main_mutex(), msg);
		task->add_ref();
		if (write(_msg_pipe[1], (void*)task.addres_of(), sizeof(async_msg_task*)) != sizeof(async_msg_task*))
		{
			task->release();
			return nullptr;
		}
		return task;
	}).get();
}

bool activity::listen_to(events::event_t)
{
	return false;
}

bool activity::dispatch_msg()
{
	if (!is_worker_thread())
		return false;
	ANG_LOGI("%s: Dispatching Message", class_name().cstr());
	return core::async::scope_locker::lock(main_mutex(), [this]()->bool
	{
		async_msg_task_t task = nullptr;

		if (read(_msg_pipe[0], (void*)task.addres_of(), sizeof(async_msg_task*)) != sizeof(async_msg_task*))
			return false;

		if (!task->status().is_active(core::async::async_action_status::canceled))
		{
			task->_status = core::async::async_action_status::running;
			on_pre_dispatch_message(task->_msg);
			main_mutex()->unlock();
			on_command_event_handler(task->_msg);
			main_mutex()->lock();
			on_post_dispatch_message(task->_msg);
			task->_status = core::async::async_action_status::completed;
		}
		task->_result = task->_msg->result();
		task->_cond->signal();
		return true;
	});
}

core::async::iasync_task* activity::run_async(ANativeActivity* _activity)
{
	if (_worker_thread != nullptr)
		return nullptr;

	if (_activity == nullptr)
		return nullptr;

	_main_thread = core::async::thread::current_thread();

	core::async::cond_t cond = new core::async::cond();
	core::async::scope_locker lock = main_mutex();

	_worker_thread = new core::async::dispatcher_thread();

	_worker_thread->start_event += [=](objptr, pointer)
	{
		//AssetsManager::Instance()->handle = activity->assetManager;
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

		if (_config == nullptr)
			_config = AConfiguration_new();

		AConfiguration_fromAssetManager(_config, _activity->assetManager);

		pipe(_msg_pipe);
		_looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
		ALooper_addFd(_looper, _msg_pipe[0], MAIN_COMMAND, ALOOPER_EVENT_INPUT, NULL, command_handler.get());

		cond->signal();

		if (!init_app())
			_worker_thread->cancel();
	};

	_worker_thread->end_event += [=](objptr, pointer) { exit_app(); };

	_worker_thread->start([=](pointer args)->dword
	{
		int ident = 0;
		int events = 0;
		core::delegates::function_data<dword(pointer)>* command = null;

		do
		{
			if (!_worker_thread->status().is_active(ang::core::async::async_action_status::running))
				break;

			if (command)
				command->invoke(null);

			update_app();

		} while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&command)) >= 0);
		return 0;
	}, null);

	cond->wait(main_mutex());
	return _worker_thread.get();
}

bool activity::init_app()
{
	return true;
}

void activity::update_app()
{

}

bool activity::exit_app()
{
	return true;
}

dword activity::on_dispatch_command_event(pointer)
{
	return dispatch_msg() ? 0 : -1;
}

dword activity::on_dispatch_input_event(pointer)
{
	AInputEvent* event = NULL;
	while (AInputQueue_getEvent(_input_queue, &event) >= 0)
	{
		if (AInputQueue_preDispatchEvent(_input_queue, event) != 0)
			continue;
		events::message_t msg = new events::message(events::android_msg_enum::touch_input, (pointer)AInputEvent_getType(event), event);
		on_command_event_handler(msg);
		AInputQueue_finishEvent(_input_queue, event, (int)msg->result());
	}
	return 0;
}

bool activity::on_pre_dispatch_message(events::message_t msg)
{
	switch (msg->msg())
	{
	case events::android_msg_enum::input_device_crerated:
	{
		if (_input_queue != null)
		{
			AInputQueue_detachLooper(_input_queue);
		}
		_input_queue = reinterpret_cast<AInputQueue*>(msg->arg1());
		if (_input_queue != NULL) {
			AInputQueue_attachLooper(_input_queue, _looper, INPUT_COMMAND, NULL, input_handler.get());
		}
		msg->result(1);
		return true;
	}
	case events::android_msg_enum::input_device_destroyed:
	{
		auto input_queue = reinterpret_cast<AInputQueue*>(msg->arg1());
		if (_input_queue == input_queue)
			_input_queue = null;
			AInputQueue_detachLooper(input_queue);
		
		msg->result(1);
		return true;
	}
	}
	return false;
}

bool activity::on_post_dispatch_message(events::message_t msg)
{
	return false;
}


void activity::on_command_event_handler(events::message_t)
{

}

void activity::on_init_window(events::message_t)
{

}

void activity::on_term_window(events::message_t)
{

}

void activity::on_window_resized(events::message_t)
{

}


void activity::on_window_redraw_needed(events::message_t)
{

}

void activity::on_content_rect_changed(events::message_t)
{

}

void activity::on_get_focus(events::message_t)
{

}

void activity::on_lost_focus(events::message_t)
{

}

void activity::on_config_changed(events::message_t)
{

}

void activity::on_low_memory(events::message_t)
{

}

void activity::on_start(events::message_t)
{

}

void activity::on_resume(events::message_t)
{

}

void activity::on_save_state(events::message_t)
{

}

void activity::on_pause(events::message_t)
{

}

void activity::on_stop(events::message_t)
{

}

void activity::on_destroy(events::message_t)
{

}

void activity::on_input_event(events::message_t)
{

}

void activity::on_update(events::message_t)
{

}

void activity::on_draw(events::message_t)
{

}
