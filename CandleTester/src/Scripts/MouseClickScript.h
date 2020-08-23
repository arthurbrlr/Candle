#pragma once

#include <Candle.h>
using namespace Candle;

#include "../TestECS.h"

class MouseClick : public Script {
public:
	void OnAwake() override
	{
	}


	void OnUpdate() override
	{
		if ( Input::OnMouseButtonDown(CDL_MOUSE_BUTTON_1) ) {

			RayCollisionData data = Ray::CastFromMouse();
			glm::vec3 ray = data.direction;
			glm::vec3 camPos = data.origin;
			glm::vec3 scaledPos = 10.f * ray;

			glm::vec3 finalPosition = camPos + scaledPos;

			_mPos = finalPosition;

			Entity entity = ECS::New("added");
			entity.AddComponent<Transform>(finalPosition).SetScale({ 0.05, 0.05, 0 });
			entity.AddComponent<SpriteRenderer>(Assets::GetTexture2D("dodo"));

			//LineRenderSystem::AddLine(camPos, finalPosition);
		}
	}


	void OnEditor() override
	{
		ImGui::InputFloat3("Mouse Pos", glm::value_ptr(_mPos), 3, ImGuiInputTextFlags_ReadOnly);
	}


private:	
	int _x;
	glm::vec3 _mPos;

CANDLE_DECL_SCRIPT(MouseClick)
};

CANDLE_SCRIPT(MouseClick)
