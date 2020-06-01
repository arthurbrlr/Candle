#pragma once

#include "Candle/CandleCore.h"

namespace Candle {

	class CANDLE_API CollisionWorld {
		
		public:
			static CollisionWorld& Get() { return *_instance; }



		private:
			static CollisionWorld* _instance;

			CollisionWorld() {}
			CollisionWorld(const CollisionWorld& other) = delete;

	};

}