#ifndef __ANG_FRAMEWORK_H__
#define __ANG_FRAMEWORK_H__

#include <angsys.h>
#include <ang/xml.h>
#include <ang/core/time.h>
#include <ang/core/async.h>
#include <ang/collections/map.h>
#include <ang/collections/vector.h>
#include <ang/graphics/angraph.h>


namespace ang
{
	namespace graphics
	{
		class framework;
		typedef object_wrapper<framework> framework_t;

		namespace ui
		{
			//class ui_element;
			//typedef object_wrapper<ui_element> ui_element;
		}
	}
}

#include "scenes.h"

namespace ang
{
	namespace graphics
	{
		class framework
			: public object
		{
		protected:
			platform::icore_view_t _core_view;
			idriver_t _driver;
			isurface_t _surface;
			iframe_buffer_t _root_frame_buffer;
			core::async::iasync_t<void> async_render_operation;
			
		public:
			framework(platform::icore_view_t);

			ANG_DECLARE_INTERFACE();

		public:
			core::delegates::listener<void(framework_t, idriver_t) > begin_rendering_event;
			core::delegates::listener<void(framework_t, idriver_t) > end_rendering_event;
			core::delegates::listener<void(framework_t, idriver_t)> draw_event;
			core::delegates::listener<void(framework_t, core::time::timer const&) > update_event;
	

		protected:
			virtual bool initialize(platform::icore_view_t view);
			virtual void on_start_render_operation();
			virtual bool terminate();

			virtual void on_begin_rendering_event();
			virtual void on_end_rendering_event();
			virtual void on_draw_event();
			virtual void on_update_event(core::time::timer const&);

		private:		
			void on_destroyed(objptr, platform::events::imsg_event_args_t);
			void on_size_changed(objptr, platform::events::idisplay_info_event_args_t);
			void on_pointer_moved(objptr, platform::events::ipointer_event_args_t);
			void on_pointer_pressed(objptr, platform::events::ipointer_event_args_t);
			void on_pointer_released(objptr, platform::events::ipointer_event_args_t);

		protected:
			virtual~framework();
		};
		
	}
}


#endif//__ANG_FRAMEWORK_H__
