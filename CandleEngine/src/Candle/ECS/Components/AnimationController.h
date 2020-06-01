#pragma once

#include "Candle/CandleCore.h"
#include "Candle/ECS/Component.h"

namespace Candle {

		// Start with 2D
	template<class T>
	struct ComponentWrapper {
		T componentKeyframe;
	};


	struct Keyframe {
		double keyframeTime;
		glm::vec4 textureCoordinates = { 0, 1, 0, 1 };
	};


		// List of keyframes
	struct Animation {

		Animation(const std::string & iTrigger, const std::vector<Keyframe> iKeyframes)
			: trigger(iTrigger), keyframes(iKeyframes)
		{
			if (iKeyframes.size() != 0)
				animationDuration = iKeyframes[iKeyframes.size() - 1].keyframeTime;
		}

		std::string trigger = "empty";
		std::vector<Keyframe> keyframes;

		double animationDuration = 1.;
		double animationProgress = 0.;
		double animationTime = 0.;
		double animationSpeed = 1.;
		
		bool play = false;
		bool loop = false;

		int currentKeyframe = 0;
	};


	class CANDLE_API AnimationController : public Component {

		public:
			AnimationController(){
				_name = "AnimationController";
			}

			AnimationController & AddAnimation(Animation & animation) { _animations.push_back(animation); return *this; }


			void OnEditor() override
			{
				static float animSpeed = (float)_animations[_currentAnimation].animationSpeed;
				ImGui::Text("Animation Speed:");
				ImGui::SliderFloat("s", &animSpeed, 0., 10.);
				_animations[_currentAnimation].animationSpeed = (double)animSpeed;

				for (auto& anim : _animations) {

					if (ImGui::TreeNode(anim.trigger.c_str())) {
						
						int i = 0;
						for (auto& keyframe : anim.keyframes) {

							std::string keyframeName = "Keyframe " + std::to_string(i++) + ", @ " + std::to_string(keyframe.keyframeTime) + " s";
							if (ImGui::TreeNode(keyframeName.c_str())) {

								ImGui::InputFloat4("TC", glm::value_ptr(keyframe.textureCoordinates), 3);
								ImGui::TreePop();
							}

						}

						ImGui::TreePop();

					}

				}
			}


			void Trigger(const std::string & trigger)
			{
				bool triggeredBool = false;

				for (int i = 0; i < _animations.size(); i++) {

					if (_animations[i].trigger == trigger) {
						if (_currentAnimation != -1) _animations[_currentAnimation].play = false;
						_currentAnimation = i;
						_animations[i].play = true;
						triggeredBool = true;
					}

				}

				if (!triggeredBool) CERROR("Animation {0} was not found in the animator controller.", trigger);
			}


			void UpdateAnimations(double deltaTime)
			{
				if (_currentAnimation == -1) return;

				#define canim _animations[_currentAnimation]

				canim.animationProgress += canim.animationSpeed * deltaTime / canim.animationDuration;
				canim.animationTime += canim.animationSpeed * deltaTime;

				if (canim.keyframes[canim.currentKeyframe].keyframeTime < canim.animationTime) {
					canim.currentKeyframe++;
				}

				if (canim.animationProgress > 1.) {
					canim.animationProgress = 0;
					canim.animationTime = 0.;
					canim.currentKeyframe = 0;

					if (!canim.loop) {
						canim.play = false;
						NextAnimation(_currentAnimation);
					}
				}

			}


			void NextAnimation(int finishedAnimation)
			{
				_currentAnimation = -1;
			}


			bool GetCurrentAnimation(Animation & anim) const
			{
				if (_animations.empty() || _currentAnimation == -1) return false;
				anim = _animations[_currentAnimation];
				return true;
			}

		private:
			int _currentAnimation = -1;
			std::vector<Animation> _animations;
			


	};

}