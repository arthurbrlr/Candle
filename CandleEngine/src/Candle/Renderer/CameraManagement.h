#pragma once

#include "Candle/CandleCore.h"
#include "RawCamera.h"
#include "Candle/ECS/Components/Transform.h"

namespace Candle {

	class CANDLE_API CameraManagement {

		public:
			CameraManagement(const CameraManagement&) = delete;

			static void Init();
			static void UpdateView();
			static void UseView(const glm::mat4& newView) { _viewProjectionMatrix = newView; }
			static void RegisterMainCamera(long mainCameraBlueprintID);
			static void Reset() { _viewProjectionMatrix = glm::mat4(0); }

			static CameraManagement& Get() { return _instance; }
			static glm::mat4& GetViewProjection() { return _viewProjectionMatrix; }
			static glm::mat4& GetProjectionMatrix();
			static glm::mat4& GetViewMatrix();
			static Transform& GetCameraTransform();

		private:
			CameraManagement() {}

			static CameraManagement _instance;
			static long _mainCameraBlueprintID;
			static glm::mat4 _viewProjectionMatrix;
			static glm::mat4 _dummy;

	};

}