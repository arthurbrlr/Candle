#pragma once

#include "Candle/CandleCore.h"
#include "glm/glm.hpp"

#include "AudioBuffer.h"

namespace Candle {

	class CANDLE_API AudioEmitter {

		public:
			virtual ~AudioEmitter() {}

			virtual void Play() = 0;
			virtual void Pause() = 0;
			virtual void Stop() = 0;
			virtual AudioEmitter& SetBuffer(Shared<AudioBuffer> buffer) = 0;

			AudioEmitter& SetPitch(double pitch) { _pitch = pitch; Update(); return *this; }
			AudioEmitter& SetGain(double gain) { _gain = gain; Update(); return *this; }
			AudioEmitter& SetLoop(bool state) { _loop = state; Update(); return *this; }
			AudioEmitter& SetPosition(const glm::vec3 & position) { _position = position; Update(); return *this; }
			AudioEmitter& SetVelocity(const glm::vec3 & velocity) { _velocity = velocity; Update(); return *this; }

			double Pitch() { return _pitch; }
			double Gain() { return _gain; }
			bool IsLooping() { return _loop; }
			glm::vec3 & Position() { return _position; }
			glm::vec3 & Velocity() { return _velocity; }
			
		protected:
			double _pitch = 1.0;
			double _gain = 1.0;
			bool _loop = false;
			glm::vec3 _position = { 0, 0, 0 };
			glm::vec3 _velocity = { 0, 0, 0 };
			
			virtual void Update() = 0;
	};

}