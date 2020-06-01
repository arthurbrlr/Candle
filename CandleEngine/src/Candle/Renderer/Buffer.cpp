#include "cdlpch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "platform/OpenGL/GLBuffer.h"

namespace Candle {


	Shared<VertexBuffer> VertexBuffer::Create(unsigned int size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<GLVertexBuffer>(size);
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


	Shared<VertexBuffer> VertexBuffer::Create(float* vertices, unsigned int size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<GLVertexBuffer>(vertices, size);
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


	Shared<IndexBuffer> IndexBuffer::Create(unsigned int* indices, unsigned int count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<GLIndexBuffer>(indices, count);
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


	void BufferLayout::CalculateOffsetsAndStride()
	{
		unsigned int off = 0;
		_stride = 0;

		for (auto& e : _elements) {

			e.offset = off;
			off += e.size;
			_stride += e.size;

		}
	}

}