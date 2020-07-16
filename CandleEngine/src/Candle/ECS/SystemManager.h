#pragma once

#include "Candle/CandleCore.h"
#include "System.h"

namespace Candle {

	class SystemManager {

		public:
			static void Update();
			static void OnEditor() { for (auto& s : _systems) s->OnEditor(); }
			static void OnRender() { for (auto& s : _systems) s->OnRenderUpdate(); }
			static void Add(System* system);

		private:
			static std::vector<Shared<System>> _systems;
	};

}