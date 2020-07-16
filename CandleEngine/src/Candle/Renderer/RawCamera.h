#pragma once

#include "Candle/CandleCore.h"

#include <glm/glm.hpp>

namespace Candle {

	enum CameraType {
		Orthographic, Perspective
	};


	class RawCamera {

		public:
			virtual ~RawCamera() {}

			glm::mat4& GetProjection();

		protected:
			void UpdateViewMatrix();

			glm::mat4 _projection = glm::mat4(0);
	};


	class OrthographicCamera : public RawCamera {

		public:
			OrthographicCamera();
			OrthographicCamera(double left, double right, double bottom, double top);
			void SetProjection(double left, double right, double bottom, double top);

			double GetHorizontalValue();
			double GetVerticalValue();

		private:
			double _left, _right, _bottom, _top;

	};


	class PerspectiveCamera : public RawCamera {

		public:
			PerspectiveCamera();
			PerspectiveCamera(double width, double height);
			void SetProjection(double width, double height);
			void SetWidth(double newWidth);
			void SetHeight(double newHeight);

			double GetWidth();
			double GetHeight();

		private:
			double _width, _height;
	};


}