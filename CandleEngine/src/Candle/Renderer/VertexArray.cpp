#include "cdlpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "platform/OpenGL/GLVertexArray.h"

namespace Candle {

	Shared<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<GLVertexArray>();
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


}