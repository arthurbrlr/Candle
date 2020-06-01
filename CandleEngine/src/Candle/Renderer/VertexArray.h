#pragma once

#include <memory>

#include "Candle/CandleCore.h"
#include "Buffer.h"

namespace Candle {

	class CANDLE_API VertexArray {

		public:
			virtual ~VertexArray() {}

			virtual void Bind() = 0;
			virtual void Unbind() = 0;

			virtual void AddVertexBuffer(const Shared<VertexBuffer> & vertexBuffer) = 0;
			virtual void SetIndexBuffer(const Shared<IndexBuffer> & indexBuffer) = 0;

			virtual const std::vector<Shared<VertexBuffer>> & GetVertexBuffers() const = 0;
			virtual const Shared<IndexBuffer> & GetIndexBuffer() const = 0;

			static Shared<VertexArray> Create();
	};

}