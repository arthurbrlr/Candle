#include "cdlpch.h"
#include "Animation.h"

namespace Candle {

	Animation::Animation(const std::string& iTrigger, const std::vector<Keyframe> iKeyframes)
		: _trigger(iTrigger), _keyframes(iKeyframes)
	{
		if ( iKeyframes.size() != 0 )
			_animationDuration = iKeyframes[iKeyframes.size() - 1].keyframeTime;
	}


	const std::vector<Keyframe>& Animation::GetKeyframes() const
	{
		return _keyframes;
	}

	const int Animation::GetCurrentKeyFrameIndex() const
	{
		return _currentKeyframe;
	}

}