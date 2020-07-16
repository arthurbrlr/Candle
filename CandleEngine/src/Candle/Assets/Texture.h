#pragma once

#include "Candle/CandleCore.h"

namespace Candle {

	class Texture {

		public:
			virtual ~Texture() {}

			inline virtual void Bind(uint32_t slot = 0) const = 0;
			inline virtual void Unbind() const = 0;
			inline virtual void SetData(void* data, uint32_t size) = 0;

			inline virtual int GetWidth() const = 0;
			inline virtual int GetHeight() const = 0;
			inline virtual uint32_t GetID() const = 0;
			inline virtual int NbChannels() const = 0;
			inline virtual bool IsTransparent() const = 0;

			virtual bool operator==(const Texture& other) const = 0;
	};

}