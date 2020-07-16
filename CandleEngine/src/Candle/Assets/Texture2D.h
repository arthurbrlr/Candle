#pragma once

#include "Texture.h"

namespace Candle {

	class Texture2D : public Texture {

		public:
			static Unique<Texture2D> Create(const std::string & filePath);
			static Unique<Texture2D> Create(uint32_t width, uint32_t height, int channels = 4);
			static Unique<Texture2D> Create(uint32_t id, uint32_t width, uint32_t height);
	};


	class TexturePart {

		public:
			TexturePart(const Shared<Texture2D> & texture, const glm::vec2 & min, const glm::vec2 & max);
			static Shared<TexturePart> CreateFromIndex(const Shared<Texture2D> & texture, const glm::vec2 & index, const glm::vec2 & tileSize, const glm::vec2 partSize = { 1, 1 });

			const Shared<Texture2D> & GetTexture() { return _texture; }
			const glm::vec4 & GetTextureCoords() { return _textureCoords; }

		private:
			Shared<Texture2D> _texture;
			glm::vec4 _textureCoords;
	};

}