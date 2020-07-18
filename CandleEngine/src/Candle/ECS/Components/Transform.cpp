#include "cdlpch.h"
#include "Transform.h"

#include "Candle/ECS/Blueprint.h"

namespace Candle {

	Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
		: _position(position), _rotation(rotation), _scale(scale)
	{
		_name = "Transform";
	}

	Transform::Transform(Blueprint* parent, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
		: _position(position), _rotation(rotation), _scale(scale)
	{
		AttachToBlueprint(parent);
		UpdateMatrix();
		_name = "Transform";
	}


	void Transform::OnEditor()
	{
		ImGui::Text("Position:");
		ImGui::DragFloat3("p", glm::value_ptr(_position), 0.1);

		ImGui::Text("Scale:");
		ImGui::DragFloat3("s", glm::value_ptr(_scale), 0.1, 0, 1000);


		ImGui::Text("Rotation:");
		ImGui::DragFloat3("r", glm::value_ptr(_rotation), 0.1, 0, 360);

		UpdateMatrix();
	}


		/* Component Methods */
	void Transform::Move(const glm::vec3& deltaMovement) 
	{
		_position += deltaMovement; 
		UpdateMatrix(); 
	}

	void Transform::Rotate(const glm::vec3& deltaRotate) 
	{
		_rotation += deltaRotate; 
		UpdateMatrix(); 
	}

	void Transform::Scale(const glm::vec3& deltaScale) 
	{
		_scale *= deltaScale; 
		UpdateMatrix(); 
	}

	void Transform::Scale(const float deltaScale) 
	{
		_scale *= deltaScale; 
		UpdateMatrix(); 
	}


	void Transform::ForceTransformUpdate() 
	{
		UpdateMatrix(); 
	}


		/* Getters */

	const glm::mat4& Transform::Get() const 
	{
		return _transformMatrix; 
	}

	const glm::vec3& Transform::GetPosition() const 
	{
		return _position; 
	}

	const glm::vec3& Transform::GetRotation() const 
	{
		return _rotation; 
	}

	const glm::vec3& Transform::GetScale() const 
	{
		return _scale; 
	}

	const glm::vec3 Transform::GetForward() const
	{
		glm::mat4 inverted = glm::inverse(_transformMatrix);
		glm::vec3 forward = normalize(glm::vec3(inverted[2]));
		return forward;
	}

	const glm::vec3 Transform::GetRight() const
	{
		glm::mat4 inverted = glm::inverse(_transformMatrix);
		glm::vec3 forward = normalize(glm::vec3(inverted[0]));
		return forward;
	}


		/* Setters */

	Transform& Transform::SetPosition(const glm::vec3 newPosition)
	{
		_position = newPosition; 
		UpdateMatrix(); 
		return *this; 
	}

	Transform& Transform::SetRotation(const glm::vec3 newRotation)
	{
		_rotation = newRotation; 
		UpdateMatrix(); 
		return *this; 
	}

	Transform& Transform::SetScale(const glm::vec3 newScale)
	{
		_scale = newScale; 
		UpdateMatrix(); 
		return *this; 
	}


	void Transform::UpdateMatrix()
	{
		_transformMatrix = glm::mat4(1.0f);

		bool hasParentTransform = _blueprint != nullptr && _blueprint->HasParent() && _blueprint->GetParent()->HasComponent<Transform>();

		if ( hasParentTransform ) {
			Transform& parentTransform = _blueprint->GetParent()->GetComponent<Transform>();
			_transformMatrix = parentTransform.Get();
		}

		/* Translate */
		_transformMatrix = glm::translate(_transformMatrix, _position);

		/* Rotations */
		_transformMatrix = glm::rotate(_transformMatrix, glm::radians(_rotation.x), glm::vec3(1.f, 0.f, 0.f));
		_transformMatrix = glm::rotate(_transformMatrix, glm::radians(_rotation.y), glm::vec3(0.f, 1.f, 0.f));
		_transformMatrix = glm::rotate(_transformMatrix, glm::radians(_rotation.z), glm::vec3(0.f, 0.f, 1.f));

		/* Scaling */
		_transformMatrix = glm::scale(_transformMatrix, _scale);

		if ( _blueprint != nullptr && _blueprint->HasChildren() ) {
			for ( auto& child : _blueprint->GetChilds() ) {
				if ( child.second->HasComponent<Transform>() ) {
					child.second->GetComponent<Transform>().ForceTransformUpdate();
				}
			}
		}
	}
}