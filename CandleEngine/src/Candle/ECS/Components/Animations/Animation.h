#pragma once

#include "Candle/CandleCore.h"
#include "Keyframe.h"

#include <vector>
#include <string>

namespace Candle {

	// temporary lines to fix every errors
	class AnimationController;

	// List of keyframes
	class Animation {

		public:
			Animation(const std::string& iTrigger, const std::vector<Keyframe> iKeyframes);

			const std::vector<Keyframe>& GetKeyframes() const;
			const int GetCurrentKeyFrameIndex() const;

		private:
			std::string _trigger = "empty";
			std::vector<Keyframe> _keyframes;

			double _animationDuration = 1.;
			double _animationProgress = 0.;
			double _animationTime = 0.;
			double _animationSpeed = 1.;

			bool _play = false;
			bool _loop = false;

			int _currentKeyframe = 0;

			// temporary lines to fix every errors
			friend class AnimationController;
	};

}