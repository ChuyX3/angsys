/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_PLATFORM_H__
#error ...
#elif !defined __ANG_PLATFORM_INPUT_H__
#define __ANG_PLATFORM_INPUT_H__

namespace ang
{
	namespace platform
	{
		namespace input
		{
			struct ikeyboard;
			typedef intf_wrapper<ikeyboard> ikeyboard_t;

			safe_flags(LINK, key_modifiers, uint)
			{
				none = 0,
				control = 1,
				shift = 2,
				alt = 4,
				home_key = 8,
				window_key = 8,
				caps_lock = 16,
				num_lock = 32,
			};

			safe_enum(LINK, pointer_hardware_type, uint)
			{
				touch,
				pen,
				mouse,
				touchpad
			};

			safe_enum(LINK, keyboard_type, uint)
			{
				def = 0,
				url,
				numerical,
			};

			safe_enum(LINK, key_state, uint)
			{
				pressed = 0,
				released = 1,
				toggled = 2,
			};

			safe_enum(LINK, virtual_key, uint)
			{
				no_key = 0,

				back = 0X08,
				tab = 0X09,
				cear = 0X0C,
				ret = 0X0D,
				shift = 0X10,
				control = 0X11,
				alt = 0X12,
				pause = 0X13,
				caps_lock = 0X14,
				num_lock = 0X90,

				escape = 0X1B,
				space = 0X20,
				page_up = 0X21,
				page_down = 0X22,
				end = 0X23,
				home = 0X24,
				left = 0X25,
				up = 0X26,
				right = 0X27,
				down = 0X28,
				snap_shot = 0X2C,
				insert = 0X2D,
				del = 0X2E,

				key0 = 0X30, key1, key2, key3, key4,
				key5, key6, key7, key8, key9,

				keyA = 0X41, keyB, keyC, keyD, keyE,
				keyF, keyG, keyH, keyI, keyJ, keyK, keyL,
				keyM, keyN, keyO, keyP, keyQ, keyR, keyS,
				keyT, keyU, keyV, keyW, keyX, keyY, keyZ,

				num_pad0 = 0X60, num_pad1, num_pad2,
				num_pad3, num_pad4, num_pad5, num_pad6,
				num_pad7, num_pad8, num_pad9,
				multiply = 0X6A,
				add = 0X6B,
				subtract = 0X6D,
				decimal = 0X6E,
				divide = 0X6F,

				F1 = 0X70, F2, F3, F4, F5, F6, F7, F8,
				F9, F10, F11, F12, F13, F14, F15, F16,
				F17, F18, F19, F20, F21, F22, F23, F24,
			};

			typedef struct poiner_info
			{
				graphics::point<float> point;
				short id;
				bool is_primary_action;
				bool is_secondary_action;
				pointer_hardware_type_t type;
				key_modifiers_t modifiers;
			}poiner_info_t;

			typedef struct key_info
			{
				virtual_key key_code;
				char32_t char_code;
				word flags;
				key_state_t state;
				key_modifiers_t modifiers;
			}key_info_t;

			typedef struct text_selection
			{
				windex first;
				windex last;
				wstring text;
			}text_selection_t;

			ang_begin_interface(LINK ikeyboard)
				visible vcall bool show_touch_keyboard()pure;
				visible vcall bool show_touch_keyboard(const text_selection_t& text_and_cursor)pure;
				visible vcall bool hide_touch_keyboard()pure;
				visible vcall bool is_visible()const pure;
				visible vcall wstring text()const pure;
				visible vcall void text(wstring)pure;
				visible vcall wsize text_lenght()const pure;
				visible vcall bool select(const text_selection_t&)pure;
				visible vcall text_selection_t selection()const pure;
				visible vcall bool is_multiline()const pure;
				visible vcall void is_multiline(bool)pure;
				visible vcall void type(keyboard_type)pure;
				visible vcall keyboard_type type()const pure;
				visible vcall wstring restriction()const pure;
				visible vcall void restriction(wstring)pure;
			ang_end_interface();
	
			////////////////////////////////////////////////////
			
		}//input
	}
}

#endif//__ANG_PLATFORM_INPUT_H__