#pragma once

#include "Candle/CandleCore.h"
#include <vector>

namespace Candle {

	enum FrameBufferType {
		Texture,
		DepthTexture,
		DepthBuffer
	};

	class CANDLE_API FrameBuffer {

		public:
			virtual ~FrameBuffer() {}

			virtual void Bind() = 0;
			virtual void Unbind(unsigned int width, unsigned int height) = 0;
			virtual void Clean() = 0;
			
			virtual unsigned int GetID() = 0;
			virtual unsigned int Get(FrameBufferType type) = 0;

			static Shared<FrameBuffer> Create(const std::vector<FrameBufferType> & attachments, unsigned int width, unsigned int height);
	};

}