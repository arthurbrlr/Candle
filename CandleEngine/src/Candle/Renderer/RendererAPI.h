#pragma once

#include <glm/glm.hpp>
#include "Candle/CandleCore.h"

#include "VertexArray.h"

namespace Candle {

	class CANDLE_API RendererAPI {
	
		public:
			enum class API {
				None = 0, OpenGL = 1
			};

			virtual ~RendererAPI() {}

			virtual void Init() = 0;
			virtual void Clear() = 0;

			virtual void SetLinesRendering(bool state) = 0;
			virtual void SetDepthTesting(bool state) = 0;
			virtual void SetAlphaBlending(bool state) = 0;

			virtual void SetClearColor(const glm::vec4 & color) = 0;
			virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
			
			virtual void DrawArray(const Shared<VertexArray> & vertexArray, uint32_t indexCount = 0) = 0;
			virtual void DrawLine(const Shared<VertexArray> & vertexArray, uint32_t indexCount = 0) = 0;
			virtual void DrawPointArray(const Shared<VertexArray> & vertexArray, uint32_t indexCount = 0) = 0;

			static inline API GetAPI() { return _API; }

		private:
			static API _API;
	
	};

}


