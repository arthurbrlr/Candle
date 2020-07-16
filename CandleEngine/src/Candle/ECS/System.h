#pragma once

#include "Candle/CandleCore.h"
#include "Component.h"

namespace Candle {

	class System {

		public:
			virtual ~System() {}

			virtual void OnUpdate() {}
			virtual void OnEvent(Event & e) {}
			virtual void OnEditor() {}
			virtual void OnRenderUpdate() {}

		protected:

	};

}