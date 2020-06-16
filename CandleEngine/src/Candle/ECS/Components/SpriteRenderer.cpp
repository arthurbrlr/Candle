#include "cdlpch.h"
#include "SpriteRenderer.h"

#include "Candle/Assets/AssetManager.h"


namespace Candle {

	SpriteRenderer::SpriteRenderer(const Shared<Texture2D>& texture)
		: _texture(texture)
	{
		_name = "SpriteRenderer";
		if ( _texture != nullptr && _texture->IsTransparent() ) {
			_flags |= SpriteRendererFlags_Transparent;
		} else {
			_flags &= ~SpriteRendererFlags_Transparent;
		}
	}


	void SpriteRenderer::OnEditor()
	{
		ImGui::Columns(2);
		ImGui::Text("Layer: ");
		ImGui::NextColumn();
		if ( ImGui::BeginCombo("l", RendererLayers::GetLayers()[_rendererLayer].c_str()) ) {
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
		static bool flipX;
		flipX = _flags & SpriteRendererFlags_FlipX;
		ImGui::Checkbox("x", &flipX);
		if ( flipX ) {
			_flags |= SpriteRendererFlags_FlipX;
		} else {
			_flags &= ~SpriteRendererFlags_FlipX;
		}

		ImGui::SameLine();
		static bool flipY;
		flipY = _flags & SpriteRendererFlags_FlipY;
		ImGui::Checkbox("y", &flipY);
		if ( flipY ) {
			_flags |= SpriteRendererFlags_FlipY;
		} else {
			_flags &= ~SpriteRendererFlags_FlipY;
		}

		ImGui::SameLine();
		static bool flip45;
		flip45 = _flags & SpriteRendererFlags_Flip45;
		ImGui::Checkbox("45", &flip45);
		if ( flip45 ) {
			_flags |= SpriteRendererFlags_Flip45;
		} else {
			_flags &= ~SpriteRendererFlags_Flip45;
		}

		ImGui::SameLine();
		static bool fixed;
		fixed = _flags & SpriteRendererFlags_FixedInViewport;
		ImGui::Checkbox("Fixed", &fixed);
		if ( fixed ) {
			_flags |= SpriteRendererFlags_FixedInViewport;
		} else {
			_flags &= ~SpriteRendererFlags_FixedInViewport;
		}

		if ( _texture != nullptr ) {
			std::string menu = "Texture " + std::to_string(_texture->GetID());
			ImGui::Text(menu.c_str());
			ImGui::SameLine();
			ImGui::TextDisabled("(Show)");
			if ( ImGui::IsItemHovered() ) {
				ImGui::BeginTooltip();
				float textureAspectRatio = (float)_texture->GetWidth() / (float)_texture->GetHeight();
				ImGui::Image((void*)(intptr_t)_texture->GetID(), ImVec2(512, 512 / textureAspectRatio), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::EndTooltip();
			}
		}
		ImGui::ColorEdit4("Color", glm::value_ptr(_colorTint));
		if ( _colorTint.a != 1. || ( _texture != nullptr && _texture->IsTransparent() ) ) _flags |= SpriteRendererFlags_Transparent;
		else _flags &= ~SpriteRendererFlags_Transparent;
	}


		/* Setters */

	SpriteRenderer& SpriteRenderer::SetTexture(const std::string& texture)
	{
		_texture = AssetManager::GetTexture2D(texture);
		if ( _texture != nullptr && _texture->IsTransparent() ) {
			_flags |= SpriteRendererFlags_Transparent;
		} else {
			_flags &= ~SpriteRendererFlags_Transparent;
		}
		return *this;
	}

	SpriteRenderer& SpriteRenderer::SetTextureCoordinates(const glm::vec4& textCoordinates) 
	{
		_defaultTextureCoordinates = textCoordinates; 
		return *this; 
	}

	SpriteRenderer& SpriteRenderer::FlipX() 
	{
		return FlipX(!CheckFlag(SpriteRendererFlags_FlipX));
	}

	SpriteRenderer& SpriteRenderer::FlipY() 
	{ 
		return FlipX(!CheckFlag(SpriteRendererFlags_FlipY));
	}

	SpriteRenderer& SpriteRenderer::FlipX(bool state) 
	{
		if ( state ) {
			_flags |= SpriteRendererFlags_FlipX;
		} else {
			_flags &= ~SpriteRendererFlags_FlipX;
		}
		return *this; 
	}

	SpriteRenderer& SpriteRenderer::FlipY(bool state) 
	{
		if ( state ) {
			_flags |= SpriteRendererFlags_FlipY;
		} else {
			_flags &= ~SpriteRendererFlags_FlipY;
		}
		return *this; 
	}

	SpriteRenderer& SpriteRenderer::Flip45() 
	{
		bool state = CheckFlag(SpriteRendererFlags_Flip45);
		if ( state ) {
			_flags |= SpriteRendererFlags_FlipX;
		} else {
			_flags &= ~SpriteRendererFlags_FlipX;
		}
		return *this; 
	}

	SpriteRenderer& SpriteRenderer::SetColor(const glm::vec4& color) 
	{ 
		_colorTint = color; 
		return *this;
	}

	SpriteRenderer& SpriteRenderer::SetTileMultiplier(double tileMultiplier) 
	{
		_tileMultiplier = tileMultiplier; 
		return *this; 
	}

	SpriteRenderer& SpriteRenderer::SetAnchor(SpriteAnchor anchor) 
	{
		_anchor = anchor; 
		return *this; 
	}

	SpriteRenderer& SpriteRenderer::SetFixedInViewport(bool state) 
	{
		if ( state ) {
			_flags |= SpriteRendererFlags_FixedInViewport;
		} else {
			_flags &= ~SpriteRendererFlags_FixedInViewport;
		}
		return *this;
	}

	SpriteRenderer& SpriteRenderer::SetFlags(SpriteRendererFlags newFlags) 
	{
		_flags = newFlags; 
		return *this;
	}


		/* Getters */

	const Shared<Texture2D>& SpriteRenderer::GetTexture() const 
	{
		return _texture; 
	}

	const glm::vec4 SpriteRenderer::GetTextureCoordinates() const
	{
		glm::vec4 updatedTextCoord = _defaultTextureCoordinates;

		if ( CheckFlag(SpriteRendererFlags_FlipX) ) {
			updatedTextCoord.x = _defaultTextureCoordinates.y;
			updatedTextCoord.y = _defaultTextureCoordinates.x;
		}

		if ( CheckFlag(SpriteRendererFlags_FlipY) ) {
			updatedTextCoord.z = _defaultTextureCoordinates.w;
			updatedTextCoord.w = _defaultTextureCoordinates.z;
		} else {
			updatedTextCoord.z = _defaultTextureCoordinates.z;
			updatedTextCoord.w = _defaultTextureCoordinates.w;
		}

		glm::vec4 buffer = updatedTextCoord;

		if ( CheckFlag(SpriteRendererFlags_Flip45) ) {
			updatedTextCoord.x = buffer.z;
			updatedTextCoord.y = buffer.w;
			updatedTextCoord.z = buffer.x;
			updatedTextCoord.w = buffer.y;
		}

		return updatedTextCoord;
	}

	const glm::vec4& SpriteRenderer::GetColorTint() const 
	{
		return _colorTint; 
	}

	const double SpriteRenderer::GetTileMultiplier() const 
	{
		return _tileMultiplier; 
	}

	const SpriteAnchor SpriteRenderer::GetAnchor() const 
	{
		return _anchor; 
	}

	glm::vec4 SpriteRenderer::GetOffsets() const 
	{
		return GetOffsetsFromAnchor(_anchor); 
	}

	glm::vec4 SpriteRenderer::GetOffsetsFromAnchor(SpriteAnchor anchor) const
	{
		switch ( anchor ) {
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


	bool SpriteRenderer::CheckFlag(SpriteRendererFlags_ flag) const
	{
		return ( _flags & flag ) == flag;
	}


		/* Flags */

	SpriteRendererFlags SpriteRenderer::GetFlags() const
	{
		return _flags; 
	}

	const bool SpriteRenderer::IsFlippedX() const
	{
		return CheckFlag(SpriteRendererFlags_FlipX);
	}

	const bool SpriteRenderer::IsFlippedY() const
	{
		return CheckFlag(SpriteRendererFlags_FlipY);
	}

	const bool SpriteRenderer::IsFixed() const
	{
		return CheckFlag(SpriteRendererFlags_FixedInViewport);
	}

	const bool SpriteRenderer::IsTransparent() const
	{
		return CheckFlag(SpriteRendererFlags_Transparent);
	}

}