#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Renderer/RawCamera.h"
#include "Candle/ECS/Components/Transform.h"

#include "Candle/Events/MouseEvent.h"
#include "Candle/Events/AppEvent.h"

class EditorCamera {

	public:
		EditorCamera() {}
		EditorCamera(double width, double height);

		void OnUpdate();
		void OnEvent(Candle::Event & event);

		void ResetView();

		void UseOrthographic(bool state);

		const glm::mat4 GetPVMatrix();
		const glm::mat4 GetPMatrix();
		const glm::mat4 GetVMatrix();

		Candle::RawCamera& GetCamera();
		Candle::OrthographicCamera& GetOrthographic();
		Candle::PerspectiveCamera& GetPerspective();
		const Candle::Transform& GetTargetTransform() const;

		const double GetRatio() const;
		const double GetZoom() const;
		const glm::vec3 GetPosition() const;
		const glm::vec3 GetRotation() const;
		const bool UseOrthographic() const;


	private:
		double _aspectRatio = 16. / 9., _zoomLevel = 15.0;

		bool _useOrthographic = true;
		Candle::OrthographicCamera _orthographic;
		Candle::PerspectiveCamera _perspective;

		Candle::Transform _cameraTransform;
		Candle::Transform _targetTransform;
			
		double _distanceFromTarget = 7;
		glm::vec2 _distanceFromTargetVector; /* Distance on both axis */
		double _angleAroundPlayer = 0;
		double _pitch = 0;
		bool _mouseScrolled = false;

		glm::mat4 _view = glm::mat4(0);

		void UpdateOrthographicMovement();
		void UpdatePerspectiveMovement();

		void UpdateCameraPosition();
		void UpdateViewMatrix();

		bool OnMouseScrolled(Candle::MouseScrolledEvent& event);
		bool OnMouseMoved(Candle::MouseMovedEvent& event);
		bool OnWindowResize(Candle::WindowResizeEvent& event);

};
