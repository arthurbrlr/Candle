#include "cdlpch.h"
#include "GLContext.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Candle {

	
	GLContext::GLContext(GLFWwindow * window)
		: _windowHandle(window)
	{
	}

	
	void GLContext::Init()
	{
		/* Initialize the GLEW library in the current context */
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			CERROR("GLEW not initialized properly");

	}


	void GLContext::SwapBuffers()
	{
		glfwSwapBuffers(_windowHandle);
	}


	void GLContext::CleanUp()
	{

	}

}

