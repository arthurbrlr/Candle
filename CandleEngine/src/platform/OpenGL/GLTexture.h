#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Assets/Texture2D.h"

#include <GL/glew.h>

namespace Candle {

	class CANDLE_API GLTexture2D : public Texture2D {

		public:	
			GLTexture2D(const std::string & filePath);
			GLTexture2D(uint32_t width, uint32_t height, int channels);
			GLTexture2D(uint32_t id, uint32_t width, uint32_t height);
			~GLTexture2D() { glDeleteTextures(1, &_textureID); }

			inline void Bind(uint32_t slot) const override { glBindTextureUnit(slot, _textureID); }
			inline void Unbind() const override { glBindTexture(GL_TEXTURE_2D, 0); }
			inline void SetData(void* data, uint32_t size) override;
			
			inline int GetWidth() const override { return _width; }
			inline int GetHeight() const override { return _height; }
			inline uint32_t GetID() const override { return _textureID; }
			inline int NbChannels() const override { return _nbChannels; }
			inline bool IsTransparent() const override { return _nbChannels == 4; }


			virtual bool operator==(const Texture& other) const override
			{
				if (&other == nullptr) return false;
				return _textureID == ((GLTexture2D&)other).GetID();
			}

		private:
			int _width, _height, _nbChannels = 0;
			uint32_t _textureID;
	};


}