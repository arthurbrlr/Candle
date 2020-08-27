#pragma once

#include "CandleCore.h"

#include "Core/Window.h"
#include "Core/Keycodes.h"

#include "Events/AppEvent.h"
#include "Events/KeyEvent.h"


namespace Candle {

	class Application
	{
		public:	
			Application				(const std::string & appName = "Candle Application");
			virtual ~Application	();

			void Run				();

			void OnEvent			(Event & event);

			inline static Application& Get() { return *_instance; }
			inline static void SetFullScreen(bool state) 
			{
				_instance->Get()._fullscreened = state;
				_instance->GetWindow().SetFullscreen(state); 
			}

			static void SetTitle(const std::string& title) { _instance->GetWindow().SetTitle(title); }
			static void SetChangesState(bool newState) { _instance->GetWindow().SetChangesState(newState); }

			inline Window& GetWindow() const { return *_window; }
			
			static uint32_t GetWidth();
			static uint32_t GetHeight();
			static double AspectRatio();
			static bool IsFullScreen();
			static void Stop();


		protected:
			static Application* _instance;
			bool OnWindowClose(WindowClosedEvent & e);
			bool OnWindowResize(WindowResizeEvent & e);
			bool OnKeyPressed(KeyPressedEvent & e);

			Unique<Window> _window;
			bool _running = true;
			bool _minimized = false;
			bool _fullscreened = true;

	};

		// Defined in the client
	Application* CreateApplication();
}

#define CDL_APP_WIDTH Application::GetWidth()
#define CDL_APP_HEIGHT Application::GetHeight()

