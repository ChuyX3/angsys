#pragma once
#include <angsys.h>
#include <ang/core/files.h>
#include <ang/graphics/graphics.h>
#include <ang/collections/hash_map.h>

namespace ang
{
	namespace graphics
	{
		namespace resources
		{
			ang_object(resource);
			ang_object(library);
			ang_object(resource_manager);
		}
		namespace buffers
		{
			ang_object(index_buffer);
			ang_object(vertex_buffer);
			ang_object(frame_buffer);
		}
		namespace textures
		{
			ang_object(texture);
			ang_object(texture_loader);
		}
		namespace effects
		{
			using itechnique = ishaders;
			using itechnique_t = ishaders_t;

			ang_object(technique);
			ang_object(pass);
			ang_object(effect);
			ang_object(effect_library);
		}
	}

	template<> struct is_inherited_from<graphics::resources::resource_manager, intf> : public true_type { };
	template<> struct is_inherited_from<graphics::resources::resource_manager, object> : public true_type { };

}

#include "resources/resource.h"
#include "resources/textures.h"
#include "resources/buffers.h"
#include "resources/effects.h"
#include "resources/models.h"
#include "resources/library.h"

