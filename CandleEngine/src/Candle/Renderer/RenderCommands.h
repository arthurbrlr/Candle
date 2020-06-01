#pragma once

#include "RendererAPI.h"

namespace Candle {

	class CANDLE_API RenderCommands {

		public:
			inline static void Init() { _rendererAPI->Init(); }
			inline static void Clear() { _rendererAPI->Clear(); }

			inline static void SetLinesRendering(bool state) { _rendererAPI->SetLinesRendering(state); }
			inline static void SetDepthTesting(bool state) { _rendererAPI->SetDepthTesting(state); }
			inline static void SetAlphaBlending(bool state) { _rendererAPI->SetAlphaBlending(state); }

			inline static void SetClearColor(const glm::vec4 & color) { _rendererAPI->SetClearColor(color); }
			inline static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) { _rendererAPI->SetViewport(x, y, width, height); }
			
			inline static void DrawArray(const Shared<VertexArray> & vertexArray, unsigned int indexCount = -1) { _rendererAPI->DrawArray(vertexArray, indexCount); }
			inline static void DrawLine(const Shared<VertexArray> & vertexArray, unsigned int indexCount = -1) { _rendererAPI->DrawLine(vertexArray, indexCount); }
			inline static void DrawPointArray(const Shared<VertexArray> & vertexArray, unsigned int indexCount = 0) { _rendererAPI->DrawPointArray(vertexArray, indexCount); }

		private:
			static RendererAPI* _rendererAPI;

	};

}
