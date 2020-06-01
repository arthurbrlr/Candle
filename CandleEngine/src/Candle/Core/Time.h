#pragma once

#include "Candle/CandleCore.h"
#include "TimeAPI.h"

namespace Candle {

	class CANDLE_API Time {

		public:	
			static inline double Seconds() { return _API->Seconds(); }
			static inline double Milliseconds() { return _API->Milliseconds(); }
			static inline double DeltaTime() { return _API->DeltaTime(); }
			static inline double FixedDeltaTime() { return _API->FixedDeltaTime(); }

			static void TimeScale(double factor) { _API->TimeScale(factor); }

			static void Start() { _API->Start(); }
			static void BeginFrame() { _API->BeginFrame(); }
			static void Tick() { _API->Tick(); }

			operator double() { return _API->Seconds(); }

		protected:
			static TimeAPI* _API;
	};

}