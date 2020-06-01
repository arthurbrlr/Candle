#include "cdlpch.h"
#include "GLRendererAPI.h"

#include <GL/glew.h>

namespace Candle {

	void GLRendererAPI::Init()
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_BLEND);
		
		glEnable(GL_DEPTH_TEST);

		//glEnable(GL_PROGRAM_POINT_SIZE);
	}

	void GLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	void GLRendererAPI::SetLinesRendering(bool state)
	{
		if (state)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	void GLRendererAPI::SetDepthTesting(bool state)
	{
		if (state) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
	}


	void GLRendererAPI::SetAlphaBlending(bool state)
	{
		if (state) glEnable(GL_BLEND);
		else glDisable(GL_BLEND);
	}


	void GLRendererAPI::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		glViewport(x, y, width, height);
	}
	   

	void GLRendererAPI::SetClearColor(const glm::vec4 & color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}


	void GLRendererAPI::DrawArray(const Shared<VertexArray>& vertexArray, unsigned int indexCount)
	{
		unsigned int count = indexCount != -1 ? indexCount : vertexArray->GetIndexBuffer()->GetIndexCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}


	void GLRendererAPI::DrawLine(const Shared<VertexArray>& vertexArray, unsigned int indexCount)
	{
		unsigned int count = indexCount != -1 ? indexCount : vertexArray->GetIndexBuffer()->GetIndexCount();
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(5);
		glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, nullptr);
		glLineWidth(1);
		glDisable(GL_LINE_SMOOTH);
	}


	void GLRendererAPI::DrawPointArray(const Shared<VertexArray> & vertexArray, unsigned int indexCount)
	{
		unsigned int count = indexCount != -1 ? indexCount : vertexArray->GetIndexBuffer()->GetIndexCount();
		glDrawElements(GL_POINTS, count, GL_UNSIGNED_INT, nullptr);
	}

}