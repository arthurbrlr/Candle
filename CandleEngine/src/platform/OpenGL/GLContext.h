#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Renderer/GraphicsContext.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Candle {

	class CANDLE_API GLContext : public GraphicsContext {

		public:
			GLContext(GLFWwindow* window);

			void Init() override;
			void SwapBuffers() override;
			void CleanUp() override;

			const unsigned char* GetContextVersion() override { return glGetString(GL_VERSION); }
			const unsigned char* GetHardwareVendor() override { return glGetString(GL_VENDOR); }
			const unsigned char* GetRenderer() override { return glGetString(GL_RENDERER); }

		private:
			GLFWwindow* _windowHandle;

	};

}

