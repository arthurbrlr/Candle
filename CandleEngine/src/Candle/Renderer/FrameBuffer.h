#pragma once

#include "Candle/CandleCore.h"
#include <vector>

namespace Candle {

	enum FrameBufferType {
		Texture,
		DepthTexture,
		DepthBuffer
	};

	class FrameBuffer {

		public:
			virtual ~FrameBuffer() {}

			virtual void Bind() = 0;
			virtual void Unbind(uint32_t width, uint32_t height) = 0;
			virtual void Clean() = 0;
			
			virtual uint32_t GetID() = 0;
			virtual uint32_t Get(FrameBufferType type) = 0;

			static Shared<FrameBuffer> Create(const std::vector<FrameBufferType> & attachments, uint32_t width, uint32_t height);
	};

}