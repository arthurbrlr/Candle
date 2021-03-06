#include "cdlpch.h"
#include "Renderer2D.h"

#include "Candle/Core/Time.h"
#include "RenderCommands.h"
#include "CameraManagement.h"

namespace Candle {

	Renderer2D::SceneData* Renderer2D::_sceneData = new Renderer2D::SceneData();
	Renderer2DData* Renderer2D::_data = new Renderer2DData();
	Renderer2DStatistics* Renderer2D::_stats = new Renderer2DStatistics();

	void Renderer2D::Init()
	{
		_data->defaultSpriteRenderer = std::make_shared<SpriteRenderer>(Assets::GetTexture2D("CDL_TEXTURE_DEFAULT"));
		_data->texturedSpriteRenderer = std::make_shared<SpriteRenderer>(Assets::GetTexture2D("CDL_TEXTURE_WHITE"));

			/* Full Screen Init */
		_data->fullscreenVertexArray = VertexArray::Create();
		_data->fullscreenVertexBuffer = VertexBuffer::Create(_data->defaultVertices, 20 * sizeof(float));
		_data->fullscreenVertexBuffer->SetLayout(fboLayout);
		_data->fullscreenVertexArray->AddVertexBuffer(_data->fullscreenVertexBuffer);

		Shared<IndexBuffer> indexBuffer = IndexBuffer::Create(_data->defaultIndices, 6 * sizeof(uint32_t));
		_data->fullscreenVertexArray->SetIndexBuffer(indexBuffer);


			/* Batch Abstraction */
		_data->opaqueBatch.Init();
		_data->transparentBatch.Init();

		/* ---- */

		_data->BindTextureBatch();
	}


	void Renderer2D::Clear()
	{
		delete[] _data->opaqueBatch.vbo;
		delete[] _data->transparentBatch.vbo;
	}

	void Renderer2D::BeginScene()
	{
		Assets::BindShader("CDL_SHADER_TEXTURE");
		Assets::GetShader("CDL_SHADER_TEXTURE")->SetMat4("u_projectionViewMatrix", CameraManagement::GetViewProjection());

		_data->BeginScene();
	}


	void Renderer2D::EndScene()
	{
				// [NOTE]
			// BeginScene 
			//	-> for each layer
			//		BeginLayer / EndLayer
			// End Scene

		Flush();
	}


	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < _data->textureBufferIndex; i++) {
			_data->textureBuffer[i]->Bind(i);
		}

			/* Opaque Batch Rendering */
		if (_data->opaqueBatch.indexCount > 0) {
			_data->opaqueBatch.UploadData();
			_data->opaqueBatch.vertexArray->Bind();
			RenderCommands::DrawArray(_data->opaqueBatch.vertexArray, _data->opaqueBatch.indexCount);
			_stats->DrawCalls++;
		}
		

			/* Transparent Batch Rendering */
		if (_data->transparentBatch.indexCount > 0) {
			RenderCommands::SetAlphaBlending(true);
			// Sort transparent quads
			
			auto compareQuadVertexPosition = [](const QuadVertex& q1, const QuadVertex& q2) -> bool {
				return q1.position.z < q2.position.z;
			};
			//std::stable_sort(_data->transparentBatch.vbo, _data->transparentBatch.vboptr, compareQuadVertexPosition);

			_data->transparentBatch.UploadData();
			_data->transparentBatch.vertexArray->Bind();
			RenderCommands::DrawArray(_data->transparentBatch.vertexArray, _data->transparentBatch.indexCount);
			RenderCommands::SetAlphaBlending(false);
			_stats->DrawCalls++;
		}


		// Need batch for UI (depthTesting = false) 

		/*
		for (uint32_t i = 0; i < _data->textureBufferIndex; i++) {
			_data->textureBuffer[i]->Unbind();
		}
		*/
	}

		// Need to be fixed 
	void Renderer2D::RegisterQuadVertexInBatch(const QuadVertex & qv, QuadVertex* & batch)
	{
		batch->position = qv.position;
		batch->textureCoordinates = qv.textureCoordinates;
		batch->textureIndex = (float)qv.textureIndex;
		batch->color = qv.color;
		batch->parameters = qv.parameters;
		batch++;
	}


	void Renderer2D::SanityCheck()
	{
		if (_data->opaqueBatch.indexCount >= _data->opaqueBatch.maxIndices) {
			EndScene();
			_data->opaqueBatch.vboptr = _data->opaqueBatch.vbo;
			_data->opaqueBatch.indexCount = 0;
		}

		if (_data->transparentBatch.indexCount >= _data->transparentBatch.maxIndices) {
			EndScene();
			_data->transparentBatch.vboptr = _data->transparentBatch.vbo;
			_data->transparentBatch.indexCount = 0;
		}
	}


		// ---- Default Quads ----
	// Used to render simple quads on the screen

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size)
	{
		DrawSprite({ position.x, position.y, 0.f }, size, 0, *_data->defaultSpriteRenderer);
	}


	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size)
	{
		DrawSprite(position, size, 0, *_data->defaultSpriteRenderer);
	}


	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const glm::vec4 & color)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, 0, color);
	}


	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const glm::vec4 & color)
	{
		_data->texturedSpriteRenderer->SetColor(color);
		DrawSprite(position, size, 0, *_data->texturedSpriteRenderer);
	}


	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, double rotation, const glm::vec4 & color)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, rotation, color);
	}


	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, double rotation, const glm::vec4 & color)
	{
		_data->texturedSpriteRenderer->SetColor(color);
		DrawSprite(position, size, rotation, *_data->texturedSpriteRenderer);
	}



		// ---- Textured Quads ----
	// Used to render textured quads on the screen (full texture)

	void Renderer2D::DrawTexturedQuad(const glm::vec2 & position, const glm::vec2 & size, Shared<Texture2D> texture)
	{
		DrawTexturedQuad({ position.x, position.y, 0.f }, size, 0, texture);
	}


	void Renderer2D::DrawTexturedQuad(const glm::vec3 & position, const glm::vec2 & size, Shared<Texture2D> texture)
	{
		DrawTexturedQuad(position, size, 0, texture);
	}


	void Renderer2D::DrawTexturedQuad(const glm::vec2 & position, const glm::vec2 & size, double rotation, Shared<Texture2D> texture)
	{
		DrawTexturedQuad({ position.x, position.y, 0. }, size, rotation, texture);
	}


	void Renderer2D::DrawTexturedQuad(const glm::vec3 & position, const glm::vec2 & size, double rotation, Shared<Texture2D> texture)
	{
		SpriteRenderer spriteRenderer(texture);
		DrawSprite(position, size, rotation, spriteRenderer);
	}


		// ---- Texture Parts Quad ----
	// Used to render texture parts on the screen (pars of a texture)

	void Renderer2D::DrawTexturePart(const glm::vec2 & position, const glm::vec2 & size, const Shared<TexturePart> & texturePart)
	{
		DrawTexturePart({ position.x, position.y, 0.0 }, size, 0, texturePart);
	}


	void Renderer2D::DrawTexturePart(const glm::vec3 & position, const glm::vec2 & size, const Shared<TexturePart> & texturePart)
	{
		DrawTexturePart(position, size, 0, texturePart);
	}


	void Renderer2D::DrawTexturePart(const glm::vec2 & position, const glm::vec2 & size, double rotation, const Shared<TexturePart> & texturePart)
	{
		DrawTexturePart({ position.x, position.y, 0.0 }, size, rotation, texturePart);
	}


	void Renderer2D::DrawTexturePart(const glm::vec3 & position, const glm::vec2 & size, double rotation, const Shared<TexturePart> & texturePart)
	{
		glm::vec4 textureCoords = texturePart->GetTextureCoords();
		Shared<Texture2D> texture = texturePart->GetTexture();
		SpriteRenderer spriteRenderer(texture);
		spriteRenderer.SetTextureCoordinates(textureCoords);
		DrawSprite(position, size, rotation, spriteRenderer);
	}



		// ---- Sprites ----
	// Used to render sprites on the screen
	// Sprites are the most complicated 2D elements (in terms of data describing them), so every 
	// functions that render a primitive type ultimately end up calling of these functions

	void Renderer2D::DrawSprite(const glm::vec2 & position, const glm::vec2 & size, const SpriteRenderer & sprite)
	{
		DrawSprite({ position.x, position.y, 0.f }, size, 0., sprite);
	}


	void Renderer2D::DrawSprite(const glm::vec3 & position, const glm::vec2 & size, const SpriteRenderer & sprite)
	{
		DrawSprite(position, size, 0., sprite);
	}


	void Renderer2D::DrawSprite(const glm::vec2 & position, const glm::vec2 & size, double rotation, const SpriteRenderer & sprite)
	{
		DrawSprite({ position.x, position.y, 0. }, size, rotation, sprite);
	}


	void Renderer2D::DrawSprite(const glm::vec3 & position, const glm::vec2 & size, double rotation, const SpriteRenderer & sprite)
	{
		SanityCheck();

		float textureIndex = 0.f;
		bool transparent = sprite.IsTransparent();
		QuadVertex* & targetBatch = transparent ? _data->transparentBatch.vboptr : _data->opaqueBatch.vboptr;

		for (uint32_t i = 0; i < _data->textureBufferIndex; i++) {
			if (_data->textureBuffer[i]->GetID() == sprite.GetTexture()->GetID()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (sprite.GetTexture() == nullptr) {
			textureIndex = 0.f;
		} else if (textureIndex == 0.f) {
			textureIndex = (float)_data->textureBufferIndex;
			_data->textureBuffer[_data->textureBufferIndex] = sprite.GetTexture();
			_data->textureBufferIndex++;
		}

		glm::vec4 offsets = sprite.GetOffsets();
		double tileMultiplier = sprite.GetTileMultiplier();
		glm::vec4 textureCoordinates = sprite.GetTextureCoordinates();
		glm::vec4 color = sprite.GetColorTint();
		glm::vec4 parameters = { sprite.IsFixed() ? 1 : 0, 0, 0, 0 };

		glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0), position)
								  * glm::rotate(glm::mat4(1.0), (float)glm::radians(rotation), { 0., 0., 1. });

		glm::vec4 bottomLeftVertex = transformMatrix * glm::vec4({ size.x * offsets.x, size.y * offsets.z, 0., 1. });
		glm::vec4 bottomRightVertex = transformMatrix * glm::vec4({ size.x * offsets.y, size.y * offsets.z, 0., 1. });
		glm::vec4 topLeftVertex = transformMatrix * glm::vec4({ size.x * offsets.x, size.y * offsets.w, 0., 1. });
		glm::vec4 topRightVertex = transformMatrix * glm::vec4({ size.x * offsets.y, size.y * offsets.w, 0., 1. });


		RegisterQuadVertexInBatch({
			bottomLeftVertex,
			{ textureCoordinates.x, textureCoordinates.z },
			textureIndex,
			color,
			parameters
		}, targetBatch);

		RegisterQuadVertexInBatch({
			bottomRightVertex,
			{ textureCoordinates.y, textureCoordinates.z },
			textureIndex,
			color,
			parameters
		}, targetBatch);

		RegisterQuadVertexInBatch({
			topLeftVertex,
			{ textureCoordinates.x, textureCoordinates.w },
			textureIndex,
			color,
			parameters
		}, targetBatch);

		RegisterQuadVertexInBatch({
			topRightVertex,
			{ textureCoordinates.y, textureCoordinates.w },
			textureIndex,
			color,
			parameters
		}, targetBatch);

			// Actual indexing for rendering
		if (transparent) _data->transparentBatch.indexCount += 6;
		else _data->opaqueBatch.indexCount += 6;

			// Stats
		if (transparent) _stats->TransparentQuads += 1;
		else _stats->OpaqueQuads += 1;
	}


	void Renderer2D::DrawSprite(const SpriteRenderer & sprite)
	{
		Transform transform;
		DrawSprite(sprite, transform);
	}


	void Renderer2D::DrawSprite(const SpriteRenderer & sprite, Transform & transform)
	{
		SanityCheck();

		float textureIndex = 0.f;
		bool transparent = sprite.IsTransparent();
		QuadVertex* & targetBatch = transparent ? _data->transparentBatch.vboptr : _data->opaqueBatch.vboptr;

		for (uint32_t i = 0; i < _data->textureBufferIndex; i++) {
			if (*_data->textureBuffer[i].get() == *sprite.GetTexture().get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (sprite.GetTexture() == nullptr) {
			textureIndex = 0.f;
		}
		else if (textureIndex == 0.f) {
			textureIndex = (float)_data->textureBufferIndex;
			_data->textureBuffer[_data->textureBufferIndex] = sprite.GetTexture();
			_data->textureBufferIndex++;
		}

		glm::vec4 offsets = sprite.GetOffsets();
		double tileMultiplier = sprite.GetTileMultiplier();
		glm::vec4 textureCoordinates = sprite.GetTextureCoordinates();
		glm::vec4 color = sprite.GetColorTint();
		glm::vec4 parameters = { sprite.IsFixed() ? 1 : 0, 0, 0, 0 };

		glm::mat4 transformMatrix = transform.Get();
		glm::vec4 bottomLeftVertex = transformMatrix * glm::vec4({ offsets.x, offsets.z, 0., 1. });
		glm::vec4 bottomRightVertex = transformMatrix * glm::vec4({ offsets.y, offsets.z, 0., 1. });
		glm::vec4 topLeftVertex = transformMatrix * glm::vec4({ offsets.x, offsets.w, 0., 1. });
		glm::vec4 topRightVertex = transformMatrix * glm::vec4({ offsets.y, offsets.w, 0., 1. });


		RegisterQuadVertexInBatch({
			bottomLeftVertex,
			{ textureCoordinates.x, textureCoordinates.z },
			textureIndex,
			color,
			parameters
		}, targetBatch);

		RegisterQuadVertexInBatch({
			bottomRightVertex,
			{ textureCoordinates.y, textureCoordinates.z },
			textureIndex,
			color,
			parameters
		}, targetBatch);

		RegisterQuadVertexInBatch({
			topLeftVertex,
			{ textureCoordinates.x, textureCoordinates.w },
			textureIndex,
			color,
			parameters
		}, targetBatch);

		RegisterQuadVertexInBatch({
			topRightVertex,
			{ textureCoordinates.y, textureCoordinates.w },
			textureIndex,
			color,
			parameters
		}, targetBatch);

			// Actual indexing for rendering
		if (transparent) _data->transparentBatch.indexCount += 6;
		else _data->opaqueBatch.indexCount += 6;

			// Stats
		if (transparent) _stats->TransparentQuads += 1;
		else _stats->OpaqueQuads += 1;
	}


	
		// ---- Partial Sprites ----
	// Used to render sprites on the screen

	void Renderer2D::DrawPartialSprite(const Transform & transform, const Shared<Texture2D> & texture, const std::vector<SpritePart> & parts, bool fixed)
	{
		Shared<SpriteRenderer> partSprite = std::make_shared<SpriteRenderer>(texture);
		partSprite->SetFixedInViewport(fixed);
		Shared<Transform> partTransform = std::make_shared<Transform>();

		for (int i = 0; i < parts.size(); i++) {

			partSprite->SetTextureCoordinates(parts[i].textureCoord).SetAnchor(parts[i].anchor);

			glm::vec3 partPosition = {
				transform.GetPosition().x + parts[i].relativePosition.x * transform.GetScale().x,
				transform.GetPosition().y + parts[i].relativePosition.y * transform.GetScale().y,
				transform.GetPosition().z + parts[i].relativePosition.z * transform.GetScale().z
			};

			glm::vec3 partScale = {
				transform.GetScale().x * parts[i].relativeScale.x + parts[i].baseSize.x,
				transform.GetScale().y * parts[i].relativeScale.y + parts[i].baseSize.y,
				1.
			};

			partTransform->SetPosition(partPosition);
			partTransform->SetScale(partScale);

			DrawSprite(*partSprite, *partTransform);
		}

	}


		/* TEMPORARY */
	void Renderer2D::DrawFrameBuffer(Shared<Texture2D> fboTexture, const std::string & shaderName)
	{

		Shared<Shader> shader = Assets::GetShader(shaderName);
		shader->Bind();
		shader->SetFloat("u_time", (float)Time());
		
		fboTexture->Bind();

		float vertices[20]{ 0 };
		for (int i = 0; i < 20; i++) vertices[i] = _data->defaultVertices[i];

		_data->fullscreenVertexArray->Bind();
		_data->fullscreenVertexBuffer->SetData(vertices, 20 * sizeof(float));
		RenderCommands::DrawArray(_data->fullscreenVertexArray, _data->fullscreenIndexCount);
	}


	void Renderer2D::DrawFullScreenQuad()
	{
		float vertices[20]{ 0 };
		for (int i = 0; i < 20; i++) vertices[i] = _data->defaultVertices[i];

		//Assets::GetShader("defaultFBO")->Bind();

		_data->fullscreenVertexArray->Bind();
		_data->fullscreenVertexBuffer->SetData(vertices, 20 * sizeof(float));
		RenderCommands::DrawArray(_data->fullscreenVertexArray, _data->fullscreenIndexCount);
	}



		/*********/
		/* LINES */
		/*********/


	void Renderer2D::DrawLine(const glm::vec3& a, const glm::vec3& b, double thickness)
	{

	}
}