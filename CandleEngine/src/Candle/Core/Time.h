#pragma once

#include "Candle/CandleCore.h"
#include "TimeAPI.h"

namespace Candle {

	class CANDLE_API Time {

		public:	
			static void Start();
			static void BeginFrame();
			static void Tick();

			operator double();
			static double Seconds();
			static double Milliseconds();
			static double DeltaTime();
			static double FixedDeltaTime();

			static void TimeScale(double factor);

		protected:
			static double _beginFrameTime, _time, _lastFrameTime, _fixedDeltaTime;
			static double _timeFactor;
	};

}