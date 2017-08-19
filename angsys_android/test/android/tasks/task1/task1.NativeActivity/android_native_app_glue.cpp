#include "pch.h"
#include "android_native_app_glue.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::android;


#define LOGI(...)  ang_debug_output_info(__VA_ARGS__)
#define LOGE(...)  ang_debug_output_error(__VA_ARGS__)

/* For debug builds, always enable the debug traces in this library */
#ifndef NDEBUG
#  define LOGV(...)  ang_debug_output_verbose(__VA_ARGS__)
#else
#  define LOGV(...)  ((void)0)
#endif

extern int main();

activity::activity(ANativeActivity* na, ibuffer_t savedState)
{
	destroyRequested = 0;

	_activity = na;
	_activity->callbacks->onDestroy = onDestroy;
	_activity->callbacks->onStart = onStart;
	_activity->callbacks->onResume = onResume;
	_activity->callbacks->onSaveInstanceState = onSaveInstanceState;
	_activity->callbacks->onPause = onPause;
	_activity->callbacks->onStop = onStop;
	_activity->callbacks->onConfigurationChanged = onConfigurationChanged;
	_activity->callbacks->onLowMemory = onLowMemory;
	_activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
	_activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
	_activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
	_activity->callbacks->onInputQueueCreated = onInputQueueCreated;
	_activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;

	_saved_state = savedState;
	_mutex = new ang::core::async::mutex();
	_cond = new ang::core::async::cond();

	int msgpipe[2];
	if (pipe(msgpipe)) {
		string error = "";
		error->format("could not create pipe: %s", strerror(errno));
		throw exception(except_code::custom, error);
	}
	msgread = msgpipe[0];
	msgwrite = msgpipe[1];

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&thread, &attr, &activity::activity_entry, this);

	// Wait for thread to start.
	_mutex->lock();
	_cond->waitfor(_mutex, [&]() { return !running; });
	_mutex->unlock();
}

activity::~activity()
{
	_mutex->lock();
	write_cmd(APP_CMD_DESTROY);
	_cond->waitfor(_mutex, [&]() { return !destroyed; });
	_mutex->unlock();
	::close(msgread);
	::close(msgwrite);
	//_the_activity = null;
}

uint activity::read_cmd() {
	uint cmd;
    if (read(msgread, &cmd, sizeof(cmd)) == sizeof(cmd)) {
        switch (cmd) {
            case APP_CMD_SAVE_STATE:
				_saved_state = null;
                break;
        }
        return cmd;
    } else {
        LOGE("No data on command pipe!");
    }
    return -1;
}

void activity::pre_execute_comand(uint cmd)
{
	switch (cmd) {
	case APP_CMD_INPUT_CHANGED:
		LOGV("APP_CMD_INPUT_CHANGED\n");
		_mutex->lock();
		if (_input_queue != null) {
			AInputQueue_detachLooper(_input_queue);
		}
		_input_queue = pendingInputQueue;
		if (_input_queue != null) {
			LOGV("Attaching input queue to looper");
			AInputQueue_attachLooper(_input_queue,
				_looper, LOOPER_ID_INPUT, null,
				on_input_event_handler.get());
		}
		_cond->signal();
		_mutex->unlock();
		break;

	case APP_CMD_INIT_WINDOW:
		LOGV("APP_CMD_INIT_WINDOW\n");
		_mutex->lock();
		_window = pendingWindow;
		_cond->signal();
		_mutex->unlock();
		break;

	case APP_CMD_TERM_WINDOW:
		LOGV("APP_CMD_TERM_WINDOW\n");
		_cond->signal();
		break;

	case APP_CMD_RESUME:
	case APP_CMD_START:
	case APP_CMD_PAUSE:
	case APP_CMD_STOP:
		LOGV("activityState=%d\n", cmd);
		_mutex->lock();
		activityState = cmd;
		_cond->signal();
		_mutex->unlock();
		break;

	case APP_CMD_CONFIG_CHANGED:
		LOGV("APP_CMD_CONFIG_CHANGED\n");
		AConfiguration_fromAssetManager(_config,
			_activity->assetManager);
		// print_cur_config(app);
		break;

	case APP_CMD_DESTROY:
		LOGV("APP_CMD_DESTROY\n");
		destroyRequested = 1;
		break;
	}
}

void activity::post_execute_comand(uint cmd) {
	switch (cmd) {
	case APP_CMD_TERM_WINDOW:
		LOGV("APP_CMD_TERM_WINDOW\n");
		_mutex->lock();
		_window = null;
		_cond->signal();
		_mutex->unlock();
		break;

	case APP_CMD_SAVE_STATE:
		LOGV("APP_CMD_SAVE_STATE\n");
		_mutex->lock();
		stateSaved = 1;
		_cond->signal();
		_mutex->unlock();
		break;

	case APP_CMD_RESUME:
		//free_saved_state(app);
		break;
	}
}

void activity::close()
{
	LOGV("android_app_destroy!");
	//free_saved_state(app);
	_mutex->lock();
	if (_input_queue != null) {
		AInputQueue_detachLooper(_input_queue);
	}
	AConfiguration_delete(_config);
	destroyed = 1;
	_cond->signal();
	_mutex->unlock();
}

void activity::process_input() {
    AInputEvent* event = null;
	while (AInputQueue_getEvent(_input_queue, &event) >= 0) {
		LOGV("New input event: type=%d\n", AInputEvent_getType(event));
		if (AInputQueue_preDispatchEvent(_input_queue, event)) {
			continue;
		}
		int32_t handled = 0;
		for (dword h : input_event(this, event))
		{
			if (h != 0) {
				handled = 1;
				break;
			}
		}
		AInputQueue_finishEvent(_input_queue, event, handled);
	}
}

void activity::process_command() {
    uint cmd = read_cmd();
    pre_execute_comand( cmd);
	command_event(this, cmd);
    post_execute_comand(cmd);
}

void* activity::activity_entry(void* param) {
    activity_t activity = (android::activity*)param;

	activity->_config = AConfiguration_new();
    AConfiguration_fromAssetManager(activity->_config, activity->_activity->assetManager);

    //print_cur_config(app);

	activity->on_command_event_handler = core::delegates::function<void(void)>(activity.get(), &activity::process_command);
	activity->on_input_event_handler = core::delegates::function<void(void)>(activity.get(), &activity::process_input);

    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_addFd(looper, activity->msgread, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, null,
            activity->on_command_event_handler.get());
	activity->_looper = looper;

	activity->_mutex->lock();
	activity->running = 1;
	activity->_cond->signal();
	activity->_mutex->unlock();

	main();

	activity->close();
    return null;
}

// --------------------------------------------------------------------
// Native activity interaction (called from main thread)
// --------------------------------------------------------------------


void activity::write_cmd(uint cmd)
{
    if (write(msgwrite, &cmd, sizeof(cmd)) != sizeof(cmd)) {
        LOGE("Failure writing app cmd: %s\n", strerror(errno));
    }
}

void activity::set_input(AInputQueue* input_queue)
{
	_mutex->lock();
	pendingInputQueue = input_queue;
	write_cmd(APP_CMD_INPUT_CHANGED);
	while (input_queue != pendingInputQueue) {
		_cond->wait(_mutex);
	}
	_mutex->unlock();
}

void activity::set_window(ANativeWindow* window)
{
	_mutex->lock();
	if (pendingWindow != null) {
		write_cmd(APP_CMD_TERM_WINDOW);
	}
	pendingWindow = window;
	if (window != null) {
		write_cmd(APP_CMD_INIT_WINDOW);
	}
	while (window != pendingWindow) {
		_cond->wait(_mutex);
	}
	_mutex->unlock();
}

void activity::set_activity_state(uint cmd) {
   _mutex->lock();
    write_cmd(cmd);
    while (activityState != cmd) {
       _cond->wait(_mutex);
    }
   _mutex->unlock();
}


void activity::onDestroy(ANativeActivity* activity) {
    LOGV("Destroy: %p\n", activity);
	activity::release_instance();
}

void activity::onStart(ANativeActivity* activity) {
    LOGV("Start: %p\n", activity);
    ((android::activity*)activity->instance)->set_activity_state(APP_CMD_START);
}

void activity::onResume(ANativeActivity* activity) {
    LOGV("Resume: %p\n", activity);
	((android::activity*)activity->instance)->set_activity_state(APP_CMD_RESUME);
}

void* activity::onSaveInstanceState(ANativeActivity* activity, size_t* outLen) {
    activity_t app = (android::activity*)activity->instance;
    void* savedState = null;

    LOGV("SaveInstanceState: %p\n", activity);
   app->_mutex->lock();
    app->stateSaved = 0;
	app->write_cmd(APP_CMD_SAVE_STATE);
    while (!app->stateSaved) {
       app->_cond->wait(app->_mutex);
    }

    if (app->_saved_state != null) {
        savedState = app->_saved_state->buffer_ptr();
        *outLen = app->_saved_state->buffer_size();
       // app->savedState = null;
       // app->savedStateSize = 0;
    }

   app->_mutex->unlock();

    return savedState;
}

void activity::onPause(ANativeActivity* activity) {
    LOGV("Pause: %p\n", activity);
    ((android::activity*)activity->instance)->set_activity_state(APP_CMD_PAUSE);
}

void activity::onStop(ANativeActivity* activity) {
    LOGV("Stop: %p\n", activity);
    ((android::activity*)activity->instance)->set_activity_state(APP_CMD_STOP);
}

void activity::onConfigurationChanged(ANativeActivity* activity) {
    LOGV("ConfigurationChanged: %p\n", activity);
	((android::activity*)activity->instance)->write_cmd(APP_CMD_CONFIG_CHANGED);
}

void activity::onLowMemory(ANativeActivity* activity) {
    LOGV("LowMemory: %p\n", activity);
	((android::activity*)activity->instance)->write_cmd(APP_CMD_LOW_MEMORY);
}

void activity::onWindowFocusChanged(ANativeActivity* activity, int focused) {
    LOGV("WindowFocusChanged: %p -- %d\n", activity, focused);
	((android::activity*)activity->instance)->write_cmd(focused ? APP_CMD_GAINED_FOCUS : APP_CMD_LOST_FOCUS);     
}

void activity::onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window) {
    LOGV("NativeWindowCreated: %p -- %p\n", activity, window);
	((android::activity*)activity->instance)->set_window(window);
}

void activity::onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window) {
    LOGV("NativeWindowDestroyed: %p -- %p\n", activity, window);
	((android::activity*)activity->instance)->set_window(null);
}

void activity::onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue) {
    LOGV("InputQueueCreated: %p -- %p\n", activity, queue);
	((android::activity*)activity->instance)->set_input(queue);
}

void activity::onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue) {
    LOGV("InputQueueDestroyed: %p -- %p\n", activity, queue);
	((android::activity*)activity->instance)->set_input(null);
}

void ANativeActivity_onCreate(ANativeActivity* activity,
        void* savedState, size_t savedStateSize) {
    LOGV("Creating: %p\n", activity);

	activity_t main_activity = android::activity::instance(activity, new(savedStateSize)buffer(savedStateSize, savedState));
	activity->instance = main_activity.get();
}
