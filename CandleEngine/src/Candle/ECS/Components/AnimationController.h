#pragma once

#include "Candle/CandleCore.h"
#include "Candle/ECS/Component.h"

#include "Animations/AnimationMap.h"

namespace Candle {

	class CANDLE_API AnimationController : public Component {

		public:
			AnimationController();
			void OnEditor() override;

			AnimationController& AddAnimation(Animation& animation);

			void Trigger(const std::string& trigger);
			void UpdateAnimations(double deltaTime);
			void NextAnimation(int finishedAnimation);
			bool GetCurrentAnimation(Animation& anim) const;

		private:
			int _currentAnimation = -1;
			std::vector<Animation> _animations;

	};

}