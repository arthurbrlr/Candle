#include "cdlpch.h"
#include "EditorCameraController.h"

#include "Candle/Core/Input.h"
#include "Candle/Core/KeyCodes.h"
#include "Candle/Core/Time.h"


namespace Candle {

	EditorCameraController::EditorCameraController(double width, double height)
	{
		_aspectRatio = width / height;
		_orthographic = OrthographicCamera(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
		_perspective = PerspectiveCamera(width, height);
		
		_position = { 0, 0, _zoomLevel };
		_rotation = { 0, 0, 0 };

		UpdateViewMatrix();
	}


	void EditorCameraController::OnUpdate()
	{
		bool changed = false;

		if (Input::IsKeyPressed(CDL_KEY_UP))
		{
			_position += glm::vec3(0, 0.5 * Time::FixedDeltaTime() * std::abs(_zoomLevel), 0);
			changed = true;
		}

		if (Input::IsKeyPressed(CDL_KEY_DOWN))
		{
			_position += glm::vec3(0, -0.5 * Time::FixedDeltaTime() * std::abs(_zoomLevel), 0);
			changed = true;
		}

		if (Input::IsKeyPressed(CDL_KEY_LEFT))
		{
			_position += glm::vec3(-0.5 * Time::FixedDeltaTime() * std::abs(_zoomLevel), 0, 0);
			changed = true;
		}

		if (Input::IsKeyPressed(CDL_KEY_RIGHT))
		{
			_position += glm::vec3(0.5 * Time::FixedDeltaTime() * std::abs(_zoomLevel), 0, 0);
			changed = true;
		}

		if ( Input::IsKeyPressed(CDL_KEY_Q) ) {
			_rotation += glm::vec3(0, -45 * Time::FixedDeltaTime(), 0);
			changed = true;
		}

		if ( Input::IsKeyPressed(CDL_KEY_E) ) {
			_rotation += glm::vec3(0, 45 * Time::FixedDeltaTime(), 0);
			changed = true;
		}

		if ( changed ) UpdateViewMatrix();

	}


	void EditorCameraController::OnEvent(Event & event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNCTION(EditorCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(EditorCameraController::OnWindowResize));
	}


	bool EditorCameraController::OnMouseScrolled(MouseScrolledEvent & event)
	{
		_zoomLevel -= event.GetYOffset();
		if (_zoomLevel <= 0.) _zoomLevel = 1.0;

		_orthographic.SetProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
		_position.z = _zoomLevel;
		UpdateViewMatrix();

		return false;
	}


	bool EditorCameraController::OnWindowResize(WindowResizeEvent & event)
	{
		_aspectRatio = (double)event.GetWidth() / (double)event.GetHeight();
		_perspective.SetProjection((double)event.GetWidth(), (double)event.GetHeight());
		_orthographic.SetProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
		return false;
	}


	void EditorCameraController::UpdateViewMatrix()
	{
		_view = glm::mat4(1.0f);

		// Rotation
		_view = glm::rotate(_view, glm::radians(_rotation.x), glm::vec3(1.f, 0.f, 0.f));
		_view = glm::rotate(_view, glm::radians(_rotation.y), glm::vec3(0.f, 1.f, 0.f));
		_view = glm::rotate(_view, glm::radians(_rotation.z), glm::vec3(0.f, 0.f, 1.f));

		// Translation
		glm::vec3 negativeCameraPos = glm::vec3(-_position.x, -_position.y, -_position.z);
		_view = glm::translate(_view, negativeCameraPos);
	}

}
