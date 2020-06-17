#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Renderer/RawCamera.h"

#include "Candle/Events/MouseEvent.h"
#include "Candle/Events/AppEvent.h"

namespace Candle {

	class CANDLE_API EditorCameraController {

		public:
			EditorCameraController() {}
			EditorCameraController(double width, double height);

			void OnUpdate();
			void OnEvent(Event & event);

			inline void UseOrthographic(bool state) { _useOrthographic = state; }


			inline const glm::mat4 GetPVMatrix() 
			{
				return GetCamera().GetProjection() * _view;
			}
			
			inline const glm::mat4 GetPMatrix()
			{
				return GetCamera().GetProjection();
			}
			
			inline const glm::mat4 GetVMatrix() 
			{
				return _view;
			}

			inline OrthographicCamera & GetOrthographic() { return _orthographic; }
			inline PerspectiveCamera & GetPerspective() { return _perspective; }
			inline RawCamera & GetCamera()
			{
				if (_useOrthographic) return _orthographic;
				else return _perspective;
			}

			inline const double GetRatio() const { return _aspectRatio; }
			inline const double GetZoom() const { return _zoomLevel; }
			inline const glm::vec3 GetPosition() const { return _position; }
			inline const glm::vec3 GetRotation() const { return _rotation; }
			inline const bool UseOrthographic() const { return _useOrthographic; }


		private:
			double _aspectRatio = 16. / 9., _zoomLevel = 25.0;

			bool _useOrthographic = true;
			OrthographicCamera _orthographic;
			PerspectiveCamera _perspective;

			glm::vec3 _position;
			glm::vec3 _rotation;
			glm::mat4 _view = glm::mat4(0);

			void UpdateViewMatrix();

			bool OnMouseScrolled(MouseScrolledEvent & event);
			bool OnWindowResize(WindowResizeEvent & event);

	};

}