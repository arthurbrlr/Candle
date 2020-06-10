#include "cdlpch.h"
#include "GLVertexArray.h"

#include <GL/glew.h>

namespace Candle {

	GLVertexArray::GLVertexArray()
	{
		glGenVertexArrays(1, &_vaoID);
		glBindVertexArray(_vaoID);
	}


	GLVertexArray::~GLVertexArray()
	{
		glDeleteVertexArrays(1, &_vaoID);
	}


	void GLVertexArray::Bind()
	{
		glBindVertexArray(_vaoID);
	}


	void GLVertexArray::Unbind()
	{
		glBindVertexArray(0);
	}


	void GLVertexArray::AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer)
	{
		CASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");

		glBindVertexArray(_vaoID);
		vertexBuffer->Bind();
		
		uint32_t index = 0;
		for (const BufferElement& element : vertexBuffer->GetLayout().GetElements()) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetCount(), GL_FLOAT, GL_FALSE, vertexBuffer->GetLayout().GetStride(), (void*)element.offset);
			index++;
		}
		
		_vertexBuffers.push_back(vertexBuffer);
	}


	void GLVertexArray::SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(_vaoID);
		indexBuffer->Bind();

		_indexBuffer = indexBuffer;
	}

}