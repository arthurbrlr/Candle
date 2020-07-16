#pragma once

#include "Candle/CandleCore.h"

#include "Scene.h"

namespace Candle {

	class EmptyScene : public Scene {

		public:
			void Load() override {}
			void OnUpdate() override {}
	};

}