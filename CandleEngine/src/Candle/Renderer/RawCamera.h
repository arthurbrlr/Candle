#pragma once

#include "Candle/CandleCore.h"

#include <glm/glm.hpp>

namespace Candle {

	enum CameraType {
		Orthographic, Perspective
	};


	class CANDLE_API RawCamera {

		public:
			virtual ~RawCamera() {}

			inline glm::mat4& GetProjection() { return _projection; }

		protected:
			void UpdateViewMatrix();

			glm::mat4 _projection = glm::mat4(0);
	};


	class CANDLE_API OrthographicCamera : public RawCamera {

		public:
			OrthographicCamera() {}
			OrthographicCamera(double left, double right, double bottom, double top);
			void SetProjection(double left, double right, double bottom, double top);

	};


	class CANDLE_API PerspectiveCamera : public RawCamera {

		public:
			PerspectiveCamera() {}
			PerspectiveCamera(double width, double height);
			void SetProjection(double width, double height);

	};


}