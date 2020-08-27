#pragma once

#include "Candle/CandleCore.h"
#include "Candle/ECS/Component.h"
#include "Candle/Renderer/RawCamera.h"
#include "glm/glm.hpp"


namespace Candle {

	class CameraHandler : public Component {

		public:
			//CameraHandler(Blueprint* parent);
			//CameraHandler(Blueprint* parent, CameraType type, double horizontalSize, double verticalSize);
			CameraHandler();
			CameraHandler(CameraType type, double horizontalSize, double verticalSize);
			~CameraHandler();

			void Serialize(std::fstream& sceneFile) override;
			void Deserialize(std::fstream& sceneFile) override;
			void OnEditor() override;

			CameraHandler& SetAsMainCamera(bool state);
			
			glm::mat4 UpdatePVMatrix();

			glm::mat4 GetProjectionView();
			glm::mat4 GetViewProjection();
			glm::mat4& GetView();
			glm::mat4& GetProjection();

		private:
			RawCamera* _camera = nullptr;
			CameraType _type = Perspective;
			glm::mat4 _viewMatrix = glm::mat4(0);
			bool _isMainCamera = true;

			void ComputeViewMatrix();

			CANDLE_DECL_COMPONENT(CameraHandler)

	};

}