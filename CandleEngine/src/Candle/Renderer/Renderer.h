#pragma once

#include "RenderCommands.h"
#include "RawCamera.h"
#include "Candle/Assets/Shader.h"
#include "Candle/Assets/Texture2D.h"


namespace Candle {

	class CANDLE_API Renderer {

		public:
			static void Init();
			static void Clear();

			static void BeginScene();
			static void EndScene();

			static void SubmitVAO(const Shared<Shader> & shader, const Shared<Texture2D> & texture, const Shared<VertexArray> & vertexArray, glm::mat4 transform = glm::mat4(1.0));
			static void SubmitVAO(const Shared<Shader> & shader, const Shared<VertexArray> & vertexArray, glm::mat4 transform = glm::mat4(1.0));

			inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};


}
