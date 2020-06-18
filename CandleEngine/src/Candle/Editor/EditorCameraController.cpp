#include "cdlpch.h"
#include "EditorCameraController.h"

#include "Candle/Core/Input.h"
#include "Candle/Core/KeyCodes.h"
#include "Candle/Core/Time.h"

#include "Candle/Renderer/Renderer2D.h"


namespace Candle {

	EditorCameraController::EditorCameraController(double width, double height)
	{
		_aspectRatio = width / height;
		_orthographic = OrthographicCamera(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
		_perspective = PerspectiveCamera(width, height);
		
		UpdateCameraPosition();
		UpdateViewMatrix();
	}


	void EditorCameraController::OnUpdate()
	{

		if ( _useOrthographic ) {
			UpdateOrthographicMovement();
		} else {
			UpdatePerspectiveMovement();
		}

	}


	void EditorCameraController::UpdateOrthographicMovement()
	{
		bool changed = false;

		if ( Input::IsKeyPressed(CDL_KEY_UP) ) {
			_cameraTransform.Move({ 0, 5.f * (float)Time::FixedDeltaTime(), 0 });
			_targetTransform.Move({ 0, 5.f * (float)Time::FixedDeltaTime(), 0 });
			changed = true;
		}

		if ( Input::IsKeyPressed(CDL_KEY_DOWN) ) {
			_cameraTransform.Move({ 0, -5.f * (float)Time::FixedDeltaTime(), 0 });
			_targetTransform.Move({ 0, -5.f * (float)Time::FixedDeltaTime(), 0 });
			changed = true;
		}

		if ( Input::IsKeyPressed(CDL_KEY_LEFT) ) {
			_cameraTransform.Move({ -5.f * (float)Time::FixedDeltaTime(), 0, 0 });
			_targetTransform.Move({ -5.f * (float)Time::FixedDeltaTime(), 0, 0 });
			changed = true;
		}

		if ( Input::IsKeyPressed(CDL_KEY_RIGHT) ) {
			_cameraTransform.Move({ 5.f * (float)Time::FixedDeltaTime(), 0, 0 });
			_targetTransform.Move({ 5.f * (float)Time::FixedDeltaTime(), 0, 0 });
			changed = true;
		}

		if ( changed ) {
			UpdateViewMatrix();
		}
	}


	void EditorCameraController::UpdatePerspectiveMovement()
	{
		bool changed = false;

		glm::vec3 currentTargetPos = _targetTransform.GetPosition();
		glm::vec3 forward = _cameraTransform.GetForward();
		glm::vec3 right = _cameraTransform.GetRight();


		if ( Input::IsKeyPressed(CDL_KEY_UP) ) {
			_targetTransform.Move(-5.f * (float)Time::FixedDeltaTime() * forward);
			changed = true;
		}

		if ( Input::IsKeyPressed(CDL_KEY_DOWN) ) {
			_targetTransform.Move(5.f * (float)Time::FixedDeltaTime() * forward);
			changed = true;
		}

		if ( Input::IsKeyPressed(CDL_KEY_LEFT) ) {
			_targetTransform.Move(-5.f * (float)Time::FixedDeltaTime() * right);
			changed = true;
		}

		if ( Input::IsKeyPressed(CDL_KEY_RIGHT) ) {
			_targetTransform.Move(5.f * (float)Time::FixedDeltaTime() * right);
			changed = true;
		}


		if ( changed || _mouseScrolled || Input::IsMouseButtonPressed(1) ) {
			UpdateCameraPosition();
			UpdateViewMatrix();
			_mouseScrolled = false;
		}
	}


	void EditorCameraController::ResetView()
	{
		_targetTransform.SetPosition({ 0, 0, 0 });
		_angleAroundPlayer = 0;
		_pitch = 0;
		_distanceFromTarget = 7;

		UpdateCameraPosition();
		UpdateViewMatrix();
	}


	void EditorCameraController::OnEvent(Event & event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNCTION(EditorCameraController::OnMouseScrolled));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FUNCTION(EditorCameraController::OnMouseMoved));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(EditorCameraController::OnWindowResize));
	}


	void EditorCameraController::UpdateViewMatrix()
	{
		_view = glm::mat4(1.0f);

		// Rotation
		_view = glm::rotate(_view, glm::radians(_cameraTransform.GetRotation().x), glm::vec3(1.f, 0.f, 0.f));
		_view = glm::rotate(_view, glm::radians(_cameraTransform.GetRotation().y), glm::vec3(0.f, 1.f, 0.f));
		_view = glm::rotate(_view, glm::radians(_cameraTransform.GetRotation().z), glm::vec3(0.f, 0.f, 1.f));

		_view = glm::rotate(_view, glm::radians((float)_pitch), _cameraTransform.GetRight());

		// Translation
		glm::vec3 negativeCameraPos = glm::vec3(-_cameraTransform.GetPosition().x, -_cameraTransform.GetPosition().y, -_cameraTransform.GetPosition().z);
		_view = glm::translate(_view, negativeCameraPos);
	}


	void EditorCameraController::UpdateCameraPosition()
	{
		_distanceFromTargetVector = {
			_distanceFromTarget * std::cos(glm::radians(_pitch)),
			_distanceFromTarget * std::sin(glm::radians(_pitch))
		};

		double theta = _targetTransform.GetRotation().y + _angleAroundPlayer;

		double cameraXOffset = _distanceFromTargetVector.x * std::sin(glm::radians(theta));
		double cameraZOffset = _distanceFromTargetVector.x * std::cos(glm::radians(theta));

		glm::vec3 cameraPos = {
			_targetTransform.GetPosition().x + cameraXOffset,
			_targetTransform.GetPosition().y + _distanceFromTargetVector.y,
			_targetTransform.GetPosition().z + cameraZOffset
		};
		_cameraTransform.SetPosition(cameraPos);

		glm::vec3 currentRotation = _cameraTransform.GetRotation();
		currentRotation.y = - ( _targetTransform.GetRotation().y + _angleAroundPlayer );
		_cameraTransform.SetRotation(currentRotation);
	}


	bool EditorCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		if ( _useOrthographic ) {

			_zoomLevel -= event.GetYOffset();
			if ( _zoomLevel <= 0. ) _zoomLevel = 1.0;

			_orthographic.SetProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
			_distanceFromTarget = _zoomLevel;

		} else {

			float scrollValue = event.GetYOffset();
			_distanceFromTarget += scrollValue;

			if ( _distanceFromTarget < 1 ) _distanceFromTarget = 1;
			else if ( _distanceFromTarget > 100 ) _distanceFromTarget = 100;

		}

		_mouseScrolled = true;

		return false;
	}


	bool EditorCameraController::OnMouseMoved(MouseMovedEvent& event)
	{

		if ( Input::IsMouseButtonPressed(1) && !_useOrthographic ) {

			auto clamp = [](double value, double upper, double lower) -> double {
				if ( value < lower ) return lower;
				if ( value > upper ) return upper;
				return value;
			};

			double dx = event.GetPX() - event.GetX();
			double dy = event.GetPY() - event.GetY();

			double dPitch = dy * _distanceFromTarget * 0.05;
			double dAngle = dx * _distanceFromTarget * 0.1;

			_pitch -= clamp(dPitch, 0.75, -0.75);
			_angleAroundPlayer += clamp(dAngle, 0.75, -0.75);
		}


		if ( Input::IsMouseButtonPressed(1) && _useOrthographic ) {

			double dx = ( event.GetPX() - event.GetX() ) * Time::FixedDeltaTime() * _zoomLevel;
			double dy = ( event.GetPY() - event.GetY() ) * Time::FixedDeltaTime() * _zoomLevel;

			_targetTransform.Move({ dx, -dy, 0 });
			_cameraTransform.Move({ dx, -dy, 0 });

			UpdateViewMatrix();
		}

		return false;
	}


	bool EditorCameraController::OnWindowResize(WindowResizeEvent & event)
	{
		_aspectRatio = (double)event.GetWidth() / (double)event.GetHeight();
		_perspective.SetProjection((double)event.GetWidth(), (double)event.GetHeight());
		_orthographic.SetProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
		return false;
	}

		/* Setters */
	void EditorCameraController::UseOrthographic(bool state)
	{
		_useOrthographic = state; 
	}


		/* Getters */
	const glm::mat4 EditorCameraController::GetPVMatrix()
	{
		return GetCamera().GetProjection() * _view;
	}

	const glm::mat4 EditorCameraController::GetPMatrix()
	{
		return GetCamera().GetProjection();
	}

	const glm::mat4 EditorCameraController::GetVMatrix()
	{
		return _view;
	}

	RawCamera& EditorCameraController::GetCamera()
	{
		if ( _useOrthographic ) return _orthographic;
		else return _perspective;
	}

	OrthographicCamera& EditorCameraController::GetOrthographic()  
	{
		return _orthographic; 
	}

	PerspectiveCamera& EditorCameraController::GetPerspective()
	{
		return _perspective;
	}

	const Transform& EditorCameraController::GetTargetTransform() const
	{
		return _targetTransform; 
	}

	const double EditorCameraController::GetRatio() const
	{
		return _aspectRatio; 
	}

	const double EditorCameraController::GetZoom() const
	{
		return _zoomLevel; 
	}

	const glm::vec3 EditorCameraController::GetPosition() const
	{
		return _cameraTransform.GetPosition(); 
	}

	const glm::vec3 EditorCameraController::GetRotation() const
	{
		return _cameraTransform.GetPosition(); 
	}

	const bool EditorCameraController::UseOrthographic() const
	{
		return _useOrthographic; 
	}


}
