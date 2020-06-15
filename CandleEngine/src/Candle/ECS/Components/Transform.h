#pragma once

#include "Candle/CandleCore.h"
#include "Candle/ECS/Component.h"

namespace Candle {

	class CANDLE_API Transform : public Component {

		public:
			Transform(glm::vec3 position = { 0, 0, 0 }, glm::vec3 rotation = { 0, 0, 0 }, glm::vec3 scale = { 1, 1, 1 }); /* Constructor */
			void OnEditor() override;

			inline const glm::mat4 & Get() const { return _transformMatrix; }
			inline const glm::vec3 & GetPosition() const { return _position; }
			inline const glm::vec3 & GetRotation() const { return _rotation; }
			inline const glm::vec3 & GetScale() const { return _scale; }

			inline Transform & SetPosition(const glm::vec3 newPosition) { _position = newPosition; UpdateMatrix(); return *this; }
			inline Transform & SetRotation(const glm::vec3 newRotation) { _rotation = newRotation; UpdateMatrix(); return *this; }
			inline Transform & SetScale(const glm::vec3 newScale) { _scale = newScale; UpdateMatrix(); return *this; }

			inline void Move(const glm::vec3 & deltaMovement) { _position += deltaMovement; UpdateMatrix(); }
			inline void Rotate(const glm::vec3 & deltaRotate) { _rotation += deltaRotate; UpdateMatrix(); }
			inline void Scale(const glm::vec3 & deltaScale) { _scale *= deltaScale; UpdateMatrix(); }
			inline void Scale(const float deltaScale) { _scale *= deltaScale; UpdateMatrix(); }

			void ForceTransformUpdate() { UpdateMatrix(); }

		private:
			glm::vec3 _position, _scale, _rotation;
			glm::mat4 _transformMatrix = glm::mat4(0);

			void UpdateMatrix();
	};

}