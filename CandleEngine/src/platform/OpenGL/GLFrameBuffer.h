#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Renderer/FrameBuffer.h"

namespace Candle {

	class GLFrameBuffer : public FrameBuffer {

		public:
			GLFrameBuffer(const std::vector<FrameBufferType> & attachments, uint32_t width, uint32_t height);
			~GLFrameBuffer();

			void Bind() override;
			void Unbind(uint32_t width, uint32_t height) override;
			void Clean() override;

			uint32_t GetID() override;
			uint32_t Get(FrameBufferType type) override { return _attachements[type]; }

		private:
			uint32_t _fboID;
			uint32_t _width, _height;
			std::unordered_map<FrameBufferType, uint32_t> _attachements;

			void CreateBuffer();
			void CreateTextureAttachment(uint32_t width, uint32_t height);
			void CreateDepthTextureAttachment(uint32_t width, uint32_t height);
			void CreateDepthBufferAttachement(uint32_t width, uint32_t height);

	};

}