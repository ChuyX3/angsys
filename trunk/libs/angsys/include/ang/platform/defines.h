/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/
#ifndef __ANG_PLATFORM_H__
#error ...
#elif !defined __ANG_PLATFORM_DEFINES_H__
#define __ANG_PLATFORM_DEFINES_H__

#define ang_intf_event(_TYPE, _NAME) \
protected: \
	virtual ang::platform::events::event_token_t add_##_NAME(ang::platform::events::event_t) = 0; \
	virtual bool remove_##_NAME(ang::platform::events::event_token_t) = 0; \
private: \
	static ang::platform::events::event_token_t add_##_NAME##_handler(ang::platform::events::base_event*, ang::platform::events::event_t); \
	static bool remove_##_NAME##_handler(ang::platform::events::base_event*, ang::platform::events::event_token_t); \
public: \
	ang::platform::events::event<_TYPE, add_##_NAME##_handler, remove_##_NAME##_handler> _NAME;

#define ang_intf_implement_event_handler(_CLASS, _NAME) \
ang::platform::events::event_token_t _CLASS::add_##_NAME##_handler(ang::platform::events::base_event* prop, ang::platform::events::event_t e) { \
	return field_to_parent(&_CLASS::_NAME, prop)->add_##_NAME(e);	\
} \
bool _CLASS::remove_##_NAME##_handler(ang::platform::events::base_event* prop, ang::platform::events::event_token_t token) { \
	return field_to_parent(&_CLASS::_NAME, prop)->remove_##_NAME(token);	\
}

#define ang_platform_event(_TYPE, _NAME) \
private: \
	ang::platform::events::event_listener m_##_NAME; \
	static ang::platform::events::event_token_t add_##_NAME##_handler(ang::platform::events::base_event*, ang::platform::events::event_t); \
	static bool remove_##_NAME##_handler(ang::platform::events::base_event*, ang::platform::events::event_token_t); \
public: \
	ang::platform::events::event<_TYPE, add_##_NAME##_handler, remove_##_NAME##_handler> _NAME;

#define ang_platform_implement_event_handler(_CLASS, _NAME) \
ang::platform::events::event_token_t _CLASS::add_##_NAME##_handler(ang::platform::events::base_event* prop, ang::platform::events::event_t e) { \
	return field_to_parent(&_CLASS::_NAME, prop)->m_##_NAME += e;	\
} \
bool _CLASS::remove_##_NAME##_handler(ang::platform::events::base_event* prop, ang::platform::events::event_token_t token) { \
	return field_to_parent(&_CLASS::_NAME, prop)->m_##_NAME -= token;	\
}

#endif /*__ANG_PLATFORM_DEFINES_H__*/

