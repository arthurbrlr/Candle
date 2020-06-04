#include "cdlpch.h"
#include "Window.h"

#include "platform/Windows/WindowsWindow.h"

namespace Candle {

	Window* Window::Create(const WindowProperties& props)
	{
		switch ( SYSTEM ) {
		case WIN64:
			return new WindowsWindow(props);
		case MACOS:
			CASSERT(false, "Mac is not supported yet");
			break;
		case LINUX:
			CASSERT(false, "Linux is not supported yet");
			break;
		}

		return nullptr;
	}

}