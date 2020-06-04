#pragma once

#include "Candle/CandleCore.h"

#include "Candle/Assets/AssetManager.h"
#include "Candle/Renderer/RendererLayers.h"

#include "Candle/ECS/Component.h"



namespace Candle {

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


	class CANDLE_API SpriteRenderer : public Component {

		public:
			SpriteRenderer(const Shared<Texture2D> & texture = nullptr, const glm::vec4 & textureCoord = { 0., 1., 0., 1. }, bool forceTransparency = false)
				: _texture(texture), _defaultTextureCoordinates(textureCoord)
			{
				_name = "SpriteRenderer";
				if ( texture != nullptr )
					_isTransparent = forceTransparency || texture->IsTransparent();
			}
							   

			void OnEditor() override
			{
				ImGui::Columns(2);
				ImGui::Text("Layer: ");
				ImGui::NextColumn();
				if(ImGui::BeginCombo("l", RendererLayers::GetLayers()[_rendererLayer].c_str()))
				{
					for ( auto& layer : RendererLayers::GetLayers() ) {
						const bool selected = _rendererLayer == layer.first;
						if ( ImGui::Selectable(layer.second.c_str(), selected) ) {
							_rendererLayer = layer.first;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::NextColumn();

				static int selected_anchor = _anchor;
				ImGui::Text("Anchor Point:");
				ImGui::NextColumn();
				ImGui::Combo("ap", &selected_anchor, spriteAnchor, IM_ARRAYSIZE(spriteAnchor));
				_anchor = (SpriteAnchor)selected_anchor;

				ImGui::NextColumn();
				ImGui::Text("Texture: ");
				ImGui::NextColumn();

					// TODO: complete edge cases for non textured quads
				if ( ImGui::BeginCombo("t", "none") ) {
					for ( auto& texture : AssetManager::GetAllTexture2D() ) {
						const bool selected = _texture == texture.second;
						if ( ImGui::Selectable(texture.first.c_str(), selected) ) {
							_texture = texture.second;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::Columns(1);

				ImGui::Text("Flip sprite: ");
				ImGui::Checkbox("x", &_flipX);
				ImGui::SameLine();
				ImGui::Checkbox("y", &_flipY);

				if (_texture != nullptr) {
					std::string menu = "Texture " + std::to_string(_texture->GetID());
					ImGui::Text(menu.c_str());
					ImGui::SameLine();
					ImGui::TextDisabled("(Show)");
					if (ImGui::IsItemHovered()) {
						ImGui::BeginTooltip();
						float textureAspectRatio = (float)_texture->GetWidth() / (float)_texture->GetHeight();
						ImGui::Image((void*)(intptr_t)_texture->GetID(), ImVec2(512, 512 / textureAspectRatio), ImVec2(0, 1), ImVec2(1, 0));
						ImGui::EndTooltip();
					}
				}
				ImGui::ColorEdit4("Color", glm::value_ptr(_colorTint));
				if (_colorTint.a != 1. || (_texture != nullptr && _texture->IsTransparent())) _isTransparent = true;
				else _isTransparent = false;
			}

				/* --- Setters --- */

			SpriteRenderer & SetTexture(const std::string & texture) 
			{
				_texture = AssetManager::GetTexture2D(texture); 
				if ( _texture != nullptr ) _isTransparent = _texture->IsTransparent();
				return *this;
			}
			SpriteRenderer & SetTextureCoordinates(const glm::vec4 & textCoordinates) { _defaultTextureCoordinates = textCoordinates; return *this; }

			SpriteRenderer & FlipX() { return FlipX(!_flipX); }
			SpriteRenderer & FlipY() { return FlipY(!_flipY); }
			SpriteRenderer & FlipX(bool state) {  _flipX = state; return *this; }
			SpriteRenderer & FlipY(bool state) { _flipY = state; return *this; }
			SpriteRenderer & Flip45() { _flip45 = !_flip45; return *this; }

			SpriteRenderer & SetColor(const glm::vec4 & color) { _colorTint = color; return *this; }
			SpriteRenderer & SetTileMultiplier(double tileMultiplier) { _tileMultiplier = tileMultiplier; return *this; }
			SpriteRenderer & SetAnchor(SpriteAnchor anchor) { _anchor = anchor; return *this; }

			SpriteRenderer & SetFixedInViewport(bool state) { _fixed = state; return *this; }

			
				/* --- Getters --- */
					
			inline const Shared<Texture2D> & GetTexture() const { return _texture; }
			inline const glm::vec4 GetTextureCoordinates() const 
			{
				glm::vec4 updatedTextCoord = _defaultTextureCoordinates;

				if (_flipX) {
					updatedTextCoord.x = _defaultTextureCoordinates.y;
					updatedTextCoord.y = _defaultTextureCoordinates.x;
				}

				if (_flipY) {
					updatedTextCoord.z = _defaultTextureCoordinates.w;
					updatedTextCoord.w = _defaultTextureCoordinates.z;
				}
				else {
					updatedTextCoord.z = _defaultTextureCoordinates.z;
					updatedTextCoord.w = _defaultTextureCoordinates.w;
				}

				glm::vec4 buffer = updatedTextCoord;

				if (_flip45) {
					updatedTextCoord.x = buffer.z;
					updatedTextCoord.y = buffer.w;
					updatedTextCoord.z = buffer.x;
					updatedTextCoord.w = buffer.y;
				}

				return updatedTextCoord;
			}
			inline const bool IsFlippedX() const { return _flipX; }
			inline const bool IsFlippedY() const { return _flipY; }

			inline const bool IsFixed() const { return _fixed; }
			inline const bool IsTransparent() const { return _isTransparent; }

			inline const glm::vec4 & GetColorTint() const { return _colorTint; }
			inline const double GetTileMultiplier() const { return _tileMultiplier; }
			inline const SpriteAnchor GetAnchor() const { return _anchor; }

			inline glm::vec4 GetOffsets() const { return GetOffsetsFromAnchor(_anchor); }

		private:
			Shared<Texture2D> _texture;
			glm::vec4 _defaultTextureCoordinates;
			unsigned int _rendererLayer = 0;

			bool _flipX = false, _flipY = false;
			bool _flip45 = false;

			double _tileMultiplier = 1.0;

			glm::vec4 _colorTint = {1, 1, 1, 1};
			SpriteAnchor _anchor = SpriteAnchor::Center;

			bool _fixed = false;
			bool _isTransparent = false;

				// Returns the quad (min_x, max_x, min_y, max_y) relative to the anchor position
			glm::vec4 GetOffsetsFromAnchor(SpriteAnchor anchor) const
			{
				switch (anchor)
				{
				case Candle::TopLeft:
					return glm::vec4(0., 1., -1., 0.);
					break;
				case Candle::TopCenter:
					return glm::vec4(-0.5, 0.5, -1., 0.);
					break;
				case Candle::TopRight:
					return glm::vec4(-1., 0., -1., 0.);
					break;
				case Candle::Left:
					return glm::vec4(0., 1., -0.5, 0.5);
					break;
				case Candle::Center:
					return glm::vec4(-0.5, 0.5, -0.5, 0.5);
					break;
				case Candle::Right:
					return glm::vec4(-1., 0., -0.5, 0.5);
					break;
				case Candle::BottomLeft:
					return glm::vec4(0., 1., 0., 1.);
					break;
				case Candle::BottomCenter:
					return glm::vec4(-0.5, 0.5, 0., 1.);
					break;
				case Candle::BottomRight:
					return glm::vec4(-1., 0., 0., 1.);
					break;
				}

				return glm::vec4(-0.5, 0.5, -0.5, 0.5);
			}
	};
}