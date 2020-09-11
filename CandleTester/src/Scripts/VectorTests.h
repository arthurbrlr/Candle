#pragma once

#include <Candle.h>
using namespace Candle;

class VectorTests : public Script {

public:
	void OnAwake() override
	{
		originEntity = ECS::New("originEntity");
		rotateEntity = ECS::New("rotationEntity");

		originEntity.AddComponent<Transform>(glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0.1, 0.1, 0.1 });
		rotateEntity.AddComponent<Transform>(glm::vec3{ 2, 0, 0 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0.1, 0.1, 0.1 });

		originEntity.AddComponent<SpriteRenderer>();
		rotateEntity.AddComponent<SpriteRenderer>();

		_entity.AddChild(originEntity);
		_entity.AddChild(rotateEntity);
	}

	void OnUpdate() override
	{
	}


	void OnEvent(Event& event) override
	{
	}


	void OnEditor() override
	{
		if ( ImGui::Button("Rotate Point around") ) {

			glm::vec3 rotationEntityPosition = rotateEntity.GetComponent<Transform>().GetPosition();
			glm::vec3 originEntityPosition = originEntity.GetComponent<Transform>().GetPosition();

			toRotate = { rotationEntityPosition.x, rotationEntityPosition.y };
			rotationCenter = { originEntityPosition.x, originEntityPosition.y };
			
			Candle::Maths_Vec2_Rotate(toRotate, 10, rotationCenter);

			rotateEntity.GetComponent<Transform>().SetPosition(glm::vec3{ toRotate.x, toRotate.y, 0 });

		}
	}


private:
	Entity rotateEntity, originEntity;

	glm::vec2 rotationCenter;
	glm::vec2 toRotate;

	CANDLE_DECL_SCRIPT(VectorTests)

};

CANDLE_SCRIPT(VectorTests)