#pragma once

#include "Candle/CandleCore.h"

namespace Candle {

	class CANDLE_API AudioBuffer {

		public:
			virtual ~AudioBuffer() {}
			
			unsigned int GetBufferID() { return _bufferID; }

		private:
			unsigned int _bufferID;
	};


}