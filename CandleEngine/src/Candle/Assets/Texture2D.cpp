#include "cdlpch.h"
#include "Texture2D.h"

#include "Candle/Renderer/Renderer.h"
#include "platform/OpenGL/GLTexture.h"

namespace Candle {

	Unique<Texture2D> Texture2D::Create(const std::string & filePath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				return std::make_unique<GLTexture2D>(filePath);
			}
			case RendererAPI::API::None:
			{
				CASSERT(false, "No render api choosen!");
				break;
			}
		}

		CASSERT(false, "No render api choosen!");
		return nullptr;
	}


	Unique<Texture2D> Texture2D::Create(unsigned int width, unsigned int height, int channels)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				return std::make_unique<GLTexture2D>(width, height, channels);
			}
			case RendererAPI::API::None:
			{
				CASSERT(false, "No render api choosen!");
				break;
			}
			}

		CASSERT(false, "No render api choosen!");
		return nullptr;
	}


	Unique<Texture2D> Texture2D::Create(unsigned int id, unsigned int width, unsigned int height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				return std::make_unique<GLTexture2D>(id, width, height);
			}
			case RendererAPI::API::None:
			{
				CASSERT(false, "No render api choosen!");
				break;
			}
		}

		CASSERT(false, "No render api choosen!");
		return nullptr;
	}



		/* TexturePart */

	TexturePart::TexturePart(const Shared<Texture2D> & texture, const glm::vec2 & min, const glm::vec2 & max)
		: _texture(texture)
	{
		_textureCoords = { min.x, max.x, min.y, max.y };
	}


	Shared<TexturePart> TexturePart::CreateFromIndex(const Shared<Texture2D> & texture, const glm::vec2 & index, const glm::vec2 & tileSize, const glm::vec2 partSize)
	{
		glm::vec2 min = { (index.x * tileSize.x) / texture->GetWidth(), (index.y * tileSize.y) / texture->GetHeight() };
		glm::vec2 max = { ( (index.x + partSize.x) * tileSize.x) / texture->GetWidth(), ( (index.y + partSize.y) * tileSize.y) / texture->GetHeight() };

		return std::make_shared<TexturePart>(texture, min, max);
	}

}