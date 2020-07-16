#pragma once

#include <cdlpch.h>

#include "Candle/CandleCore.h"
#include "Candle/Events/Event.h"
#include "Candle/Renderer/GraphicsContext.h"

namespace Candle {

	struct WindowProperties {
		std::string _title;
		uint32_t _width;
		uint32_t _height;

		WindowProperties (const std::string & title = "Candle Engine",
						  uint32_t width = 1920,
						  uint32_t height = 1080)
			: _title(title), _width(width), _height(height) {}
	};

	class Window {

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
			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;
			virtual bool IsVSync() const = 0;
	};


}