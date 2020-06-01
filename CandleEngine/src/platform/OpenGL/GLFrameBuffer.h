#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Renderer/FrameBuffer.h"

namespace Candle {

	class GLFrameBuffer : public FrameBuffer {

		public:
			GLFrameBuffer(const std::vector<FrameBufferType> & attachments, unsigned int width, unsigned int height);
			~GLFrameBuffer();

			void Bind() override;
			void Unbind(unsigned int width, unsigned int height) override;
			void Clean() override;

			unsigned int GetID() override;
			unsigned int Get(FrameBufferType type) override { return _attachements[type]; }

		private:
			unsigned int _fboID;
			unsigned int _width, _height;
			std::unordered_map<FrameBufferType, unsigned int> _attachements;

			void CreateBuffer();
			void CreateTextureAttachment(unsigned int width, unsigned int height);
			void CreateDepthTextureAttachment(unsigned int width, unsigned int height);
			void CreateDepthBufferAttachement(unsigned int width, unsigned int height);

	};

}