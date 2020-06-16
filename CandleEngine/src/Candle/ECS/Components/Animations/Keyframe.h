#pragma once

#include "Candle/CandleCore.h"
#include "glm/glm.hpp"

namespace Candle {

	struct Keyframe {
		double keyframeTime;

			// SpriteRenderer Keyframe data
		glm::vec4 textureCoordinates = { 0, 1, 0, 1 };
	};

}