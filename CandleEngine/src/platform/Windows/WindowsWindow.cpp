#include "cdlpch.h"

#include "stb_image.h"

#include "Candle/Events/AppEvent.h"
#include "Candle/Events/KeyEvent.h"
#include "Candle/Events/MouseEvent.h"

#include "WindowsWindow.h"
#include "platform/OpenGL/GLContext.h"

#include "Candle/Core/Input.h"

namespace Candle {

	static bool _glfwInit = false;

	static void GLFWErrorCallback(int error, const char* desc)
	{
		CERROR("GLFW error ({0}) : {1}", error, desc);
	}


	WindowsWindow::WindowsWindow(const WindowProperties & props)
	{
		Init(props);
	}


	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}


	void WindowsWindow::Init(const WindowProperties & props)
	{
		_data._title = props._title;
		_data._width = props._width;
		_data._height = props._height;

		CINFO("Creating window {0} ({1} x {2})", _data._title, _data._width, _data._height);


		if (!_glfwInit) {
			int success = glfwInit();
			CASSERT(success, "GLFW was not initialized");

			/* Set the version of OpenGL used in the project */
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
			glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);

			_glfwInit = true;
		}
		
		_window = glfwCreateWindow((int)_data._width, (int)_data._height, _data._title.c_str(), nullptr, nullptr);
		glViewport(0, 0, (int)_data._width, (int)_data._height);
		glfwMakeContextCurrent(_window);
		SetVSync(false);


		std::string filePath = "res/textures/cdl_icon.png";
		int w, h, nbch;
		GLFWimage image[2];

		image[0].pixels = stbi_load(filePath.c_str(), &w, &h, &nbch, 4);
		image[0].width = w;
		image[0].height = h;

		image[1].pixels = stbi_load(filePath.c_str(), &w, &h, &nbch, 4);
		image[1].width = w;
		image[1].height = h;

		glfwSetWindowIcon(_window, 2, image);


		_context = new GLContext(_window);
		_context->Init();

		glfwSetWindowUserPointer(_window, &_data);

			// Setting all callbacks
		glfwSetWindowSizeCallback(_window, [](GLFWwindow* w, int width, int height) 
		{
			CTRACE("Resize Callback at {0} x {1}", width, height);
			WindowData & data = *(WindowData*)glfwGetWindowUserPointer(w);
			data._width = width;
			data._height = height;

			WindowResizeEvent event(width, height);
			data._eventCallback(event);
		});


		glfwSetWindowCloseCallback(_window, [](GLFWwindow* w) 
		{
			WindowData & data = *(WindowData*)glfwGetWindowUserPointer(w);

			WindowClosedEvent event;
			data._eventCallback(event);
		});

		
		glfwSetKeyCallback(_window, [](GLFWwindow* w, int key, int scan, int action, int mods) 
		{
			WindowData & data = *(WindowData*)glfwGetWindowUserPointer(w);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data._eventCallback(event);
					Input::RegisterKeyEvent(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data._eventCallback(event);
					Input::RegisterKeyEvent(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data._eventCallback(event);
					Input::RegisterKeyEvent(event);
					break;
				}
			}
		});


		glfwSetCharCallback(_window, [](GLFWwindow* w, unsigned int keycode)
		{
			WindowData & data = *(WindowData*)glfwGetWindowUserPointer(w);

			KeyTypedEvent event(keycode);
			data._eventCallback(event);
		});

		glfwSetMouseButtonCallback(_window, [](GLFWwindow* w, int button, int action, int mods) 
		{
			WindowData & data = *(WindowData*)glfwGetWindowUserPointer(w);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data._eventCallback(event);
					Input::RegisterMouseButtonEvent(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data._eventCallback(event);
					Input::RegisterMouseButtonEvent(event);
					break;
				}
			}
		});


		glfwSetScrollCallback(_window, [](GLFWwindow* w, double xOff, double yOff) 
		{
			WindowData & data = *(WindowData*)glfwGetWindowUserPointer(w);

			MouseScrolledEvent event(xOff, yOff);
			data._eventCallback(event);
		});


		glfwSetCursorPosCallback(_window, [](GLFWwindow* w, double x, double y) 
		{
			WindowData & data = *(WindowData*)glfwGetWindowUserPointer(w);

			MouseMovedEvent event(x, y);
			data._eventCallback(event);
		});


		glfwSetErrorCallback(GLFWErrorCallback);
	}


	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(_window);
	}


	void WindowsWindow::OnUpdate()
	{
		_context->SwapBuffers();
		Input::PollEvents();
	}


	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled) 
			glfwSwapInterval(1);
		else 
			glfwSwapInterval(0);

		_data._vsync = enabled;
	}


	void WindowsWindow::SetFullscreen(bool state)
	{
		GLFWmonitor* monitor = GetCurrentMonitor(_window);
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if ( state )
			glfwSetWindowMonitor(_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		else
			glfwSetWindowMonitor(_window, NULL, 0, 0, mode->width, mode->height, mode->refreshRate);
	}


	bool WindowsWindow::IsVSync() const
	{
		return _data._vsync;
	}


	void WindowsWindow::ResizeViewport() const
	{
		glViewport(0, 0, (int)_data._width, (int)_data._height);
	}


	GLFWmonitor* WindowsWindow::GetCurrentMonitor( GLFWwindow * window )
	{
		
		auto mini = [](int x, int y) {
			return x < y ? x : y;
		};

		auto maxi = [](int x, int y) {
			return x > y ? x : y;
		};
		
		int nmonitors, i;
		int wx, wy, ww, wh;
		int mx, my, mw, mh;
		int overlap, bestoverlap;
		GLFWmonitor* bestmonitor;
		GLFWmonitor** monitors;
		const GLFWvidmode* mode;

		bestoverlap = 0;
		bestmonitor = NULL;

		glfwGetWindowPos(window, &wx, &wy);
		glfwGetWindowSize(window, &ww, &wh);
		monitors = glfwGetMonitors(&nmonitors);

		for ( i = 0; i < nmonitors; i++ ) {
			mode = glfwGetVideoMode(monitors[i]);
			glfwGetMonitorPos(monitors[i], &mx, &my);
			mw = mode->width;
			mh = mode->height;

			overlap =
				maxi(0, mini(wx + ww, mx + mw) - maxi(wx, mx)) *
				maxi(0, mini(wy + wh, my + mh) - maxi(wy, my));

			if ( bestoverlap < overlap ) {
				bestoverlap = overlap;
				bestmonitor = monitors[i];
			}
		}

		return bestmonitor;
	}

}
