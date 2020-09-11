#include "cdlpch.h"
#include "Collider2D.h"

#include "Candle/ECS/Entity.h"
#include "Candle/ECS/Components/Transform.h"

namespace Candle {

	Collider2D::Collider2D()
		: Component::Component()
	{
	}



		/* Debug Point Collider */
	DebugPointCollider::DebugPointCollider()
		: Collider2D::Collider2D()
	{
	}

	glm::vec2 DebugPointCollider::GetPosition() const
	{
		Entity thisEntity{ SceneManagement::CurrentScene(), _entity };
		if ( thisEntity.HasComponent<Transform>() ) {
			glm::vec3 position = thisEntity.GetComponent<Transform>().GetPosition();
			return glm::vec2{ position.x, position.y };
		}
		return { 0.0, 0.0 };
	}

		/* Circle Collider */

	CircleCollider::CircleCollider(double radius)
		: Collider2D::Collider2D(), _radius(radius)
	{
	}

	double CircleCollider::GetRadius() const
	{
		return _radius;
	}

	glm::vec2 CircleCollider::GetPosition() const
	{
		Entity thisEntity{ SceneManagement::CurrentScene(), _entity };
		if ( thisEntity.HasComponent<Transform>() ) {
			glm::vec3 position = thisEntity.GetComponent<Transform>().GetPosition();
			return glm::vec2{ position.x, position.y };
		}
		return { 0.0, 0.0 };
	}

	void CircleCollider::OnEditor()
	{
		ImGui::InputDouble("Radius", &_radius);
	}



		/* Box Collider */
	BoxCollider::BoxCollider(const glm::vec2& size)
		: Collider2D::Collider2D(), _size(size)
	{
	}

	glm::vec2* BoxCollider::GetVertices() const
	{
		static glm::vec2 vertices[4];
		Entity thisEntity{ SceneManagement::CurrentScene(), _entity };
		
		vertices[0] = glm::vec2{ -_size.x / 2.0 , -_size.y / 2.0 };
		vertices[1] = glm::vec2{ -_size.x / 2.0 ,  _size.y / 2.0 };
		vertices[2] = glm::vec2{  _size.x / 2.0 , -_size.y / 2.0 };
		vertices[3] = glm::vec2{  _size.x / 2.0 ,  _size.y / 2.0 };

		if ( thisEntity.HasComponent<Transform>() ) {

			glm::mat4 identity(1.0);

			double rotZ = thisEntity.GetComponent<Transform>().GetRotation().z;
			identity = glm::rotate(identity, glm::radians((float)rotZ), glm::vec3{ 0, 0, 1 });
			identity = glm::scale(identity, thisEntity.GetComponent<Transform>().GetScale());

			glm::mat2 rotationMatrix = { identity[0][0], identity[0][1], identity[1][0], identity[1][1] };
			glm::vec3 position = thisEntity.GetComponent<Transform>().GetPosition();

			for ( int i = 0; i < 4; i++ ) {
				vertices[i] = rotationMatrix * vertices[i] + (glm::vec2)position;
			}

		}

		return vertices;
	}

	glm::vec2 BoxCollider::GetSize() const
	{
		return _size;
	}

	glm::vec2 BoxCollider::GetPosition() const
	{
		Entity thisEntity{ SceneManagement::CurrentScene(), _entity };
		if ( thisEntity.HasComponent<Transform>() ) {
			glm::vec3 position = thisEntity.GetComponent<Transform>().GetPosition();
			return glm::vec2{ position.x, position.y };
		}
		return { 0.0, 0.0 };
	}

	double BoxCollider::GetRotation() const
	{
		Entity thisEntity{ SceneManagement::CurrentScene(), _entity };
		if ( thisEntity.HasComponent<Transform>() ) {
			return thisEntity.GetComponent<Transform>().GetRotation().z;
		}
		return 0.;
	}


	void BoxCollider::OnEditor()
	{
		static glm::vec2* vertices = nullptr;

		if ( ImGui::Button("GetVertices") ) {
			vertices = GetVertices();
		}

		ImGui::InputFloat("X size", &_size.x);
		ImGui::InputFloat("Y size", &_size.y);

		if ( vertices ) {
			ImGui::Text("Vertices:");
			ImGui::InputFloat2("v0", glm::value_ptr(vertices[0]), 2, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat2("v1", glm::value_ptr(vertices[1]), 2, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat2("v2", glm::value_ptr(vertices[2]), 2, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat2("v3", glm::value_ptr(vertices[3]), 2, ImGuiInputTextFlags_ReadOnly);
		}
	}

		/* AABB Collider */

	AABB::AABB(const glm::vec2& size)
		: Collider2D::Collider2D(), _size(size)
	{
	}

	glm::vec2 AABB::GetMin() const
	{
		Entity thisEntity{ SceneManagement::CurrentScene(), _entity };
		glm::vec2 position = { 0, 0 };

		if ( thisEntity.HasComponent<Transform>() ) {
			position = thisEntity.GetComponent<Transform>().GetPosition();
		}

		return position - 0.5f * _size;
	}

	glm::vec2 AABB::GetMax() const
	{
		Entity entity{ SceneManagement::CurrentScene(), _entity };
		glm::vec2 position = { 0, 0 };

		if ( entity.HasComponent<Transform>() ) {
			position = entity.GetComponent<Transform>().GetPosition();
		}

		return position + 0.5f * _size;
	}

	glm::vec2 AABB::GetPosition() const
	{
		Entity entity{ SceneManagement::CurrentScene(), _entity };
		glm::vec2 position = { 0, 0 };

		if ( entity.HasComponent<Transform>() ) {
			return entity.GetComponent<Transform>().GetPosition();
		}

		return { 0, 0 };
	}

	glm::vec2 AABB::GetSize() const
	{
		return _size;
	}

}