#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Core/Time.h"
#include "Candle/ECS/System.h"

#include "Candle/ECS/Components/AnimationController.h"


namespace Candle {

	class CANDLE_API AnimationSystem : public System {

		public:
			
			void OnUpdate() override
			{

				std::vector<Candle::Shared<Candle::Blueprint>> bps = Candle::BlueprintManager::Get<AnimationController>();

				for (auto& bp : bps) {
					AnimationController& controller = bp->GetComponent<AnimationController>();
					if (!controller.IsActive()) continue;
					controller.UpdateAnimations(Time::DeltaTime());
				}

			}
			



	};


}