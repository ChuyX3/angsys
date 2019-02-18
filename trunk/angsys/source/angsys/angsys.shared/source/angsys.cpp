/*********************************************************************************************************************/
/*   File Name: angsys.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.h"
#include "ang_memory.h"
#include "encoder_interface.h"
#include "runtime_manager.h"
#include "thread_manager.h"
#include "file_system.h"

#include <ang/dom/xml.h>

#include <ang/platform/platform.h>
//#include "ang/core/async.h"

#include <ang/collections/hash_map.h>

using namespace ang;
using namespace ang::memory;

ANG_EXTERN ulong64 get_performance_time_us(void)
{
	{
#if defined ANDROID_PLATFORM
		struct timespec ts;
		ulong64 theTick = 0;
		clock_gettime(CLOCK_REALTIME, &ts);
		theTick = (ulong64)ts.tv_nsec / 1000.0;
		theTick += (ulong64)ts.tv_sec * 1000000.0;
		return theTick;
#else
		static struct PerformanceFrequency {
			ulong64 QuadPart;
			PerformanceFrequency() {
				LARGE_INTEGER _frec;
				QueryPerformanceFrequency(&_frec);
				QuadPart = (ulong64)_frec.QuadPart;
			}
		}frec;

		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		return  (1000000 * count.QuadPart) / frec.QuadPart; //uS
#endif
	}
}




namespace ang
{
	class ang_main_instance
	{
	public:
		static ang_main_instance* instance();

		ang_main_instance()
			: default_memory_allocator(ang_default_memory)
			, object_memory_allocator(ang_object_memory)
			, buffer_memory_allocator(ang_buffer_memory)
			, aligned_memory_allocator()
			, runtime_type_manager()
			, thread_manager(null)
		{
			objects = new(malloc(sizeof(object_manager))) object_manager();
			thread_manager = new(malloc(sizeof(core::async::thread_manager))) core::async::thread_manager();

			core::files::file_system::instance();
		}
		~ang_main_instance()
		{
			core::files::file_system::release_instance();

			thread_manager->~thread_manager();
			free(thread_manager);
			thread_manager = null;

			strings::string_pool::release_instance();

			ascii_encoder = null;
			unicode_encoder = null;
			utf8_encoder = null;
			utf16_encoder = null;
			utf16_se_encoder = null;
			utf16_le_encoder = null;
			utf16_be_encoder = null;
			utf32_encoder = null;
			utf32_se_encoder = null;
			utf32_le_encoder = null;
			utf32_be_encoder = null;
			
			objects->~object_manager();	
			free(objects);			
			objects = null;
		}

		allocator_internal default_memory_allocator;
		allocator_internal object_memory_allocator;
		allocator_internal buffer_memory_allocator;
		aligned_allocator_internal aligned_memory_allocator;
		runtime::runtime_type_manager runtime_type_manager;

#if defined _DEBUG || defined _DEVELOPPER
		object_manager* objects;
#endif //DEBUG

		core::async::thread_manager* thread_manager;

		text::iencoder_t ascii_encoder;
		text::iencoder_t unicode_encoder;
		text::iencoder_t utf8_encoder;
		text::iencoder_t utf16_encoder;
		text::iencoder_t utf16_se_encoder;
		text::iencoder_t utf16_le_encoder;
		text::iencoder_t utf16_be_encoder;
		text::iencoder_t utf32_encoder;
		text::iencoder_t utf32_se_encoder;
		text::iencoder_t utf32_le_encoder;
		text::iencoder_t utf32_be_encoder;
	};

	static struct ang_main_instance_constructor
	{
		ang_main_instance* instance = null;
		ang_main_instance_constructor()
		{
			instance = (ang_main_instance*)malloc(sizeof(ang_main_instance));
			new(instance)ang_main_instance();
		}
		~ang_main_instance_constructor()
		{
			instance->~ang_main_instance();
			free(instance);
		}
	}instanceConstructor;

	ang_main_instance* ang_main_instance::instance()
	{
		return instanceConstructor.instance;
	}

}




aligned_allocator_internal* memory::get_aligned_allocator() {
	return &ang_main_instance::instance()->aligned_memory_allocator;
}

iraw_allocator* memory::get_raw_allocator(ang_memory_hint_t hint)
{
	switch (hint)
	{
	case ang_object_memory:
		return &ang_main_instance::instance()->object_memory_allocator;
	case ang_buffer_memory:
		return &ang_main_instance::instance()->buffer_memory_allocator;
	case ang_aligned_memory:
		return &ang_main_instance::instance()->aligned_memory_allocator;
	default:
		return &ang_main_instance::instance()->default_memory_allocator;
	}
}

runtime::runtime_type_manager* runtime::runtime_type_manager::instance()
{
	return &ang_main_instance::instance()->runtime_type_manager;
}

#if defined _DEBUG || defined _DEVELOPPER
object_manager* object_manager::instance()
{
	return ang_main_instance::instance()->objects;
}
#endif

text::iencoder_t text::iencoder::get_encoder(text::encoding_t e)
{
	static struct _initializer
	{
		_initializer() {
			ang_main_instance::instance()->ascii_encoder = new text::encoder_interface<text::encoding::ascii>();
			ang_main_instance::instance()->unicode_encoder = new text::encoder_interface<text::encoding::unicode>();
			ang_main_instance::instance()->utf8_encoder = new text::encoder_interface<text::encoding::utf8>();
			ang_main_instance::instance()->utf16_encoder = new text::encoder_interface<text::encoding::utf16>();
			ang_main_instance::instance()->utf16_se_encoder = new text::encoder_interface<text::encoding::utf16_se>();
			ang_main_instance::instance()->utf16_le_encoder = new text::encoder_interface<text::encoding::utf16_le>();
			ang_main_instance::instance()->utf16_be_encoder = new text::encoder_interface<text::encoding::utf16_be>();
			ang_main_instance::instance()->utf32_encoder = new text::encoder_interface<text::encoding::utf32>();
			ang_main_instance::instance()->utf32_se_encoder = new text::encoder_interface<text::encoding::utf32_se>();
			ang_main_instance::instance()->utf32_le_encoder = new text::encoder_interface<text::encoding::utf32_le>();
			ang_main_instance::instance()->utf32_be_encoder = new text::encoder_interface<text::encoding::utf32_be>();
		}
	}s_initializer;

	switch (e.get())
	{
	case text::encoding::ascii: return ang_main_instance::instance()->ascii_encoder;
	case text::encoding::unicode: return ang_main_instance::instance()->unicode_encoder;
	case text::encoding::utf8: return ang_main_instance::instance()->utf8_encoder;
	case text::encoding::utf16: return ang_main_instance::instance()->utf16_encoder;
	case text::encoding::utf16_se: return ang_main_instance::instance()->utf16_se_encoder;
	case text::encoding::utf16_le: return ang_main_instance::instance()->utf16_le_encoder;
	case text::encoding::utf16_be: return ang_main_instance::instance()->utf32_be_encoder;
	case text::encoding::utf32: return ang_main_instance::instance()->utf32_encoder;
	case text::encoding::utf32_se: return ang_main_instance::instance()->utf32_se_encoder;
	case text::encoding::utf32_le: return ang_main_instance::instance()->utf32_le_encoder;
	case text::encoding::utf32_be: return ang_main_instance::instance()->utf32_be_encoder;
	default: return null;
	}
}



static const ang_uint32_t list[] = {
	7u,
	13u,
	31u,
	61u,
	127u,
	251u,
	509u,
	1021u,
	2039u,
	4093u,
	8191u,
	16381u,
};

wsize algorithms::hash_table_get_next_size(wsize size) {
	wsize out = 0;
	if (size >= 16381) {
		out = 16381;
		while (size >= out)
			out *= 2;
		return out;
	}
	else {
		for (auto i = 0U; i < algorithms::array_size(list); ++i)
			if (size < list[i])
				return list[i];
		return -1;
	}
}


long64 algorithms::hash_index_maker<raw_str_t>::make(raw_str_t& key_, ulong64 TS)
{
	ulong64 h = 75025;
	
	text::iencoder_t encoder = text::iencoder::get_encoder(key_.encoding());
	windex i = 0, c = key_.count();

	for (char32_t n = encoder->to_char32(key_.ptr(), i); 
		n != 0 && i < c;
		n = encoder->to_char32(key_.ptr(), i))
	{
		h = (h << 5) + h + dword(n) + 1;
	}
	return ang_uint64_t(h % TS);
}

long64 algorithms::hash_index_maker<raw_cstr_t>::make(raw_cstr_t& key_, ulong64 TS)
{
	ulong64 h = 75025;

	text::iencoder_t encoder = text::iencoder::get_encoder(key_.encoding());
	windex i = 0, c = key_.count();

	for (char32_t n = encoder->to_char32(key_.ptr(), i);
		n != 0 && i < c;
		n = encoder->to_char32(key_.ptr(), i))
	{
		h = (h << 5) + h + dword(n) + 1;
	}
	return ang_uint64_t(h % TS);
}




