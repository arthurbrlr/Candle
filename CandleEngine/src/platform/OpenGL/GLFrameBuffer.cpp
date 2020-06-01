#include "cdlpch.h"
#include "GLFrameBuffer.h"

#include <GL/glew.h>

namespace Candle {


	GLFrameBuffer::GLFrameBuffer(const std::vector<FrameBufferType> & attachments, unsigned int width, unsigned int height)
		: _width(width), _height(height)
	{

		CreateBuffer();

		for (auto& fba : attachments) {

			switch (fba)
			{
				case FrameBufferType::Texture:
					CreateTextureAttachment(width, height);
					break;

				case FrameBufferType::DepthTexture:
					CreateDepthTextureAttachment(width, height);
					break;

				case FrameBufferType::DepthBuffer:	
					CreateDepthBufferAttachement(width, height);
					break;
			}

		}

	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		Clean();
	}


	void GLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
		glViewport(0, 0, _width, _height);
	}


	void GLFrameBuffer::Unbind(unsigned int width, unsigned int height)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
	}



	void GLFrameBuffer::Clean()
	{
		glDeleteFramebuffers(1, &_fboID);

		for (auto& it : _attachements) {
			switch (it.first)
			{
				case FrameBufferType::Texture:
					glDeleteTextures(1, &it.second);
					break;

				case FrameBufferType::DepthTexture:
					glDeleteTextures(1, &it.second);
					break;

				case FrameBufferType::DepthBuffer:	
					glDeleteRenderbuffers(1, &it.second);
					break;
			}
		}
	}


	unsigned int GLFrameBuffer::GetID()
	{
		return _fboID;
	}


	void GLFrameBuffer::CreateBuffer()
	{
		glGenFramebuffers(1, &_fboID);
		glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
	}


	void GLFrameBuffer::CreateTextureAttachment(unsigned int width, unsigned int height)
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

		_attachements[FrameBufferType::Texture] = texture;
	}


	void GLFrameBuffer::CreateDepthTextureAttachment(unsigned int width, unsigned int height)
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);

		_attachements[FrameBufferType::DepthTexture] = texture;
	}


	void GLFrameBuffer::CreateDepthBufferAttachement(unsigned int width, unsigned int height)
	{
		unsigned int depthBuffer;
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		_attachements[FrameBufferType::DepthBuffer] = depthBuffer;
	}
}