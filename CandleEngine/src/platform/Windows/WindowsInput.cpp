#include "cdlpch.h"
#include "WindowsInput.h"
#include "Candle/Application.h"

#include <GLFW/glfw3.h>

namespace Candle {

	Input* Input::_instance = new WindowsInput();

	void WindowsInput::PollEventsImpl()
	{
		glfwPollEvents();
	}

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
		int pressed = glfwGetKey(window, keycode);
		return pressed == GLFW_PRESS || pressed == GLFW_REPEAT;
	}


	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
		int pressed = glfwGetMouseButton(window, button);
		return pressed == GLFW_PRESS;
	}
	
	
	double WindowsInput::GetMouseXImpl()
	{
		auto[x, y] = GetMousePosImpl();
		return x;
	}
	
	
	double WindowsInput::GetMouseYImpl()
	{
		auto[x, y] = GetMousePosImpl();
		return y;
	}


	std::pair<double, double> WindowsInput::GetMousePosImpl()
	{
		GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		//int left, right, top, bottom;
		//glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);
		return { x, y };
	}

}
