#pragma once

#include "Candle/CandleCore.h"

namespace Candle {

	enum class ShaderDataType {
		None = 0,
		Float,
		vec2f,
		vec3f,
		vec4f,
		mat3f,
		mat4f,
		Int,
		vec2i,
		vec3i,
		vec4i,
		Bool
	};

	
	static unsigned int ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case Candle::ShaderDataType::None:		return 0;
			case Candle::ShaderDataType::Float:		return 4;
			case Candle::ShaderDataType::vec2f:		return 2 * 4;
			case Candle::ShaderDataType::vec3f:		return 3 * 4;
			case Candle::ShaderDataType::vec4f:		return 4 * 4;
			case Candle::ShaderDataType::mat3f:		return 3 * 3 * 4;
			case Candle::ShaderDataType::mat4f:		return 4 * 4 * 4;
			case Candle::ShaderDataType::Int:		return 4;
			case Candle::ShaderDataType::vec2i:		return 2 * 4;
			case Candle::ShaderDataType::vec3i:		return 3 * 4;
			case Candle::ShaderDataType::vec4i:		return 4 * 4;
			case Candle::ShaderDataType::Bool:		return 1;
		}

		CASSERT(false, "Unknown shader data type");

		return 0;
	}


	struct BufferElement {
		std::string Name;
		ShaderDataType Type;
		unsigned int offset;
		unsigned int size;
		bool Normalized;

		BufferElement(const std::string & name, ShaderDataType type, bool normalized = false)
			: Name(name), Type(type), size(ShaderDataTypeSize(type)), offset(0), Normalized(normalized) {}

		unsigned int GetCount() const {
			switch (Type)
			{
				case Candle::ShaderDataType::None:		return 0;
				case Candle::ShaderDataType::Float:		return 1;
				case Candle::ShaderDataType::vec2f:		return 2;
				case Candle::ShaderDataType::vec3f:		return 3;
				case Candle::ShaderDataType::vec4f:		return 4;
				case Candle::ShaderDataType::mat3f:		return 3 * 3;
				case Candle::ShaderDataType::mat4f:		return 4 * 4;
				case Candle::ShaderDataType::Int:		return 1;
				case Candle::ShaderDataType::vec2i:		return 2;
				case Candle::ShaderDataType::vec3i:		return 3;
				case Candle::ShaderDataType::vec4i:		return 4;
				case Candle::ShaderDataType::Bool:		return 1;
			}

			return 0;
		}
	};


	class CANDLE_API BufferLayout {

		public:
			BufferLayout() {}
			BufferLayout(const std::initializer_list<BufferElement> & elements)
				: _elements(elements)
			{
				CalculateOffsetsAndStride();
			}

			inline const std::vector<BufferElement> & GetElements() const { return _elements; }

			std::vector<BufferElement>::iterator begin() { return _elements.begin(); }
			std::vector<BufferElement>::iterator end() { return _elements.end(); }

			inline const unsigned int GetStride() const { return _stride; }

		private:
			std::vector<BufferElement> _elements;
			unsigned int _stride = 0;

			void CalculateOffsetsAndStride();

	};

	class CANDLE_API VertexBuffer {

		public:
			virtual ~VertexBuffer () {}

			virtual void Bind() = 0;
			virtual void Unbind() = 0;

			virtual void SetData(const void* data, unsigned int size) = 0;

			virtual void SetLayout(const BufferLayout & layout) = 0;
			virtual inline const BufferLayout & GetLayout() const = 0;
			
			static Shared<VertexBuffer> Create(unsigned int size);
			static Shared<VertexBuffer> Create(float* vertices, unsigned int size);
	};


	class CANDLE_API IndexBuffer {

	public:
		virtual ~IndexBuffer () {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual inline unsigned int GetIndexCount() = 0;

		static Shared<IndexBuffer> Create(unsigned int* indices, unsigned int size);
	};


}