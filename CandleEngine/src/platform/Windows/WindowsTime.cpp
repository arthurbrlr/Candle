#include "cdlpch.h"
#include "Candle/Core/Time.h"

#include <GLFW/glfw3.h>

namespace Candle {

	double Time::_beginFrameTime = 0.,
		   Time::_time = 0.,
		   Time::_lastFrameTime = 0.,
		   Time::_fixedDeltaTime = 0.;
	double Time::_timeFactor = 1.;

	void Time::Start()
	{
		_lastFrameTime = 0;
		_beginFrameTime = glfwGetTime();
		_time = _beginFrameTime;
		_fixedDeltaTime = _beginFrameTime;
	}


	void Time::BeginFrame()
	{
		_beginFrameTime = glfwGetTime();
		_time = _beginFrameTime;
		_fixedDeltaTime = _beginFrameTime - _lastFrameTime;
		_lastFrameTime = _beginFrameTime;
	}

	void Time::Tick()
	{
		_time = glfwGetTime();
	}

	Time::operator double()
	{
		return Seconds();
	}

	double Time::Seconds() 
	{ 
		Tick();
		return _time; 
	}

	double Time::Milliseconds() 
	{ 
		Tick();
		return _time * 1000.; 
	}

	double Time::DeltaTime() 
	{
		return _fixedDeltaTime * _timeFactor; 
	}

	double Time::FixedDeltaTime() 
	{
		return _fixedDeltaTime; 
	}

	void Time::TimeScale(double factor)
	{
		_timeFactor = factor;
	}

}