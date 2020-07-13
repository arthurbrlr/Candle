#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Renderer/RawCamera.h"
#include "Candle/ECS/Components/Transform.h"

#include "Candle/Events/MouseEvent.h"
#include "Candle/Events/AppEvent.h"

namespace Candle {

	class CANDLE_API EditorCamera {

		public:
			EditorCamera() {}
			EditorCamera(double width, double height);

			void OnUpdate();
			void OnEvent(Event & event);

			void ResetView();

			void UseOrthographic(bool state);

			const glm::mat4 GetPVMatrix();
			const glm::mat4 GetPMatrix();
			const glm::mat4 GetVMatrix();

			RawCamera& GetCamera();
			OrthographicCamera& GetOrthographic();
			PerspectiveCamera& GetPerspective();
			const Transform& GetTargetTransform() const;

			const double GetRatio() const;
			const double GetZoom() const;
			const glm::vec3 GetPosition() const;
			const glm::vec3 GetRotation() const;
			const bool UseOrthographic() const;


		private:
			double _aspectRatio = 16. / 9., _zoomLevel = 15.0;

			bool _useOrthographic = true;
			OrthographicCamera _orthographic;
			PerspectiveCamera _perspective;

			Transform _cameraTransform;
			Transform _targetTransform;
			
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

			bool OnMouseScrolled(MouseScrolledEvent& event);
			bool OnMouseMoved(MouseMovedEvent& event);
			bool OnWindowResize(WindowResizeEvent& event);

	};

}