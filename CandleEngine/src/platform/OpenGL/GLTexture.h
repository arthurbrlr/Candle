#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Assets/Texture2D.h"

#include <GL/glew.h>

namespace Candle {

	class CANDLE_API GLTexture2D : public Texture2D {

		public:	
			GLTexture2D(const std::string & filePath);
			GLTexture2D(unsigned int width, unsigned int height);
			GLTexture2D(unsigned int id, unsigned int width, unsigned int height);
			~GLTexture2D() { glDeleteTextures(1, &_textureID); }

			inline void Bind(unsigned int slot) const override { glBindTextureUnit(slot, _textureID); }
			inline void Unbind() const override { glBindTexture(GL_TEXTURE_2D, 0); }
			inline void SetData(void* data, unsigned int size) override;
			
			inline int GetWidth() const override { return _width; }
			inline int GetHeight() const override { return _height; }
			inline unsigned int GetID() const override { return _textureID; }
			inline int NbChannels() const override { return _nbChannels; }
			inline bool IsTransparent() const override { return _nbChannels == 4; }


			virtual bool operator==(const Texture& other) const override
			{
				if (&other == nullptr) return false;
				return _textureID == ((GLTexture2D&)other).GetID();
			}

		private:
			int _width, _height, _nbChannels = 0;
			unsigned int _textureID;
	};


}