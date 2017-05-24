
#ifndef __ANGRAPH_MEDIA_H__
#define __ANGRAPH_MEDIA_H__

#include <angsys.h>
#include <ang/core/files.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGRAPH_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
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
	namespace media
	{
			ANG_BEGIN_ENUM(LINK, sound_element_type, uint)
				none = 0,
				background_music,
				continuous_sound,
				transition_effect,
				uielement_effect,
			ANG_END_ENUM(sound_element_type);

			struct isound_engine;
			struct isound_element;

			typedef intf_wrapper<isound_engine> isound_engine_t;
			typedef intf_wrapper<isound_element> isound_element_t;

			ANG_BEGIN_INTERFACE(LINK, isound_element)
				visible vcall sound_element_type_t type()const pure;
				visible vcall bool is_playing()const pure;
				visible vcall wstring source_file()const pure;
				visible vcall float volume()const pure;
				visible vcall void volume(float) pure;
				visible vcall float pitch()const pure;
				visible vcall void pitch(float) pure;
				visible vcall bool play() pure;
				visible vcall bool pause() pure;
				visible vcall bool stop() pure;
			ANG_END_INTERFACE();

			ANG_BEGIN_INTERFACE(LINK, isound_engine)
				visible static isound_engine_t create_sound_engine();
				visible vcall bool start() pure;
				visible vcall bool recover() pure;
				visible vcall bool stop() pure;
				visible vcall bool is_running()const pure;
				visible vcall isound_element_t play_background_music(cwstr_t path, float sec) pure;
				visible vcall isound_element_t create_sound_element(sound_element_type_t, cwstr_t path) pure;
			ANG_END_INTERFACE();
	}
}

#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANGRAPH_MEDIA_H__