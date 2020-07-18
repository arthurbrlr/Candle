#pragma once

#include "Candle/CandleCore.h"

#include "Candle/ECS/Component.h"
#include "Candle/Renderer/RendererLayers.h"


namespace Candle {

	class Texture2D;

	static const char* spriteAnchor[] = { "TopLeft",  "TopCenter", "TopRight", "Left", "Center", "Right",  "BottomLeft", "BottomCenter", "BottomRight" };

	enum SpriteAnchor {
		TopLeft, TopCenter, TopRight,
		Left, Center, Right,
		BottomLeft, BottomCenter, BottomRight
	};

	struct SpritePart {
		const char* name;
		glm::vec3 relativePosition = { 0, 0, 0 }; // Position relative to the local transform position
		glm::vec4 textureCoord = { 0, 1, 0, 1 }; // Part texture coordinates
		glm::vec2 baseSize = { 1, 1 }; // Default size of the element
		glm::vec2 relativeScale = { 1, 1 }; // Scale relative to the transform element
		SpriteAnchor anchor = SpriteAnchor::Center; 
	};

	typedef int SpriteRendererFlags;
	enum SpriteRendererFlags_ {
		SpriteRendererFlags_None			= 0,
		SpriteRendererFlags_FlipX			= BIT(1),
		SpriteRendererFlags_FlipY			= BIT(2),
		SpriteRendererFlags_Flip45			= BIT(3),
		SpriteRendererFlags_FixedInViewport	= BIT(4),
		SpriteRendererFlags_Transparent		= BIT(5)
	};


	class SpriteRenderer : public Component {

		public:
			SpriteRenderer(const Shared<Texture2D>& texture = nullptr); /* Constructor */
			SpriteRenderer(Blueprint* parent, const Shared<Texture2D>& texture = nullptr); /* Constructor */
			void OnEditor() override; /* Used by the engine editor to display informations about the component */

				/* --- Setters --- */

			SpriteRenderer& SetTexture(const std::string& texture);
			SpriteRenderer& SetTextureCoordinates(const glm::vec4& textCoordinates);
			SpriteRenderer& FlipX();
			SpriteRenderer& FlipY();
			SpriteRenderer& FlipX(bool state);
			SpriteRenderer& FlipY(bool state);
			SpriteRenderer& Flip45();
			SpriteRenderer& SetColor(const glm::vec4& color);
			SpriteRenderer& SetTileMultiplier(double tileMultiplier);
			SpriteRenderer& SetAnchor(SpriteAnchor anchor);
			SpriteRenderer& SetFixedInViewport(bool state);
			SpriteRenderer& SetFlags(SpriteRendererFlags newFlags);

			
				/* --- Getters --- */
					
			const Shared<Texture2D>& GetTexture() const;
			const glm::vec4 GetTextureCoordinates() const;
			const glm::vec4& GetColorTint() const;
			const double GetTileMultiplier() const;
			const SpriteAnchor GetAnchor() const;
			glm::vec4 GetOffsets() const;
			SpriteRendererFlags GetFlags() const;

			const bool IsFlippedX() const;
			const bool IsFlippedY() const;
			const bool IsFixed() const;
			const bool IsTransparent() const;


		private:
			Shared<Texture2D> _texture;
			glm::vec4 _defaultTextureCoordinates = { 0, 1, 0, 1 };
			uint32_t _rendererLayer = 0;
			SpriteRendererFlags _flags = SpriteRendererFlags_None;

			double _tileMultiplier = 1.0;

			glm::vec4 _colorTint = {1, 1, 1, 1};
			SpriteAnchor _anchor = SpriteAnchor::Center;

				// Returns the quad (min_x, max_x, min_y, max_y) relative to the anchor position
			glm::vec4 GetOffsetsFromAnchor(SpriteAnchor anchor) const;
			bool CheckFlag(SpriteRendererFlags_ flag) const;
	};
}