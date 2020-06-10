#include "cdlpch.h"
#include "FrameBuffer.h"
#include "Renderer.h"

#include "platform/OpenGL/GLFrameBuffer.h"

namespace Candle {
	
	Shared<FrameBuffer> FrameBuffer::Create(const std::vector<FrameBufferType> & attachments, uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<GLFrameBuffer>(attachments, width, height);
			}
			case RendererAPI::API::None:
			{
				CASSERT(false, "No render api choosen");
				break;
			}
		}

		CASSERT(false, "No render api choosen");
		return nullptr;
	}

}