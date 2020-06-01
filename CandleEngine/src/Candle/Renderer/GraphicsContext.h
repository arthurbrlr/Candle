#pragma once

#include "Candle/CandleCore.h"

namespace Candle {

	class CANDLE_API GraphicsContext {

		public:

			virtual void Init() = 0;
			virtual void SwapBuffers() = 0;
			virtual void CleanUp() = 0;

			const virtual unsigned char* GetContextVersion() = 0;
			const virtual unsigned char* GetHardwareVendor() = 0;
			const virtual unsigned char* GetRenderer() = 0;

		private:
	};

}