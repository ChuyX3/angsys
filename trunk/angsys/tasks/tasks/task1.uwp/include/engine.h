#pragma once

namespace ang
{
	namespace graphics
	{
		ang_object(engine);
		ang_object(holographic_space);
		ang_object(holographic_camera);

		namespace d3d11
		{
			ang_object(driver);

		}


		class holographic_space
			: public object
		{
		private:
			Windows::Graphics::Holographic::HolographicSpace^ m_space;
			Windows::Perception::Spatial::SpatialLocator^ m_locator;
			Windows::Perception::Spatial::SpatialStationaryFrameOfReference^ m_referenceFrame;

			Windows::Foundation::EventRegistrationToken m_cameraAddedToken;
			Windows::Foundation::EventRegistrationToken m_cameraRemovedToken;
			Windows::Foundation::EventRegistrationToken m_locatabilityChangedToken;

		public:
			holographic_space(Windows::Graphics::Holographic::HolographicSpace^);

			ANG_DECLARE_INTERFACE();

		protected:
			Windows::Graphics::Holographic::HolographicSpace^ space()const;

		protected:
			void OnLocatabilityChanged(Windows::Perception::Spatial::SpatialLocator ^sender, Platform::Object ^args);
			void OnCameraAdded(Windows::Graphics::Holographic::HolographicSpace ^sender, Windows::Graphics::Holographic::HolographicSpaceCameraAddedEventArgs ^args);
			void OnCameraRemoved(Windows::Graphics::Holographic::HolographicSpace ^sender, Windows::Graphics::Holographic::HolographicSpaceCameraRemovedEventArgs ^args);

		private:
			virtual~holographic_space();
					
			
		};




		class engine
			: public object
			, public singleton<engine_t>
		{
		private:
			holographic_space_t m_space;

		public:
			void init(holographic_space_t);
			void resume();
			void suspend();
			void update(core::time::step_timer_t);
			void render();
			void exit();

		public:
			ANG_DECLARE_INTERFACE();

		private:
			friend  singleton<engine_t>;
			engine();
			virtual~engine();
		};
	}
}