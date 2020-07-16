#include "cdlpch.h"
#include "AnimationController.h"

namespace Candle {

	AnimationController::AnimationController()
	{
		_name = "AnimationController";
	}


	void AnimationController::OnEditor()
	{
		static float animSpeed = (float)_animations[_currentAnimation]._animationSpeed;
		ImGui::Text("Animation Speed:");
		ImGui::DragFloat("s", &animSpeed, 0.1, 0.);
		_animations[_currentAnimation]._animationSpeed = (double)animSpeed;

		for ( auto& anim : _animations ) {

			if ( ImGui::TreeNode(anim._trigger.c_str()) ) {

				int i = 0;
				for ( auto& keyframe : anim._keyframes ) {

					std::string keyframeName = "Keyframe " + std::to_string(i++) + ", @ " + std::to_string(keyframe.keyframeTime) + " s";
					if ( ImGui::TreeNode(keyframeName.c_str()) ) {

						ImGui::InputFloat4("TC", glm::value_ptr(keyframe.textureCoordinates), 3);
						ImGui::TreePop();
					}

				}

				ImGui::TreePop();

			}

		}
	}


		/* Methods */

	AnimationController& AnimationController::AddAnimation(Animation& animation)
	{
		_animations.push_back(animation); 
		return *this; 
	}


	void AnimationController::Trigger(const std::string& trigger)
	{
		bool triggeredBool = false;

		for ( int i = 0; i < _animations.size(); i++ ) {

			if ( _animations[i]._trigger == trigger ) {
				if ( _currentAnimation != -1 ) _animations[_currentAnimation]._play = false;
				_currentAnimation = i;
				_animations[i]._play = true;
				triggeredBool = true;
			}

		}

		if ( !triggeredBool ) CERROR("Animation {0} was not found in the animator controller.", trigger);
	}


	void AnimationController::UpdateAnimations(double deltaTime)
	{
		if ( _currentAnimation == -1 ) return;

#define canim _animations[_currentAnimation]

		canim._animationProgress += canim._animationSpeed * deltaTime / canim._animationDuration;
		canim._animationTime += canim._animationSpeed * deltaTime;

		if ( canim._keyframes[canim._currentKeyframe].keyframeTime < canim._animationTime ) {
			canim._currentKeyframe++;
		}

		if ( canim._animationProgress > 1. ) {
			canim._animationProgress = 0;
			canim._animationTime = 0.;
			canim._currentKeyframe = 0;

			if ( !canim._loop ) {
				canim._play = false;
				NextAnimation(_currentAnimation);
			}
		}

	}


	void AnimationController::NextAnimation(int finishedAnimation)
	{
		_currentAnimation = -1;
	}


	bool AnimationController::GetCurrentAnimation(Animation& anim) const
	{
		if ( _animations.empty() || _currentAnimation == -1 ) return false;
		anim = _animations[_currentAnimation];
		return true;
	}

}