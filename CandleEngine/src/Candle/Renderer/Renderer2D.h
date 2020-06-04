#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Application.h"
#include "Candle/ECS/Components/EngineComponents.h"
#include "VertexArray.h"
#include "RawCamera.h"

namespace Candle {

	struct QuadVertex {
		glm::vec3 position;
		glm::vec2 textureCoordinates;
		float textureIndex;
		glm::vec4 color;
		glm::vec4 parameters = { 1, 1, 1, 1 };
	};


	static BufferLayout defaultLayout = {
		{"position",			ShaderDataType::vec3f},
		{"textureCoordinates",  ShaderDataType::vec2f},
		{"textureIndex",		ShaderDataType::Float},
		{"color",				ShaderDataType::vec4f},
		{"parameters",			ShaderDataType::vec4f}
	};


	static BufferLayout fboLayout = {
		{"position",			ShaderDataType::vec3f},
		{"textureCoordinates",  ShaderDataType::vec2f}
	};


	struct Batch {
		const unsigned int maxQuad = 10000;
		const unsigned int maxQuadVertices = maxQuad * 4;
		const unsigned int maxIndices = maxQuad * 6;

		Shared<VertexArray> vertexArray;
		Shared<VertexBuffer> vertexBuffer;
		unsigned int indexCount = 0;
		QuadVertex* vbo;
		QuadVertex* vboptr;

		void Init()
		{
			vertexArray = VertexArray::Create();
			vertexArray->Bind();

			vertexBuffer = VertexBuffer::Create(maxQuadVertices * sizeof(QuadVertex));
			vertexBuffer->SetLayout(defaultLayout);
			vertexArray->AddVertexBuffer(vertexBuffer);

			vbo = new QuadVertex[maxQuadVertices];

			unsigned int* quadIndices = new unsigned int[maxIndices];
			unsigned int off = 0;
			for (unsigned int i = 0; i < maxIndices; i += 6) {
				quadIndices[i + 0] = off + 0;
				quadIndices[i + 1] = off + 1;
				quadIndices[i + 2] = off + 2;

				quadIndices[i + 3] = off + 2;
				quadIndices[i + 4] = off + 1;
				quadIndices[i + 5] = off + 3;

				off += 4;
			}

			Shared<IndexBuffer> ebo = IndexBuffer::Create(quadIndices, maxIndices);
			vertexArray->SetIndexBuffer(ebo);

			vboptr = vbo;
		}


		void UploadData()
		{
			unsigned int bufferSize = (unsigned int)((uint8_t*)vboptr - (uint8_t*)vbo);
			vertexBuffer->SetData(vbo, bufferSize);
		}


		void Reset()
		{
			vboptr = vbo;
			indexCount = 0;
		}
	};


	struct Renderer2DData {
		Shared<SpriteRenderer> defaultSpriteRenderer;
		Shared<SpriteRenderer> texturedSpriteRenderer;

			/* Full Screen Quad Related Stuff */
		float defaultVertices[20] = {
			-1.0, -1.0, 0.0, /**/ 0.0, 0.0,
			 1.0, -1.0, 0.0, /**/ 1.0, 0.0,
			-1.0,  1.0, 0.0, /**/ 0.0, 1.0,
			 1.0,  1.0, 0.0, /**/ 1.0, 1.0
		};


		unsigned int defaultIndices[6] = {
			0, 1, 2,
			2, 1, 3
		};

		Shared<VertexArray> fullscreenVertexArray;
		Shared<VertexBuffer> fullscreenVertexBuffer;
		const unsigned int fullscreenIndexCount = 6;


			/* Batch Abstraction Stuff */
		Batch opaqueBatch;
		Batch transparentBatch;


			/* Points Related stuff */


			/* Texture Binding */
		static const unsigned int maxTextures = 32;
		std::array<Shared<Texture2D>, maxTextures> textureBuffer;
		unsigned int textureBufferIndex = 1;


		void BeginScene()
		{
			textureBufferIndex = 1;

			opaqueBatch.Reset();
			transparentBatch.Reset();
		}


		void BindTextureBatch()
		{
			textureBuffer[0] = texturedSpriteRenderer->GetTexture();
		}
	};


	struct Renderer2DStatistics {

		unsigned int DrawCalls = 0;
		unsigned int OpaqueQuads = 0;
		unsigned int TransparentQuads = 0;
		unsigned int LineCount = 0;
		double ecsSpriteRenderTime = 0;

		unsigned int VertexCount() { return OpaqueQuads * 4 + TransparentQuads * 4; }
		unsigned int IndicesCount() { return OpaqueQuads * 6 + TransparentQuads * 6; }

		void Reset()
		{
			DrawCalls = 0;
			OpaqueQuads = 0;
			TransparentQuads = 0;
			LineCount = 0;
			ecsSpriteRenderTime = 0;
		}

	};


	class CANDLE_API Renderer2D {

		public:
			static void Init();
			static void Clear();

			static void BeginScene();
			static void EndScene();
			static void Flush();
			
			static Renderer2DStatistics* GetStats() { return _stats; }
			static Renderer2DData* GetData() { return _data; }

				// Clean this (temp)
			static void OnTextureShaderReload() { _data->BindTextureBatch(); }

				// -----------------------------
				// ----- Drawing Functions -----
				// -----------------------------

				// ---- Default Quads ----
			// Used to render simple quads on the screen

			static void DrawQuad(const glm::vec2 & position, const glm::vec2 & size);
			static void DrawQuad(const glm::vec3 & position, const glm::vec2 & size);

			static void DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const glm::vec4 & color);
			static void DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const glm::vec4 & color);

			static void DrawQuad(const glm::vec2 & position, const glm::vec2 & size, double rotation, const glm::vec4 & color);
			static void DrawQuad(const glm::vec3 & position, const glm::vec2 & size, double rotation, const glm::vec4 & color);


				// ---- Textured Quads ----
			// Used to render textured quads on the screen (full texture)

			static void DrawTexturedQuad(const glm::vec2 & position, const glm::vec2 & size, Shared<Texture2D> texture);
			static void DrawTexturedQuad(const glm::vec3 & position, const glm::vec2 & size, Shared<Texture2D> texture);

			static void DrawTexturedQuad(const glm::vec2 & position, const glm::vec2 & size, double rotation, Shared<Texture2D> texture);
			static void DrawTexturedQuad(const glm::vec3 & position, const glm::vec2 & size, double rotation, Shared<Texture2D> texture);


				// ---- Texture Parts Quad ----
			// Used to render texture parts on the screen (pars of a texture)

			static void DrawTexturePart(const glm::vec2 & position, const glm::vec2 & size, const Shared<TexturePart> & texturePart);
			static void DrawTexturePart(const glm::vec3 & position, const glm::vec2 & size, const Shared<TexturePart> & texturePart);

			static void DrawTexturePart(const glm::vec2 & position, const glm::vec2 & size, double rotation, const Shared<TexturePart> & texturePart);
			static void DrawTexturePart(const glm::vec3 & position, const glm::vec2 & size, double rotation, const Shared<TexturePart> & texturePart);

			
				// ---- Sprites ----
			// Used to render sprites on the screen
			// Sprites are the most complicated 2D elements (in terms of data describing them), so every 
			// functions that render a primitive type ultimately end up calling of these functions

			static void DrawSprite(const glm::vec2 & position, const glm::vec2 & size, const SpriteRenderer & sprite);
			static void DrawSprite(const glm::vec3 & position, const glm::vec2 & size, const SpriteRenderer & sprite);

			static void DrawSprite(const glm::vec2 & position, const glm::vec2 & size, double rotation, const SpriteRenderer & sprite);
			static void DrawSprite(const glm::vec3 & position, const glm::vec2 & size, double rotation, const SpriteRenderer & sprite);

			static void DrawSprite(const SpriteRenderer & sprite);
			static void DrawSprite(const SpriteRenderer & sprite, Transform & transform);


				// ---- Partial Sprites ----
			// Used to render sprites on the screen

			static void DrawPartialSprite(const Transform & transform, const Shared<Texture2D> & texture, const std::vector<SpritePart> & parts, bool fixed);

				/* TEMPORARY */
			static void DrawFrameBuffer(Shared<Texture2D> fboTexture, const std::string & shaderName = "CDL_SHADER_FSQuad");
			static void DrawFullScreenQuad();


				// ---- Lines ----
			static void DrawLine(const glm::vec3 & a, const glm::vec3 & b, double width);

		private:
			static struct SceneData {
				glm::mat4 pvmatrix;
				RawCamera * camera;
			}* _sceneData;

			static Renderer2DData* _data;
			static Renderer2DStatistics* _stats;

			static void RegisterQuadVertexInBatch(const QuadVertex & qv, QuadVertex* & batch);
			static void SanityCheck();
	};

}