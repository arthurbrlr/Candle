#pragma once

#include "Candle/Renderer/Buffer.h"

namespace Candle {

	class CANDLE_API GLVertexBuffer : public VertexBuffer {

		public:
			GLVertexBuffer(unsigned int size);
			GLVertexBuffer(float* vertices, unsigned int size);
			~GLVertexBuffer();

			void Bind() override;
			void Unbind() override;

			void SetData(const void* data, unsigned int size) override;

			inline unsigned int GetID() { return _vboID; }

			inline void SetLayout(const BufferLayout & layout) override { _layout = layout; }
			virtual inline const BufferLayout & GetLayout() const override { return _layout; }

		private:
			unsigned int _vboID;
			BufferLayout _layout;

	};


	class CANDLE_API GLIndexBuffer : public IndexBuffer {
		
		public:
			GLIndexBuffer(unsigned int* indices, unsigned int count);
			~GLIndexBuffer();

			void Bind() override;
			void Unbind() override;

			inline unsigned int GetID() { return _eboID; }
			inline unsigned int GetIndexCount() override { return _indexCount; }

		private:	
			unsigned int _eboID;
			unsigned int _indexCount;
	};

}