#pragma once

#include <Candle.h>
using namespace Candle;

class RandomMovementAgent : public Script {
public:
	void OnAwake() override
	{

	}


	void OnUpdate() override
	{
		Transform& transform = _entity.GetComponent<Transform>();

		refresh -= Time::DeltaTime();

		if ( refresh < 0 ) {
			direction = { Noise::Random(-1, 1), Noise::Random(-1, 1), 0 };
			//direction = glm::normalize(direction);
			refresh = Noise::Random() * 5;
		}

		transform.Move(direction * (float)Time::DeltaTime());
	}


	void OnEditor() override
	{
		ImGui::InputFloat("Refresh", &refresh);
		ImGui::InputFloat3("Direction", glm::value_ptr(direction));
	}


private:
	float refresh = 0;
	glm::vec3 direction;

CANDLE_DECL_SCRIPT(RandomMovementAgent)
};

CANDLE_SCRIPT(RandomMovementAgent)