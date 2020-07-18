#pragma once

#include "Candle/CandleCore.h"

#include "Scene.h"
#include "Candle/ECS/Blueprint.h"
#include "Candle/ECS/Components/CameraHandler.h"

namespace Candle {

	class EmptyScene : public Scene {

		public:
			void Load() override 
			{
				Blueprint& mainCamera = ECS::New("mainCamera");
				mainCamera.AddComponent<CameraHandler>();
			}
			void OnUpdate() override {}
	};

}