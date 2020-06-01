#pragma once

#include "Candle/CandleCore.h"
#include "Candle/ECS/Component.h"

namespace Candle {

	class CANDLE_API Transform : public Component {

		public:
			Transform(glm::vec3 position = { 0, 0, 0 }, glm::vec3 rotation = { 0, 0, 0 }, glm::vec3 scale = { 1, 1, 1 })
				: _position(position), _rotation(rotation), _scale(scale)
			{
				UpdateMatrix();
				_name = "Transform";
			}


			static Shared<Transform> Create(glm::vec3 position = { 0, 0, 0 }, glm::vec3 rotation = { 0, 0, 0 }, glm::vec3 scale = { 1, 1, 1 })
			{
				return std::make_shared<Transform>(position, rotation, scale);
			}


			void OnEditor() override
			{
				ImGui::Text("Position:");
				ImGui::InputFloat3("p", glm::value_ptr(_position), 3);
				
				ImGui::Text("Scale:");
				ImGui::InputFloat3("s", glm::value_ptr(_scale), 3);

				
				ImGui::Text("Rotation:");
				ImGui::InputFloat3("r", glm::value_ptr(_rotation), 3);
			}


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

		private:
			glm::vec3 _position, _scale, _rotation;
			glm::mat4 _transformMatrix = glm::mat4(0);

			void UpdateMatrix()
			{
				_transformMatrix = glm::mat4(1.0f);

				/* Translate */
				_transformMatrix = glm::translate(_transformMatrix, _position);

				/* Rotations */
				_transformMatrix = glm::rotate(_transformMatrix, glm::radians(_rotation.x), glm::vec3(1.f, 0.f, 0.f));
				_transformMatrix = glm::rotate(_transformMatrix, glm::radians(_rotation.y), glm::vec3(0.f, 1.f, 0.f));
				_transformMatrix = glm::rotate(_transformMatrix, glm::radians(_rotation.z), glm::vec3(0.f, 0.f, 1.f));

				/* Scaling */
				_transformMatrix = glm::scale(_transformMatrix, _scale);
			}


				/* DEBUG - EDITOR */

			float _positionArray[3] = { 0, 0, 0 };
			float _scaleArray[3] = { 1, 1, 1 };
			float _rotationArray[3] = { 0, 0, 0 };
	};

}