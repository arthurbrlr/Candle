#pragma once

#include <Candle.h>
using namespace Candle;

#include "../TestECS.h"

class MouseClick : public Script {

	public:
		MouseClick()
		{
			_name = __FILE__;
		}

		MouseClick(Blueprint* player)
			: _playerBlueprint(player)
		{
			_name = __FILE__;
		}

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

				Blueprint& added = ECS::New("added");
				added.AddComponent<Transform>(finalPosition).SetScale({ 0.05, 0.05, 0 });
				added.AddComponent<SpriteRenderer>(Assets::GetTexture2D("dodo"));

				LineRenderSystem::AddLine(camPos, finalPosition);
			}
		}


		void OnEditor() override
		{
			ImGui::InputFloat3("Mouse Pos", glm::value_ptr(_mPos), 3, ImGuiInputTextFlags_ReadOnly);
		}


	private:	

		int _x;
		glm::vec3 _mPos;
		Blueprint* _playerBlueprint;

		//glm::vec3 _mPos = glm::vec3(0);
		//Blueprint* _playerBlueprint = nullptr;

};

AUTO_REGISTER_SCRIPT(MouseClick)
