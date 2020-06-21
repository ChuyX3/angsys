#ifndef __ANG_FRAMEWORK_H__
#elif !defined __ANG_FRAMEWORK_STATE_MACHINE_H__
#define __ANG_FRAMEWORK_STATE_MACHINE_H__

namespace ang
{
	namespace framework
	{

		class loading_state
			: public implement<loading_state
			, iid("ang::framework::loading_state")
			, istate>
		{
		private:
			istate_t m_state;
			dom::xml::ixml_node_t m_args;
			core::async::iasync<bool> m_task;

		public:
			loading_state();

		public:
			virtual game_state_t state_id()const override;
			virtual graphics::scenes::iscene_t scene()const override;
			virtual core::async::iasync<bool> load_async(igame_t, dom::xml::ixml_node_t) override;
			virtual bool update(igame_t, core::time::step_timer const&) override;
			virtual void draw(igame_t, graphics::iframe_buffer_t, graphics::scenes::icamera_t) override;
			virtual bool unload(igame_t) override;

		private:
			virtual~loading_state();
		};

		class ingame_state
			: public implement<ingame_state
			, iid("ang::framework::loading_state")
			, istate>
		{
		private:
			graphics::scenes::iscene_t m_scene;

		public:
			ingame_state();

		public:
			virtual game_state_t state_id()const override;
			virtual graphics::scenes::iscene_t scene()const override;
			virtual core::async::iasync<bool> load_async(igame_t, dom::xml::ixml_node_t) override;
			virtual bool update(igame_t, core::time::step_timer const&) override;
			virtual void draw(igame_t, graphics::iframe_buffer_t, graphics::scenes::icamera_t) override;
			virtual bool unload(igame_t) override;

		private:
			virtual~ingame_state();
		};

		class main_menu_state
			: public implement<main_menu_state
			, iid("ang::framework::loading_state")
			, istate>
		{
		public:
			virtual game_state_t state_id()const override;
			virtual graphics::scenes::iscene_t scene()const override;
			virtual core::async::iasync<bool> load_async(igame_t, dom::xml::ixml_node_t) override;
			virtual bool update(igame_t, core::time::step_timer const&) override;
			virtual void draw(igame_t, graphics::iframe_buffer_t, graphics::scenes::icamera_t) override;
			virtual bool unload(igame_t) override;
		};


		class state_manager
			: public implement<state_manager
			, iid("ang::framework::state_manager")
			, istate_manager>
		{
		private:
			collections::queue<istate_t> m_states;

		public:
			state_manager();

		public:
			virtual istate_t current()const override;
			virtual istate_t create_state(dom::xml::ixml_node_t)const override;
			virtual bool load(igame_t, dom::xml::ixml_node_t)override;
			virtual void update(igame_t, core::time::step_timer const&) override;
			virtual void draw(igame_t) override;
			virtual bool unload() override;

		private:
			virtual~state_manager();
		};

	}
}

#endif/*__ANG_FRAMEWORK_STATE_MACHINE_H__*/