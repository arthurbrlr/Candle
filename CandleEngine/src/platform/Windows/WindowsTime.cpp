#include "cdlpch.h"
#include "WindowsTime.h"

namespace Candle {

	void WindowsTime::Start()
	{
		_lastFrameTime = 0;
		_beginFrameTime = glfwGetTime();
		_time = _beginFrameTime;
		_fixedDeltaTime = _beginFrameTime;
	}


	void WindowsTime::BeginFrame()
	{
		_beginFrameTime = glfwGetTime();
		_time = _beginFrameTime;
		_fixedDeltaTime = _beginFrameTime - _lastFrameTime;
		_lastFrameTime = _beginFrameTime;
	}

	void WindowsTime::Tick()
	{
		_time = glfwGetTime();
	}

}