#pragma once

#include "Candle/Renderer/RendererAPI.h"

namespace Candle {

	class GLRendererAPI : public RendererAPI {

		public:
			GLRendererAPI() {}

			void Init() override;
			void Clear() override;

			void SetLinesRendering(bool state) override;
			void SetDepthTesting(bool state) override;
			void SetAlphaBlending(bool state) override;

			void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
			void SetClearColor(const glm::vec4 & color) override;

			void DrawArray(const Shared<VertexArray> & vertexArray, uint32_t indexCount = -1) override;
			void DrawLine(const Shared<VertexArray>& vertexArray, uint32_t indexCount = -1) override;
			void DrawPointArray(const Shared<VertexArray> & vertexArray, uint32_t indexCount = -1) override;

		private:

	};


}

