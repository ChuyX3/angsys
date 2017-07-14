#pragma once

#include "d3d11/driver.h"
#include <ang/graphics/drawing.h>

#if DIRECTX_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace d3d11
		{
			class d3d11_draw_context;
			typedef object_wrapper<d3d11_draw_context> d3d11_draw_context_t;

			class d3d11_solid_brush;
			class d3d11_texturing_brush;

		//	typedef object_wrapper<d3d11_solid_brush> d3d11_solid_brush_t;
		//	typedef object_wrapper<d3d11_texturing_brush> d3d11_texturing_brush_t;


			class d3d11_draw_context
				: public object
				, public drawing::idraw_context
			{
			public:
				d3d11_draw_context(d3d11_driver_t, core::files::ifile_system_t = null);

			public: /*overrides*/
				ANG_DECLARE_INTERFACE();

				drawing::ibrush_t create_solid_brush(color_t) override;
				drawing::ibrush_t create_texturing_brush(textures::tex_wrap_mode_t, textures::itexture_t) override;
				void begin_draw(iframe_buffer_t) override;
				void end_draw() override;
				void clear(color_t) override;

			protected:
				bool create(d3d11_driver_t driver, core::files::ifile_system_t fs);
				bool close();

			public: //properties
				property<const bool, d3d11_draw_context> is_drawing;
				property<const d3d11_driver_t, d3d11_draw_context> driver;
				property<const effects::ieffect_library_t, d3d11_draw_context> effect_library;
				//property<const textures::itexture_loader_t, d3d11_draw_context> texture_loader;

			private:
				virtual~d3d11_draw_context();
			};

		}
	}
}

#endif
