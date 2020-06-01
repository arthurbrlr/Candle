#pragma once

#include "Candle/Renderer/RendererAPI.h"

namespace Candle {

	class CANDLE_API GLRendererAPI : public RendererAPI {

		public:
			GLRendererAPI() {}

			void Init() override;
			void Clear() override;

			void SetLinesRendering(bool state) override;
			void SetDepthTesting(bool state) override;
			void SetAlphaBlending(bool state) override;

			void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
			void SetClearColor(const glm::vec4 & color) override;

			void DrawArray(const Shared<VertexArray> & vertexArray, unsigned int indexCount = -1) override;
			void DrawLine(const Shared<VertexArray>& vertexArray, unsigned int indexCount = -1) override;
			void DrawPointArray(const Shared<VertexArray> & vertexArray, unsigned int indexCount = -1) override;

		private:

	};


}

