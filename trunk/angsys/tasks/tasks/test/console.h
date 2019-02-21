#pragma once

namespace ang
{
	namespace platform
	{
		namespace console
		{
			
			ang_object(app);
			ang_object(window);
			ang_object(input);
			ang_object(output);

			namespace ui
			{
				ang_object(element);

				ang_object(menu);
				ang_object(dialog);
				ang_object(label);
				ang_object(button);
				ang_object(textbox);

			}
		}

		namespace events
		{
			ang_interface(iconsole_key_event_args);
			ang_interface(iconsole_mouse_event_args);

			ang_begin_interface(iconsole_key_event_args, imsg_event_args)
				visible vcall input::virtual_key_t key()const pure;
				visible vcall input::key_modifiers_t modifiers()const pure;
				visible vcall bool is_primary_action()const pure;
			ang_end_interface();
			
			ang_begin_interface(iconsole_mouse_event_args, imsg_event_args)
				visible vcall graphics::point<short> point()const pure;
			visible vcall input::key_modifiers_t modifiers()const pure;
			visible vcall bool is_primary_action()const pure;
			ang_end_interface();
		}

		namespace console
		{
		
			typedef graphics::point<short> coord, coord_t, point;
			typedef graphics::size<short> bound, bound_t, size;
			typedef graphics::rect<short> rect, rect_t;

			safe_enum(, color, byte)
			{
				black = 0,
				navy,
				green,
				turquoise,
				scarlet,
				magenta,
				olive,
				silver,
				gray,
				blue,
				lime,
				cyan,
				red,
				pink,
				yellow,
				white
			};

			typedef class text_property
			{
			private:
				union
				{
					word _value;
					struct
					{
						color _foreground : 4;
						color _background : 4;
					};
				};
			public:
				text_property(color_t fore = color::silver, color_t back = color::black) {
					_value = 0;
					_foreground = fore.get();
					_background = back.get();
				}

				color_t foreground()const { return _foreground; }
				void foreground(color_t value) { _foreground = value.get(); }
				color_t background()const { return _background; }
				void background(color_t value) { _background = value.get(); }
				word code()const { return _value; }
				void code(word c) { _value = 0XFF & c; }
			}text_property_t;

			

			class app
				: public object
			{
			private:
				typedef struct mouse_event_record {
					coord_t mouse_position;
					dword button_state;
					dword control_key_state;
					dword event_flags;
				} mouse_event_record_t, *mouse_event_record_ptr_t;
				mouse_event_record_t mouse_state;

			public:
				app();
				ANG_DECLARE_INTERFACE();

				virtual int run(array<string> args);

				core::delegates::listener<void(objptr, objptr)> create_event;
				core::delegates::listener<void(objptr, objptr)> destroy_event;

				core::delegates::listener<void(objptr, events::iconsole_key_event_args_t)> key_presed_event;
				core::delegates::listener<void(objptr, events::iconsole_key_event_args_t)> key_released_event;

				core::delegates::listener<void(objptr, events::iconsole_mouse_event_args_t)> pointer_presed_event;
				core::delegates::listener<void(objptr, events::iconsole_mouse_event_args_t)> pointer_move_event;
				core::delegates::listener<void(objptr, events::iconsole_mouse_event_args_t)> pointer_released_event;		

				property<const pointer, app> input_handle;
				property<const pointer, app> output_handle;

				property<const output_t, app> output;
			protected:
				virtual int init();
				virtual bool loop();
				virtual int exit();

				virtual void on_mouse_event(pointer event);
				virtual void on_key_event(pointer event);
			protected:
				virtual~app();
			};

			class window
				: public object
			{

			};

	

			class output
				: public object
			{
			private:
				pointer handle;

			public:
				output();

				ANG_DECLARE_INTERFACE();

				bound_t size()const;
				void size(bound_t)const;

				uint cursor_size()const;
				bool cursor_visible()const;
				void cursor_info(uint size, bool vis = true);

				coord_t cursor()const;
				void cursor(coord_t);

				text_property_t text_property()const;
				void text_property(text_property_t);

				void clrscr();
				void draw_solid_rect(rect_t, text_property_t, char fill = 0);
				void draw_frame_rect(rect_t, text_property_t, bool thick = false, bool fill = false);

				void print(cstr_t text);
				void print(cstr_t format, collections::vector<variant> vars);
				template<typename...Ts> void print(cstr_t format, Ts... args) {
					print(format, collections::vector<variant>{ ((variant)args)... });
				}

				void write(coord_t xy, cstr_t text);
				void write(coord_t xy, cstr_t format, collections::vector<variant> vars);
				template<typename...Ts> void write(coord_t xy, cstr_t format, Ts... args) {
					write(xy, format, collections::vector<variant>{ ((variant)args)... });
				}



			private:
				virtual~output();
			};

			ang_object(menu);

		
			class menu 
				: public object
			{
			private:
				coord_t _coord;
				text_property_t _normal_text_property;
				text_property_t _highlight_text_property;

				vector<string> _items;

			public:
				menu();

				ANG_DECLARE_INTERFACE();

				virtual int show(output_t output)const;

				coord_t coord()const { return _coord; }
				void coord(coord_t value) { _coord = value; }

				text_property_t normal_text_property()const { return _normal_text_property; }
				void normal_text_property(text_property_t value) { _normal_text_property = value; }

				text_property_t highlight_text_property()const { return _highlight_text_property; }
				void highlight_text_property(text_property_t value) { _highlight_text_property = value; }

				array_view<string> items()const { return _items; }
				void items(array_view<string> value) { _items = value; }
				void items(initializer_list<string> value) { _items = value; }
				void add_item(string value) { _items += value; }
				void add_items(array_view<string> value) { _items += value; }

			protected:
				virtual~menu();

			};
			
		}
	}
}