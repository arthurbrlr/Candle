#pragma once

#include "Candle/CandleCore.h"

#include "Scene.h"
#include "Candle/ECS/Entity.h"
#include "Candle/ECS/ECS.h"
#include "Candle/ECS/Components/CameraHandler.h"

#include "Candle/ECS/Components/EngineComponents.h"

namespace Candle {

	class EmptyScene : public Scene {

		public:
			EmptyScene()
			{
				_sceneName = "Empty Scene";
			}

			void Load() override 
			{
				Entity mainCamera = ECS::New("Camera");
				mainCamera.AddComponent<CameraHandler>(CameraType::Perspective, 1920, 1080);
				mainCamera.GetComponent<CameraHandler>().SetAsMainCamera(true);
				mainCamera.AddComponent<Transform>(glm::vec3{ 0,0,10 });
			}
			void OnUpdate() override {}
	};

}