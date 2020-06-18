#pragma once

#include "Candle/CandleCore.h"
#include "Candle/ECS/Component.h"
#include "Candle/Renderer/RawCamera.h"
#include "glm/glm.hpp"


namespace Candle {

	class CANDLE_API CameraHandler : public Component {

		public:
			CameraHandler(CameraType type, double horizontalSize, double verticalSize);
			void OnEditor() override;

			CameraHandler& SetAsMainCamera(bool state);
			
			glm::mat4 UpdatePVMatrix();

			glm::mat4 GetProjectionView();
			glm::mat4 GetViewProjection();
			glm::mat4& GetView();
			glm::mat4& GetProjection();

		private:
			Unique<RawCamera> _camera;
			CameraType _type;
			glm::mat4 _viewMatrix = glm::mat4(0);
			bool _isMainCamera = true;

			void ComputeViewMatrix();

	};

}