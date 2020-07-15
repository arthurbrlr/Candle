#include "cdlpch.h"
#include "RawCamera.h"

#include "Candle/Application.h"

namespace Candle {
		// ----------
		// Raw Camera
		// ----------
	glm::mat4& RawCamera::GetProjection()
	{
		return _projection;
	}

	void RawCamera::UpdateViewMatrix()
	{
	}


		// -------------------
		// Orthographic Camera
		// -------------------
	OrthographicCamera::OrthographicCamera()
	{
	}


	OrthographicCamera::OrthographicCamera(double left, double right, double bottom, double top)
		: _left(left), _right(right),_bottom(bottom), _top(top)
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
	PerspectiveCamera::PerspectiveCamera()
	{
	}


	PerspectiveCamera::PerspectiveCamera(double width, double height)
		: _width(width), _height(height)
	{
		SetProjection(width, height);
	}


		// Setters

	void PerspectiveCamera::SetProjection(double width, double height)
	{
		if (height == 0.) return;
		_projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 300.0f);
	}


	void PerspectiveCamera::SetHeight(double newHeight)
	{
		_height = newHeight;
		SetProjection(_width, _height);
	}


	void PerspectiveCamera::SetWidth(double newWidth)
	{
		_width = newWidth;
		SetProjection(_width, _height);
	}


		// Getters
	
	double PerspectiveCamera::GetHeight()
	{
		return _height;
	}


	double PerspectiveCamera::GetWidth()
	{
		return _width;
	}
}