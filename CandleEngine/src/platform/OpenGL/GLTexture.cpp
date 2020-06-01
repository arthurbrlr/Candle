#include "cdlpch.h"
#include "GLTexture.h"

#include "stb_image.h"

namespace Candle {

	GLTexture2D::GLTexture2D(const std::string & filePath)
	{

		/* Texture binding */
		glGenTextures(1, &_textureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _textureID);

		/* Texture loading */
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(filePath.c_str(), &_width, &_height, &_nbChannels, 0);
		if (data) {
			if (_nbChannels == 4) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			} else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		} else {
			CERROR("{0} was not loaded properly", filePath);
			CASSERT(false, "Texture load assert");
		}
		stbi_image_free(data);

	}


	GLTexture2D::GLTexture2D(unsigned int width, unsigned int height)
		: _width(width), _height(height)
	{
		_nbChannels = 4;

		glGenTextures(1, &_textureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _textureID);

		glTextureStorage2D(_textureID, 1, GL_RGBA8, width, height);

		glTextureParameteri(_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(_textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(_textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}


	GLTexture2D::GLTexture2D(unsigned int id, unsigned int width, unsigned int height)
		: _textureID(id), _width(width), _height(height)
	{

	}


	void GLTexture2D::SetData(void* data, unsigned int size)
	{
		CASSERT(size == _width * _height * 4, "Data must fill the entire texture");
		glTextureSubImage2D(_textureID, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

}