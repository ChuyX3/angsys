#ifndef __ANGUI_H__
#define __ANGUI_H__

#include <angsys.h>
#include <ang/xml.h>
#include <ang/core/time.h>
#include <ang/graphics/angraph.h>
#include <ang/platform/platform.h>
#include <ang/collections/vector.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGUI_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGUI_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif

namespace ang
{
	namespace ui
	{
		class panel;
		class ui_element;
		typedef object_wrapper<panel> panel_t;
		typedef object_wrapper<ui_element> ui_element_t;

		namespace events
		{
			typedef var_args event_args;
			typedef var_args_t event_args_t;
			typedef core::delegates::function_data<void(objptr, event_args_t)> event;
			typedef core::delegates::function<void(objptr, event_args_t)> event_t;
		}
	}
}


namespace ang
{
	namespace ui
	{
	
		class LINK ui_element
			: public object
		{
		protected:
			ui_element();
			virtual~ui_element();

			ANG_DECLARE_INTERFACE();

		public:
			virtual foundation::point<float> location()const = 0;
			virtual void location(foundation::point<float>) = 0;
			virtual  foundation::size<float> dimention()const = 0;
			virtual  void dimention(foundation::size<float>) = 0;
		};


		class LINK panel
			: public ui_element
		{
		protected:
			foundation::point<float> _location;
			foundation::size<float> _dimention;
			graphics::color_t _background_color;
			graphics::iframe_buffer_t _background_buffer;
			collections::vector<ui_element_t> _elements;

		public:
			panel();
		

			ANG_DECLARE_INTERFACE();

		protected:
			void update(core::time::timer const&);
			void draw();

		protected:
			virtual~panel();
		};
	}
}

#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANGUI_H__