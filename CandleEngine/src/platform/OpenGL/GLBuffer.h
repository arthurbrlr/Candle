#pragma once

#include "Candle/Renderer/Buffer.h"

namespace Candle {

	class CANDLE_API GLVertexBuffer : public VertexBuffer {

		public:
			GLVertexBuffer(uint32_t size);
			GLVertexBuffer(float* vertices, uint32_t size);
			~GLVertexBuffer();

			void Bind() override;
			void Unbind() override;

			void SetData(const void* data, uint32_t size) override;

			inline uint32_t GetID() { return _vboID; }

			inline void SetLayout(const BufferLayout & layout) override { _layout = layout; }
			virtual inline const BufferLayout & GetLayout() const override { return _layout; }

		private:
			uint32_t _vboID;
			BufferLayout _layout;

	};


	class CANDLE_API GLIndexBuffer : public IndexBuffer {
		
		public:
			GLIndexBuffer(uint32_t* indices, uint32_t count);
			~GLIndexBuffer();

			void Bind() override;
			void Unbind() override;

			inline uint32_t GetID() { return _eboID; }
			inline uint32_t GetIndexCount() override { return _indexCount; }

		private:	
			uint32_t _eboID;
			uint32_t _indexCount;
	};

}