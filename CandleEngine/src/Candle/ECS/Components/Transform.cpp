#include "cdlpch.h"
#include "Transform.h"

#include "Candle/ECS/Blueprint.h"

namespace Candle {


	Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
		: _position(position), _rotation(rotation), _scale(scale)
	{
		UpdateMatrix();
		_name = "Transform";
	}


	void Transform::OnEditor()
	{
		ImGui::Text("Position:");
		ImGui::InputFloat3("p", glm::value_ptr(_position), 3);

		ImGui::Text("Scale:");
		ImGui::InputFloat3("s", glm::value_ptr(_scale), 3);


		ImGui::Text("Rotation:");
		ImGui::InputFloat3("r", glm::value_ptr(_rotation), 3);

		UpdateMatrix();
	}


	void Transform::UpdateMatrix()
	{
		_transformMatrix = glm::mat4(1.0f);

		if ( _blueprint != nullptr && _blueprint->HasParent() && _blueprint->GetParent()->HasComponent<Transform>() ) {

			Transform& parentTransform = _blueprint->GetParent()->GetComponent<Transform>();

			/* Translate */
			_transformMatrix = glm::translate(_transformMatrix, parentTransform.GetPosition());

			/* Rotations */
			_transformMatrix = glm::rotate(_transformMatrix, glm::radians(parentTransform.GetRotation().x), glm::vec3(1.f, 0.f, 0.f));
			_transformMatrix = glm::rotate(_transformMatrix, glm::radians(parentTransform.GetRotation().y), glm::vec3(0.f, 1.f, 0.f));
			_transformMatrix = glm::rotate(_transformMatrix, glm::radians(parentTransform.GetRotation().z), glm::vec3(0.f, 0.f, 1.f));

			/* Scaling */
			_transformMatrix = glm::scale(_transformMatrix, parentTransform.GetScale());
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