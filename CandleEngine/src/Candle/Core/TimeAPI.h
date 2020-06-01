#pragma once

#include "Candle/CandleCore.h"

namespace Candle {

	class CANDLE_API TimeAPI {

	public:
		virtual ~TimeAPI() {}

		inline double Seconds() { Tick();  return _time; }
		inline double Milliseconds() { Tick();  return _time * 1000.; }
		inline double DeltaTime() { return _fixedDeltaTime * _timeFactor; }
		inline double FixedDeltaTime() { return _fixedDeltaTime; }

		virtual void TimeScale(double factor) { _timeFactor = factor; }

		virtual void Start() = 0;
		virtual void BeginFrame() = 0;
		virtual void Tick() = 0;

	protected:
		double _beginFrameTime = 0, _time = 0., _lastFrameTime = 0., _fixedDeltaTime = 0.;
		double _timeFactor = 1.;
	};

}