#pragma once

#include "Candle/CandleCore.h"
#include "glm/glm.hpp"

namespace Candle {

	class CANDLE_API AudioListener {

		public:
			AudioListener() 
			{
				_position = { 0, 0, 0 };
				_velocity = { 0, 0, 0 };
				_orientation = { 0, 0, 0 };
			}

			AudioListener& SetPosition(const glm::vec3 & position) { _position = position; }
			AudioListener& SetVelocity(const glm::vec3 & velocity) { _velocity = velocity; }
			AudioListener& SetOrientation(const glm::vec3 & orientation) { _orientation = orientation; }

			const glm::vec3 & Position() const { return _position; }
			const glm::vec3 & Velocity() const { return _velocity; }
			const glm::vec3 & Orientation() const { return _orientation; }


		private:
			glm::vec3 _position, _velocity, _orientation;
	};


}