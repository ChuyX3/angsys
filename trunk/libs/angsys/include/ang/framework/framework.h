/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_FRAMEWORK_H__
#define __ANG_FRAMEWORK_H__

#include <angsys.h>
#include <ang/core/files.h>
#include <ang/core/async.h>
#include <ang/graphics/foundations.h>
#include <ang/dom/xml.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANG_DYNAMIC_LIBRARY
#ifdef ANG_FRAMEWORK_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANG_FRAMEWORK_EXPORTS
#else//#elif defined ANG_STATIC_LIBRARY
#define LINK
#endif//ANG_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

namespace ang
{
	namespace framework
	{
		namespace ui
		{
			ang_declare_interface(imenu);
			ang_declare_interface(icontrol);
		}

		ang_declare_object(game);
		ang_declare_interface(igame);
		ang_declare_interface(istate);
		ang_declare_interface(istate_manager);
		ang_declare_object(state_manager);
	}

	namespace graphics
	{
		namespace resources
		{
			ang_declare_object(library);
			ang_declare_object(resource_manager);
		}

		namespace textures
		{
			ang_declare_object(texture_loader);
		}

		namespace effects
		{
			ang_declare_object(pass);
			ang_declare_object(effect);
			ang_declare_object(effect_library);
		}

		namespace meshes
		{
			ang_declare_object(mesh);
			ang_declare_object(mesh_loader);
			ang_declare_object(geometry);
			ang_declare_object(mesh_data);
			ang_declare_object(mesh_stream);
		}
	}

	namespace framework
	{
		declare_enum(LINK, game_state, uint)
		{
			none,
			loading,
			main_menu,
			ingame
		};

		struct igame 
			: intf<igame
			, iid("ang::framework::igame")>
		{
			virtual graphics::idriver_t driver()const = 0;
			virtual graphics::isurface_t surface()const = 0;
			virtual graphics::ifactory_t factory()const = 0;
			virtual const core::time::step_timer& timer()const = 0;
			virtual core::async::idispatcher_t dispatcher()const = 0;
			virtual graphics::resources::ilibrary_t library()const = 0;
			virtual istate_manager_t state_manager()const = 0;

			virtual error run(graphics::graph_driver_type_t, string app_name, string app_title) = 0;
			virtual void update() = 0;
			virtual void draw(graphics::scenes::itransform_t cam, graphics::iframe_buffer_t fbo) = 0;

		};

		struct istate
			: intf<istate
			, iid("ang::framework::istate")>
		{
			virtual game_state_t state_id()const = 0;
			virtual graphics::scenes::iscene_t scene()const = 0;
			virtual core::async::iasync<bool> load_async(igame_t, dom::xml::ixml_node_t) = 0;
			virtual istate_t next_state()const = 0;
			virtual bool update(igame_t, core::time::step_timer const&) = 0;
			virtual void draw(igame_t, graphics::iframe_buffer_t, graphics::scenes::icamera_t) = 0;
			virtual bool unload(igame_t) = 0;
		};

		struct istate_manager
			: intf<istate
			, iid("ang::framework::istate_manager")>
		{
			virtual istate_t current()const = 0;
			virtual bool load(igame_t, dom::xml::ixml_node_t) = 0;
			virtual istate_t create_state(dom::xml::ixml_node_t)const = 0;
			virtual void update(igame_t, core::time::step_timer const&) = 0;
			virtual void draw(igame_t) = 0;
			virtual bool unload() = 0;
		};

	}
}

#include <ang/framework/textures.h>
#include <ang/framework/meshes.h>
#include <ang/framework/effects.h>
#include <ang/framework/library.h>
#include <ang/framework/scenes.h>
#include <ang/framework/state_machine.h>

namespace ang
{
	namespace framework
	{
		class LINK game 
			: public implement<game
			, iid("ang::framework::game")
			, igame>
		{
		protected:
			graphics::graph_driver_type_t m_driver_type;
			graphics::idriver_t m_driver;
			graphics::isurface_t m_surface;
			core::time::step_timer m_timer;
			graphics::resources::resource_manager_t m_res_manager;
			state_manager_t m_state_manager;

		public: /*constructor*/
			game();

		public: /**/
			error run(graphics::graph_driver_type_t, string app_name = "game", string app_title = "game") override;
			void update() override;
			void draw(graphics::scenes::itransform_t cam, graphics::iframe_buffer_t fbo) override;

		public: /*properties*/
			graphics::idriver_t driver()const override;
			graphics::isurface_t surface()const override;
			graphics::ifactory_t factory()const override;
			const core::time::step_timer& timer()const override;
			core::async::idispatcher_t dispatcher()const override;
			graphics::resources::ilibrary_t library()const override;
			istate_manager_t state_manager()const override;

		public: /*event handlers*/
			virtual void on_initialize(objptr sender, platform::events::icreated_event_args_t args);
			virtual void on_update(objptr sender, platform::events::imsg_event_args_t args);
			virtual void on_finalize(objptr sender, platform::events::imsg_event_args_t args);

			virtual void on_pointer_pressed(objptr sender, platform::events::ipointer_event_args_t args);
			virtual void on_pointer_moved(objptr sender, platform::events::ipointer_event_args_t args);
			virtual void on_pointer_released(objptr sender, platform::events::ipointer_event_args_t args);
			virtual void on_pointer_entered(objptr sender, platform::events::ipointer_event_args_t args);
			virtual void on_pointer_leaved(objptr sender, platform::events::ipointer_event_args_t args);

			virtual void on_key_pressed(objptr sender, platform::events::ikey_event_args_t args);
			virtual void on_key_released(objptr sender, platform::events::ikey_event_args_t args);
			virtual void on_text_changed(objptr sender, platform::events::itext_change_event_args_t args);

			virtual void on_size_changed(objptr sender, platform::events::idisplay_info_event_args_t args);
			virtual void on_display_changed(objptr sender, platform::events::idisplay_info_event_args_t args);

		protected:
			virtual~game();
		};
	}
}



#endif//__ANG_FRAMEWORK_H__