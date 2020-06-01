#include "cdlpch.h"
#include "GLBuffer.h"

#include <GL/glew.h>

namespace Candle {

	GLVertexBuffer::GLVertexBuffer(unsigned int size)
	{
		glGenBuffers(1, &_vboID);
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}


	GLVertexBuffer::GLVertexBuffer(float* vertices, unsigned int size)
	{
		glGenBuffers(1, &_vboID);
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}


	GLVertexBuffer::~GLVertexBuffer()
	{
		glDeleteBuffers(1, &_vboID);
	}
		

	void GLVertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	}


	void GLVertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	void GLVertexBuffer::SetData(const void* data, unsigned int size)
	{
		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}


	// ----

	GLIndexBuffer::GLIndexBuffer(unsigned int* indices, unsigned int count)
		: _indexCount(count)
	{
		glGenBuffers(1, &_eboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	}


	GLIndexBuffer::~GLIndexBuffer()
	{
		glDeleteBuffers(1, &_eboID);
	}


	void GLIndexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, _eboID);
	}


	void GLIndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}