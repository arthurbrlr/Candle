#pragma once

#include "Candle/Renderer/VertexArray.h"

namespace Candle {

	class GLVertexArray : public VertexArray {

		public:
			GLVertexArray();
			~GLVertexArray();

			void Bind() override;
			void Unbind() override;

			void AddVertexBuffer(const Shared<VertexBuffer> & vertexBuffer) override;
			void SetIndexBuffer(const Shared<IndexBuffer> & indexBuffer) override;

			const std::vector<Shared<VertexBuffer>> & GetVertexBuffers() const override { return _vertexBuffers; };
			const Shared<IndexBuffer> & GetIndexBuffer() const override { return _indexBuffer; }

			inline const unsigned int GetID() const { return _vaoID; }

		private:
			unsigned int _vaoID;
			std::vector<Shared<VertexBuffer> > _vertexBuffers;
			Shared<IndexBuffer> _indexBuffer;

	};

}

