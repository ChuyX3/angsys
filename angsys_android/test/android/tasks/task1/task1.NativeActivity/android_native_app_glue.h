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

#ifndef _ANDROID_NATIVE_APP_GLUE_H
#define _ANDROID_NATIVE_APP_GLUE_H

#include <poll.h>
#include <pthread.h>
#include <sched.h>

#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

#include <angsys.h>
#include <ang/core/async.h>

namespace ang
{
	namespace platform
	{
		namespace android
		{
			class activity;
			typedef ang::object_wrapper<activity> activity_t;

			class activity
				: public object
				, public singleton<activity_t>
			{
			private:
				friend singleton<activity_t>;
				static void* activity_entry(void* param);

			private:
				objptr _user_data;
				ibuffer_t _saved_state;

				ANativeActivity* _activity;
				AConfiguration* _config;
				ALooper* _looper;
				AInputQueue* _input_queue;
				ANativeWindow* _window;
				ARect contentRect;
				int activityState;
				
				mutable ang::core::async::mutex_t _mutex;
				mutable ang::core::async::cond_t _cond;
				int msgread;
				int msgwrite;
				pthread_t thread;
				int running;
				int stateSaved;
				int destroyed;
				int redrawNeeded;
				AInputQueue* pendingInputQueue;
				ANativeWindow* pendingWindow;
				ARect pendingContentRect;

				ang::core::delegates::function<void(void)> on_command_event_handler;
				ang::core::delegates::function<void(void)> on_input_event_handler;

			protected:
				activity() { throw exception(except_code::invalid_param); }
				activity(ANativeActivity* na, ibuffer_t savedState);
				virtual~activity();

			public:
				int destroyRequested;

				ang::core::delegates::listener<dword(activity_t, uint)> command_event;
				ang::core::delegates::listener<dword(activity_t, AInputEvent*)> input_event;

				objptr user_data()const { return _user_data; }
				void user_data(objptr value) { _user_data = value; }

				ibuffer_t saved_state()const { return _saved_state; }
				void saved_state(ibuffer_t value) { _saved_state = value; }

				ANativeActivity* native_activity()const { return _activity; }
				AConfiguration* configuration()const { return _config; }
				ALooper* looper()const { return _looper; }
				AInputQueue* input_queue()const { return _input_queue; }
				ANativeWindow* window()const { return _window; }

				ang::core::async::mutex_t main_mutex()const { return _mutex; }
				ang::core::async::cond_t main_cond()const { return _cond; }

				cstr_t get_language()const {
					static char lang[3];
					AConfiguration_getLanguage(_config, lang); lang[2] = 0;
					return lang;
				}
				cstr_t get_country()const {
					static char country[3];
					AConfiguration_getLanguage(_config, country); country[2] = 0;
					return country;
				}
				int get_mcc()const { return AConfiguration_getMcc(_config); }
				int get_mnc()const { return AConfiguration_getMnc(_config); }
				int get_orientation()const { return AConfiguration_getOrientation(_config); }
				int get_touch_screen()const { return AConfiguration_getTouchscreen(_config); }
				int get_density()const { return AConfiguration_getDensity(_config); }
				int get_keyboard()const { return AConfiguration_getKeyboard(_config); }
				int get_navigation()const { return AConfiguration_getNavigation(_config); }
				int get_keys_hidden()const { return AConfiguration_getKeysHidden(_config); }
				int get_nav_hidden()const { return AConfiguration_getNavHidden(_config); }
				int get_sdk_version()const { return AConfiguration_getSdkVersion(_config); }
				int get_screen_size()const { return AConfiguration_getScreenSize(_config); }
				int get_screen_long()const { return AConfiguration_getScreenLong(_config); }
				int get_ui_mode_type()const { return AConfiguration_getUiModeType(_config); }
				int get_ui_mode_night()const { return AConfiguration_getUiModeNight(_config); }

			public:
				uint read_cmd(void);
				void write_cmd(uint);
				void set_input(AInputQueue*);
				void set_window(ANativeWindow*);
				void set_activity_state(uint);
				void process_command();
				void process_input();
				void close();

			protected:
				virtual void pre_execute_comand(uint cmd);
				virtual void post_execute_comand(uint cmd);

			private:

				static void onDestroy(ANativeActivity* activity);
				static void onStart(ANativeActivity* activity);
				static void onResume(ANativeActivity* activity);
				static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen);
				static void onPause(ANativeActivity* activity);
				static void onStop(ANativeActivity* activity);
				static void onConfigurationChanged(ANativeActivity* activity);
				static void onLowMemory(ANativeActivity* activity);
				static void onWindowFocusChanged(ANativeActivity* activity, int focused);
				static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window);
				static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window);
				static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue);
				static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue);

			};

			enum LOOPER_ID {
				LOOPER_ID_MAIN = 1,
				LOOPER_ID_INPUT = 2,
				LOOPER_ID_USER = 3,
			};

			enum APP_CMD : uint {
				APP_CMD_INPUT_CHANGED,
				APP_CMD_INIT_WINDOW,
				APP_CMD_TERM_WINDOW,
				APP_CMD_WINDOW_RESIZED,
				APP_CMD_WINDOW_REDRAW_NEEDED,
				APP_CMD_CONTENT_RECT_CHANGED,
				APP_CMD_GAINED_FOCUS,
				APP_CMD_LOST_FOCUS,
				APP_CMD_CONFIG_CHANGED,
				APP_CMD_LOW_MEMORY,
				APP_CMD_START,
				APP_CMD_RESUME,
				APP_CMD_SAVE_STATE,
				APP_CMD_PAUSE,
				APP_CMD_STOP,
				APP_CMD_DESTROY,
			};
		}
	}
}

ANG_REGISTER_RUNTIME_TYPENAME(AInputEvent);
ANG_REGISTER_RUNTIME_TYPENAME(AInputEvent*);

#endif /* _ANDROID_NATIVE_APP_GLUE_H */
