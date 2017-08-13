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

activity_t activity::_the_activity = null;

activity::activity(ANativeActivity* na, ibuffer_t savedState)
{
	_the_activity = this;
	_activity = na;
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
	android_app_write_cmd(this, APP_CMD_DESTROY);
	_cond->waitfor(_mutex, [&]() { return !destroyed; });
	_mutex->unlock();
	close(msgread);
	close(msgwrite);
	//_the_activity = null;
}

byte activity::read_cmd() {
	byte cmd;
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

void activity::pre_execute_comand(byte cmd)
{
	switch (cmd) {
	case APP_CMD_INPUT_CHANGED:
		LOGV("APP_CMD_INPUT_CHANGED\n");
		_mutex->lock();
		if (input_queue != null) {
			AInputQueue_detachLooper(input_queue);
		}
		input_queue = pendingInputQueue;
		if (input_queue != null) {
			LOGV("Attaching input queue to looper");
			AInputQueue_attachLooper(input_queue,
				_looper, LOOPER_ID_INPUT, null,
				on_input_event_handler.get());
		}
		_cond->signal();
		_mutex->unlock();
		break;

	case APP_CMD_INIT_WINDOW:
		LOGV("APP_CMD_INIT_WINDOW\n");
		_mutex->lock();
		window = pendingWindow;
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

void activity::post_execute_comand(byte cmd) {
	switch (cmd) {
	case APP_CMD_TERM_WINDOW:
		LOGV("APP_CMD_TERM_WINDOW\n");
		_mutex->lock();
		window = null;
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

static void android_app_destroy(activity_t app) {
    LOGV("android_app_destroy!");
    //free_saved_state(app);
   app->_mutex->lock();
    if (app->input_queue != null) {
        AInputQueue_detachLooper(app->input_queue);
    }
    AConfiguration_delete(app->config);
    app->destroyed = 1;
  app->_cond->signal();
   app->_mutex->unlock();
    // Can't touch app object after this.
}

static void process_input(activity_t app) {
    AInputEvent* event = null;
    while (AInputQueue_getEvent(app->input_queue, &event) >= 0) {
        LOGV("New input event: type=%d\n", AInputEvent_getType(event));
        if (AInputQueue_preDispatchEvent(app->input_queue, event)) {
            continue;
        }
        int32_t handled = 0;
       handled = app->onInputEvent(app, event);
        AInputQueue_finishEvent(app->input_queue, event, handled);
    }
}

static void process_cmd(activity_t app) {
    int8_t cmd = android_app_read_cmd(app);
    android_app_pre_exec_cmd(app, cmd);
    app->onAppCmd(app, cmd);
    android_app_post_exec_cmd(app, cmd);
}

void* activity::activity_entry(void* param) {
    activity_t activity = (android::activity*)param;

	activity->_config = AConfiguration_new();
    AConfiguration_fromAssetManager(activity->_config, activity->_activity->assetManager);

    //print_cur_config(app);

	activity->on_command_event_handler = process_cmd;
	activity->on_input_event_handler = process_input;

    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_addFd(looper, activity->msgread, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, null,
            activity->on_command_event_handler.get());
	activity->_looper = looper;

	activity->_mutex->lock();
	activity->running = 1;
	activity->_cond->signal();
	activity->_mutex->unlock();

	main();

    android_app_destroy(app);
    return null;
}

// --------------------------------------------------------------------
// Native activity interaction (called from main thread)
// --------------------------------------------------------------------


static void android_app_write_cmd(activity_t app, int8_t cmd) {
    if (write(app->msgwrite, &cmd, sizeof(cmd)) != sizeof(cmd)) {
        LOGE("Failure writing app cmd: %s\n", strerror(errno));
    }
}

static void android_app_set_input(activity_t app, AInputQueue* input_queue) {
   app->_mutex->lock();
    app->pendingInputQueue = input_queue;
    android_app_write_cmd(app, APP_CMD_INPUT_CHANGED);
    while (app->input_queue != app->pendingInputQueue) {
       app->_cond->wait(app->_mutex);
    }
   app->_mutex->unlock();
}

static void android_app_set_window(activity_t app, ANativeWindow* window) {
   app->_mutex->lock();
    if (app->pendingWindow != null) {
        android_app_write_cmd(app, APP_CMD_TERM_WINDOW);
    }
    app->pendingWindow = window;
    if (window != null) {
        android_app_write_cmd(app, APP_CMD_INIT_WINDOW);
    }
    while (app->window != app->pendingWindow) {
       app->_cond->wait(app->_mutex);
    }
   app->_mutex->unlock();
}

static void android_app_set_activity_state(activity_t app, int8_t cmd) {
   app->_mutex->lock();
    android_app_write_cmd(app, cmd);
    while (app->activityState != cmd) {
       app->_cond->wait(app->_mutex);
    }
   app->_mutex->unlock();
}

static void android_app_free(activity_t app) {

}

static void onDestroy(ANativeActivity* activity) {
    LOGV("Destroy: %p\n", activity);
    android_app_free((android::activity*)activity->instance);
}

static void onStart(ANativeActivity* activity) {
    LOGV("Start: %p\n", activity);
    android_app_set_activity_state((android::activity*)activity->instance, APP_CMD_START);
}

static void onResume(ANativeActivity* activity) {
    LOGV("Resume: %p\n", activity);
    android_app_set_activity_state((android::activity*)activity->instance, APP_CMD_RESUME);
}

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen) {
    activity_t app = (android::activity*)activity->instance;
    void* savedState = null;

    LOGV("SaveInstanceState: %p\n", activity);
   app->_mutex->lock();
    app->stateSaved = 0;
    android_app_write_cmd(app, APP_CMD_SAVE_STATE);
    while (!app->stateSaved) {
       app->_cond->wait(app->_mutex);
    }

    if (app->savedState != null) {
        savedState = app->savedState;
        *outLen = app->savedStateSize;
        app->savedState = null;
        app->savedStateSize = 0;
    }

   app->_mutex->unlock();

    return savedState;
}

static void onPause(ANativeActivity* activity) {
    LOGV("Pause: %p\n", activity);
    android_app_set_activity_state((android::activity*)activity->instance, APP_CMD_PAUSE);
}

static void onStop(ANativeActivity* activity) {
    LOGV("Stop: %p\n", activity);
    android_app_set_activity_state((android::activity*)activity->instance, APP_CMD_STOP);
}

static void onConfigurationChanged(ANativeActivity* activity) {
    activity_t app = (android::activity*)activity->instance;
    LOGV("ConfigurationChanged: %p\n", activity);
    android_app_write_cmd(app, APP_CMD_CONFIG_CHANGED);
}

static void onLowMemory(ANativeActivity* activity) {
    activity_t app = (android::activity*)activity->instance;
    LOGV("LowMemory: %p\n", activity);
    android_app_write_cmd(app, APP_CMD_LOW_MEMORY);
}

static void onWindowFocusChanged(ANativeActivity* activity, int focused) {
    LOGV("WindowFocusChanged: %p -- %d\n", activity, focused);
    android_app_write_cmd((android::activity*)activity->instance,
            focused ? APP_CMD_GAINED_FOCUS : APP_CMD_LOST_FOCUS);
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window) {
    LOGV("NativeWindowCreated: %p -- %p\n", activity, window);
    android_app_set_window((android::activity*)activity->instance, window);
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window) {
    LOGV("NativeWindowDestroyed: %p -- %p\n", activity, window);
    android_app_set_window((android::activity*)activity->instance, null);
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue) {
    LOGV("InputQueueCreated: %p -- %p\n", activity, queue);
    android_app_set_input((android::activity*)activity->instance, queue);
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue) {
    LOGV("InputQueueDestroyed: %p -- %p\n", activity, queue);
    android_app_set_input((android::activity*)activity->instance, null);
}

void ANativeActivity_onCreate(ANativeActivity* activity,
        void* savedState, size_t savedStateSize) {
    LOGV("Creating: %p\n", activity);
    activity->callbacks->onDestroy = onDestroy;
    activity->callbacks->onStart = onStart;
    activity->callbacks->onResume = onResume;
    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onPause = onPause;
    activity->callbacks->onStop = onStop;
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;
    activity->callbacks->onLowMemory = onLowMemory;
    activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    activity->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;

	static activity_t main_activity = new android::activity(activity, new(savedStateSize)buffer(savedStateSize, savedState));

	activity->instance = main_activity.get();
}
