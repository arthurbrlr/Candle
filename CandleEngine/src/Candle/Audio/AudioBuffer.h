#pragma once

#include "Candle/CandleCore.h"

namespace Candle {

	class AudioBuffer {

		public:
			virtual ~AudioBuffer() {}
			
			uint32_t GetBufferID() { return _bufferID; }

		private:
			uint32_t _bufferID;
	};


}