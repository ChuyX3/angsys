/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "threaded_app", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "threaded_app", __VA_ARGS__))

/* For debug builds, always enable the debug traces in this library */
#ifndef NDEBUG
#  define LOGV(...)  ((void)__android_log_print(ANDROID_LOG_VERBOSE, "threaded_app", __VA_ARGS__))
#else
#  define LOGV(...)  ((void)0)
#endif

static void free_saved_state(application_t app) {
	app->mutex.lock();
    if (app->savedState != NULL) {
        free(app->savedState);
		app->savedState = NULL;
		app->savedStateSize = 0;
    }
	app->mutex.unlock();
}

int8_t android_app_read_cmd(application_t app) {
    int8_t cmd;
    if (read(app->msgread, &cmd, sizeof(cmd)) == sizeof(cmd)) {
        switch (cmd) {
            case APP_CMD_SAVE_STATE:
                free_saved_state(app);
                break;
        }
        return cmd;
    } else {
        LOGE("No data on command pipe!");
    }
    return -1;
}

static void print_cur_config(application_t app) {
    char lang[2], country[2];
    AConfiguration_getLanguage(app->config, lang);
    AConfiguration_getCountry(app->config, country);

    LOGV("Config: mcc=%d mnc=%d lang=%c%c cnt=%c%c orien=%d touch=%d dens=%d "
            "keys=%d nav=%d keysHid=%d navHid=%d sdk=%d size=%d long=%d "
            "modetype=%d modenight=%d",
            AConfiguration_getMcc(app->config),
            AConfiguration_getMnc(app->config),
            lang[0], lang[1], country[0], country[1],
            AConfiguration_getOrientation(app->config),
            AConfiguration_getTouchscreen(app->config),
            AConfiguration_getDensity(app->config),
            AConfiguration_getKeyboard(app->config),
            AConfiguration_getNavigation(app->config),
            AConfiguration_getKeysHidden(app->config),
            AConfiguration_getNavHidden(app->config),
            AConfiguration_getSdkVersion(app->config),
            AConfiguration_getScreenSize(app->config),
            AConfiguration_getScreenLong(app->config),
            AConfiguration_getUiModeType(app->config),
            AConfiguration_getUiModeNight(app->config));
}

void android_app_pre_exec_cmd(application_t app, int8_t cmd) {
    switch (cmd) {
        case APP_CMD_INPUT_CHANGED:
            LOGV("APP_CMD_INPUT_CHANGED\n");
            app->mutex.lock();
            if (app->inputQueue != NULL) {
                AInputQueue_detachLooper(app->inputQueue);
            }
            app->inputQueue = app->pendingInputQueue;
            if (app->inputQueue != NULL) {
                LOGV("Attaching input queue to looper");
                AInputQueue_attachLooper(app->inputQueue,
                        app->looper, LOOPER_ID_INPUT, NULL,
                        &app->inputPollSource);
            }
            app->cond.signal();
            app->mutex.unlock();
            break;

        case APP_CMD_INIT_WINDOW:
            LOGV("APP_CMD_INIT_WINDOW\n");
            app->mutex.lock();
            app->window = app->pendingWindow;
            app->cond.signal();
            app->mutex.unlock();
            break;

        case APP_CMD_TERM_WINDOW:
            LOGV("APP_CMD_TERM_WINDOW\n");
            app->cond.signal();
            break;

        case APP_CMD_RESUME:
        case APP_CMD_START:
        case APP_CMD_PAUSE:
        case APP_CMD_STOP:
            LOGV("activityState=%d\n", cmd);
            app->mutex.lock();
            app->activityState = cmd;
            app->cond.signal();
            app->mutex.unlock();
            break;

        case APP_CMD_CONFIG_CHANGED:
            LOGV("APP_CMD_CONFIG_CHANGED\n");
            AConfiguration_fromAssetManager(app->config,
                    app->activity->assetManager);
            print_cur_config(app);
            break;

        case APP_CMD_DESTROY:
            LOGV("APP_CMD_DESTROY\n");
            app->destroyRequested = 1;
            break;
    }
}

void android_app_post_exec_cmd(application_t app, int8_t cmd) {
    switch (cmd) {
        case APP_CMD_TERM_WINDOW:
            LOGV("APP_CMD_TERM_WINDOW\n");
            app->mutex.lock();
            app->window = NULL;
            app->cond.signal();
            app->mutex.unlock();
            break;

        case APP_CMD_SAVE_STATE:
            LOGV("APP_CMD_SAVE_STATE\n");
            app->mutex.lock();
            app->stateSaved = 1;
            app->cond.signal();
            app->mutex.unlock();
            break;

        case APP_CMD_RESUME:
            free_saved_state(app);
            break;
    }
}

static void android_app_destroy(application_t app) {
    LOGV("android_app_destroy!");
    free_saved_state(app);
    app->mutex.lock();
    if (app->inputQueue != NULL) {
        AInputQueue_detachLooper(app->inputQueue);
    }
    AConfiguration_delete(app->config);
    app->destroyed = 1;
    app->cond.signal();
    app->mutex.unlock();
    // Can't touch app object after this.
}

static void process_input(application_t app, struct android_poll_source* source) {
    AInputEvent* event = NULL;
    while (AInputQueue_getEvent(app->inputQueue, &event) >= 0) {
        LOGV("New input event: type=%d\n", AInputEvent_getType(event));
        if (AInputQueue_preDispatchEvent(app->inputQueue, event)) {
            continue;
        }
        int32_t handled = 0;
        if (app->onInputEvent != NULL) handled = app->onInputEvent(app, event);
        AInputQueue_finishEvent(app->inputQueue, event, handled);
    }
}

static void process_cmd(application_t app, struct android_poll_source* source) {
    int8_t cmd = android_app_read_cmd(app);
    android_app_pre_exec_cmd(app, cmd);
    if (app->onAppCmd != NULL) app->onAppCmd(app, cmd);
    android_app_post_exec_cmd(app, cmd);
}

dword application::android_app_entry(ang::core::async::thread_t sender, ang::var_args_t args) {

    this->config = AConfiguration_new();
    AConfiguration_fromAssetManager(this->config, this->activity->assetManager);

    print_cur_config(this);

    this->cmdPollSource.id = LOOPER_ID_MAIN;
    this->cmdPollSource.app = this;
    this->cmdPollSource.process = process_cmd;
    this->inputPollSource.id = LOOPER_ID_INPUT;
    this->inputPollSource.app = this;
    this->inputPollSource.process = process_input;

    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_addFd(looper, this->msgread, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, NULL,
            &this->cmdPollSource);
    this->looper = looper;

    this->mutex.lock();
    this->running = 1;
    this->cond.signal();
    this->mutex.unlock();

    android_main(this);

    android_app_destroy(this);


    return NULL;
}

// --------------------------------------------------------------------
// Native activity interaction (called from main thread)
// --------------------------------------------------------------------

static application_t android_app_create(ANativeActivity* activity,
	void* savedState, size_t savedStateSize) {
	application_t app = new application();

	app->activity = activity;

	//pthread_mutex_init(&app->mutex, NULL);
	//pthread_cond_init(&app->cond, NULL);

	if (savedState != NULL) {
		app->savedState = malloc(savedStateSize);
		app->savedStateSize = savedStateSize;
		memcpy(app->savedState, savedState, savedStateSize);
	}

	int msgpipe[2];
	if (pipe(msgpipe)) {
		LOGE("could not create pipe: %s", strerror(errno));
		return nullptr;
	}
	app->msgread = msgpipe[0];
	app->msgwrite = msgpipe[1];

	app->thread = ang::core::async::thread::create_thread(ang::bind(app, &application::android_app_entry), null, PTHREAD_CREATE_DETACHED, null, false);

    // Wait for thread to start.
	app->mutex.lock();
    while (!app->running) {
		app->cond.wait(app->mutex);
    }
	app->mutex.unlock();

    return app;
}

static void android_app_write_cmd(application_t app, int8_t cmd) {
    if (write(app->msgwrite, &cmd, sizeof(cmd)) != sizeof(cmd)) {
        LOGE("Failure writing app cmd: %s\n", strerror(errno));
    }
}

static void android_app_set_input(application_t app, AInputQueue* inputQueue) {
    app->mutex.lock();
    app->pendingInputQueue = inputQueue;
    android_app_write_cmd(app, APP_CMD_INPUT_CHANGED);
    while (app->inputQueue != app->pendingInputQueue) {
        app->cond.wait(app->mutex);
    }
    app->mutex.unlock();
}

static void android_app_set_window(application_t app, ANativeWindow* window) {
    app->mutex.lock();
    if (app->pendingWindow != NULL) {
        android_app_write_cmd(app, APP_CMD_TERM_WINDOW);
    }
    app->pendingWindow = window;
    if (window != NULL) {
        android_app_write_cmd(app, APP_CMD_INIT_WINDOW);
    }
    while (app->window != app->pendingWindow) {
        app->cond.wait(app->mutex);
    }
    app->mutex.unlock();
}

static void android_app_set_activity_state(application_t app, int8_t cmd) {
    app->mutex.lock();
    android_app_write_cmd(app, cmd);
    while (app->activityState != cmd) {
        app->cond.wait(app->mutex);
    }
    app->mutex.unlock();
}

static void android_app_free(application_t app) {
	app->mutex.lock();
    android_app_write_cmd(app, APP_CMD_DESTROY);
    while (!app->destroyed) {
		app->cond.wait(app->mutex);
    }
	app->mutex.unlock();

    close(app->msgread);
    close(app->msgwrite);
    //pthread_cond_destroy(&app->cond);
    //pthread_mutex_destroy(&app->mutex);

	app->activity->instance = nullptr;
	app->release();
}

void application::onDestroy(ANativeActivity* activity) {
    LOGV("Destroy: %p\n", activity);
    android_app_free((application*)activity->instance);
}

void application::onStart(ANativeActivity* activity) {
    LOGV("Start: %p\n", activity);
    android_app_set_activity_state((application*)activity->instance, APP_CMD_START);
}

void application::onResume(ANativeActivity* activity) {
    LOGV("Resume: %p\n", activity);
    android_app_set_activity_state((application*)activity->instance, APP_CMD_RESUME);
}

void* application::onSaveInstanceState(ANativeActivity* activity, size_t* outLen) {
	application_t app = (application*)activity->instance;
    void* savedState = NULL;

    LOGV("SaveInstanceState: %p\n", activity);
    app->mutex.lock();
    app->stateSaved = 0;
    android_app_write_cmd(app, APP_CMD_SAVE_STATE);
    while (!app->stateSaved) {
        app->cond.wait(app->mutex);
    }

    if (app->savedState != NULL) {
        savedState = app->savedState;
        *outLen = app->savedStateSize;
        app->savedState = NULL;
        app->savedStateSize = 0;
    }

    app->mutex.unlock();

    return savedState;
}

void application::onPause(ANativeActivity* activity) {
    LOGV("Pause: %p\n", activity);
    android_app_set_activity_state((application*)activity->instance, APP_CMD_PAUSE);
}

void application::onStop(ANativeActivity* activity) {
    LOGV("Stop: %p\n", activity);
    android_app_set_activity_state((application*)activity->instance, APP_CMD_STOP);
}

void application::onConfigurationChanged(ANativeActivity* activity) {
	application_t app = (application*)activity->instance;
    LOGV("ConfigurationChanged: %p\n", activity);
    android_app_write_cmd(app, APP_CMD_CONFIG_CHANGED);
}

void application::onLowMemory(ANativeActivity* activity) {
	application_t app = (application*)activity->instance;
    LOGV("LowMemory: %p\n", activity);
    android_app_write_cmd(app, APP_CMD_LOW_MEMORY);
}

void application::onWindowFocusChanged(ANativeActivity* activity, int focused) {
    LOGV("WindowFocusChanged: %p -- %d\n", activity, focused);
    android_app_write_cmd((application*)activity->instance,
            focused ? APP_CMD_GAINED_FOCUS : APP_CMD_LOST_FOCUS);
}

void application::onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window) {
    LOGV("NativeWindowCreated: %p -- %p\n", activity, window);
    android_app_set_window((application*)activity->instance, window);
}

void application::onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window) {
    LOGV("NativeWindowDestroyed: %p -- %p\n", activity, window);
    android_app_set_window((application*)activity->instance, NULL);
}

void application::onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue) {
    LOGV("InputQueueCreated: %p -- %p\n", activity, queue);
    android_app_set_input((application*)activity->instance, queue);
}

void application::onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue) {
    LOGV("InputQueueDestroyed: %p -- %p\n", activity, queue);
    android_app_set_input((application*)activity->instance, NULL);
}

void ANativeActivity_onCreate(ANativeActivity* activity,
        void* savedState, size_t savedStateSize) {
    LOGV("Creating: %p\n", activity);
    activity->callbacks->onDestroy = &application::onDestroy;
    activity->callbacks->onStart = &application::onStart;
    activity->callbacks->onResume = &application::onResume;
    activity->callbacks->onSaveInstanceState = &application::onSaveInstanceState;
    activity->callbacks->onPause = &application::onPause;
    activity->callbacks->onStop = &application::onStop;
    activity->callbacks->onConfigurationChanged = &application::onConfigurationChanged;
    activity->callbacks->onLowMemory = &application::onLowMemory;
    activity->callbacks->onWindowFocusChanged = &application::onWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated = &application::onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed = &application::onNativeWindowDestroyed;
    activity->callbacks->onInputQueueCreated = &application::onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = &application::onInputQueueDestroyed;
	application_t app = android_app_create(activity, savedState, savedStateSize);
	activity->instance = app.get();
	app->add_ref();
}


application::application()
{

}

application::~application()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(application, ang::object);