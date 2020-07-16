#pragma once

#include "Candle/CandleCore.h"

namespace Candle {

	struct RayData {
		glm::vec3 origin;
		glm::vec3 direction;
		double length;
	};

	struct RayCollisionData {
		glm::vec3 origin;
		glm::vec3 direction;
		double length;

		bool hasHit = false;
		uint32_t hitID = -1;
	};


	class Ray {

		public:
			
			static RayCollisionData Cast(RayData data);
			static RayCollisionData CastFromMouse();


	};

}