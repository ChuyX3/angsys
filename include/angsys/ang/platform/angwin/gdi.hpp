#ifndef __ANGWIN_H__
#error ...
#elif !defined __ANGDI_H__
#define __ANGDI_H__

namespace ang
{
	namespace graphics
	{
		class device_context;
		class paint_dc;

		typedef object_wrapper<device_context> device_context_t;
		typedef object_wrapper<paint_dc> paint_dc_t;


		class LINK device_context
			: public object
			, public platform::icore_context
		{
		protected:
			pointer handle;

		public:
			device_context(platform::windows::window_t wnd);			

			ANG_DECLARE_INTERFACE();
	
			bool is_valid()const;
			virtual pointer get_core_context_handle()const override;
			virtual bool bind_graphic_native_surface(pointer) override;

		protected:
			virtual bool create(pointer);
			virtual bool close();

		public: //properties
			virtual pointer get_HDC()const;

			graphics::point<float> move_to(graphics::point<float>);
			graphics::point<float> current_position()const;
			graphics::point<float> draw_line_to(graphics::point<float>);
			void draw_primitive(primitive_t, const collections::ienum<graphics::point<float>>* ptns, bool solid = true);

		protected:
			virtual~device_context();
		};

		class LINK paint_dc
			: public device_context
		{
		public:
			paint_dc(platform::windows::window_t);

			ANG_DECLARE_INTERFACE();
			
		protected:
			virtual bool create(pointer);
			virtual bool close();

		public: //Properties
			graphics::rect<float> clip_area()const;

		protected:
			virtual~paint_dc();
		};
	}
}

#endif//__ANGWIN_H__