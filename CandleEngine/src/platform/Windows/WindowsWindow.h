#pragma once

#include "Candle/Renderer/GraphicsContext.h"
#include "Candle/Core/Window.h"

struct GLFWwindow;
struct GLFWmonitor;

namespace Candle {

	class WindowsWindow : public Window {

		public:
			WindowsWindow(const WindowProperties & props);
			virtual ~WindowsWindow();

			void OnUpdate() override;


			inline void SetEventCallback(const EventCallbackFunction & callback) override { _data._eventCallback = callback; }
			void SetVSync(bool enabled) override;
			void SetFullscreen(bool state) override;
			void ResizeViewport() const override;

			inline void* GetNativeWindow() const override { return _window; };
			inline GraphicsContext& GetContext() const override { return *_context; }
			bool IsVSync() const override;
			inline unsigned int GetWidth() const override { return _data._width; };
			inline unsigned int GetHeight() const override { return _data._height; };

		private:
			GLFWwindow* _window;
			GraphicsContext* _context;

			struct WindowData {
				std::string _title;
				unsigned int _width;
				unsigned int _height;
				bool _vsync;
				EventCallbackFunction _eventCallback;
			} _data;

			void Init(const WindowProperties & props);
			void Shutdown();
			GLFWmonitor* GetCurrentMonitor(GLFWwindow* window);

	};

}