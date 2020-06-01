#pragma once

#include <cdlpch.h>

#include "Candle/CandleCore.h"
#include "Candle/Events/Event.h"
#include "Candle/Renderer/GraphicsContext.h"

namespace Candle {

	struct WindowProperties {
		std::string _title;
		unsigned int _width;
		unsigned int _height;

		WindowProperties (const std::string & title = "Candle Engine",
						  unsigned int width = 1920,
						  unsigned int height = 1080)
			: _title(title), _width(width), _height(height) {}
	};

	class CANDLE_API Window {

		public:
			using EventCallbackFunction = std::function<void(Event&)>;

			virtual ~Window() {}
			
			static Window* Create(const WindowProperties & props = WindowProperties());
			virtual void OnUpdate() = 0;


			virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;
			virtual void SetVSync(bool enabled) = 0;
			virtual void SetFullscreen(bool state) = 0;
			virtual void ResizeViewport() const = 0;


			virtual void* GetNativeWindow() const = 0;
			virtual GraphicsContext& GetContext() const = 0;
			virtual unsigned int GetWidth() const = 0;
			virtual unsigned int GetHeight() const = 0;
			virtual bool IsVSync() const = 0;
	};


}