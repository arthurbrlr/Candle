#pragma once

#include <Candle.h>
using namespace Candle;

class PlayerScript : public Script {
public:
	void OnAwake() override
	{
		vel = { 0, 0, 0 };
		acc = { 0, -30, 0 };
		movementForce = { 0, 0, 0 };
		upVelocity = 20;
	}

	void OnUpdate() override
	{
		Transform& transform = _entity.GetComponent<Transform>();

		if (Input::IsKeyPressed(CDL_KEY_SPACE) && grounded) {
			vel.y += upVelocity;
		}

		if (Input::IsKeyPressed(CDL_KEY_A) && !Input::IsKeyPressed(CDL_KEY_D)) {
			vel.x = -5;
			if ( _entity.HasComponent<SpriteRenderer>() ) _entity.GetComponent<SpriteRenderer>().FlipX(true);
		} else if (Input::IsKeyPressed(CDL_KEY_D) && !Input::IsKeyPressed(CDL_KEY_A)) {
			vel.x = 5;
			if ( _entity.HasComponent<SpriteRenderer>() ) _entity.GetComponent<SpriteRenderer>().FlipX(false);
		} else {
			vel.x = 0;
		}


		vel += (float)Time::DeltaTime() * acc;
		transform.Move((float)Time::DeltaTime() * vel);
			
		glm::vec3 pos = transform.GetPosition();
		if (pos.y < 0) {
			transform.SetPosition({ pos.x, 0, pos.z });
			vel.y = 0;
			grounded = true;
		} else {
			grounded = false;
		}
	}


	void OnEvent(Event & event) override
	{
		EventDispatcher dispatcher(event);
	}


	void OnEditor() override
	{
		ImGui::InputFloat3("vel", glm::value_ptr(vel), 3);
		ImGui::InputFloat3("acc", glm::value_ptr(acc), 3);
		ImGui::Spacing();
		ImGui::InputFloat("Up velocity", &upVelocity);
	}


private:
	bool grounded = true;
	glm::vec3 vel, acc, movementForce;
	float upVelocity;

CANDLE_DECL_SCRIPT(PlayerScript)
};

CANDLE_SCRIPT(PlayerScript)