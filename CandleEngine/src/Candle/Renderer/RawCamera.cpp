#include "cdlpch.h"
#include "RawCamera.h"

#include "Candle/Application.h"

namespace Candle {

		// -------------------
		// Orthographic Camera
		// -------------------
	OrthographicCamera::OrthographicCamera(double left, double right, double bottom, double top)
	{
		SetProjection(left, right, bottom, top);
	}


	void OrthographicCamera::SetProjection(double left, double right, double bottom, double top)
	{
		_projection = glm::ortho(left, right, bottom, top, -1.0, 100.0);
	}

		// -------------------
		// Perspective Camera
		// -------------------

	PerspectiveCamera::PerspectiveCamera(double width, double height)
	{
		SetProjection(width, height);
	}


	void PerspectiveCamera::SetProjection(double width, double height)
	{
		if (height == 0.) return;
		_projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 300.0f);
	}
	
}