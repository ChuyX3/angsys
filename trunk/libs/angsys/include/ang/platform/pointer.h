/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_PLATFORM_INPUT_H__
#error ...
#elif !defined __ANG_PLATFORM_INPUT_POINTER_H__
#define __ANG_PLATFORM_INPUT_POINTER_H__

namespace ang
{
	namespace platform
	{
		namespace input
		{
			declare_enum(LINK, pointer_hardware_type, uint)
			{
				touch,
				pen,
				mouse,
				touchpad
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

			//struct nvt LINK ipointer
			//	: intf<ipointer
			//	, iid("ang::platform::input::ipointer")>
			//{
			//	//static ipointer_t instance();
			//	virtual icontroller_t controller(uint)const = 0;
			//	virtual events::event_token_t add_controller_connected_event(events::event_t) = 0;
			//	virtual bool remove_controller_connected_event(events::event_token_t) = 0;
			//	virtual events::event_token_t add_controller_disconnected_event(events::event_t) = 0;
			//	virtual bool remove_controller_disconnected_event(events::event_token_t) = 0;
			//};
			
		}//input
	}
}

#endif//__ANG_PLATFORM_INPUT_POINTER_H__
