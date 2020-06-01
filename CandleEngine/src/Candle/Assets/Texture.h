#pragma once

#include "Candle/CandleCore.h"

namespace Candle {

	class CANDLE_API Texture {

		public:
			virtual ~Texture() {}

			inline virtual void Bind(unsigned int slot = 0) const = 0;
			inline virtual void Unbind() const = 0;
			inline virtual void SetData(void* data, unsigned int size) = 0;

			inline virtual int GetWidth() const = 0;
			inline virtual int GetHeight() const = 0;
			inline virtual unsigned int GetID() const = 0;
			inline virtual int NbChannels() const = 0;
			inline virtual bool IsTransparent() const = 0;

			virtual bool operator==(const Texture& other) const = 0;
	};

}