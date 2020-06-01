#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Core/TimeAPI.h"

#include <GLFW/glfw3.h>

namespace Candle {

	class CANDLE_API WindowsTime : public TimeAPI {

		public:
			void Start() override;
			void BeginFrame() override;
			void Tick() override;

	};

}