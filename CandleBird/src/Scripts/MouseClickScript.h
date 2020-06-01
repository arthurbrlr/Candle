#pragma once

#include <Candle.h>
using namespace Candle;

#include "../TestECS.h"

class MouseClick : public Script {

	public:

		void OnAwake() override
		{
			_name = "MouseClick";
		}


		void OnUpdate() override
		{
			auto[mx, my] = Input::GetMousePos();
			glm::vec2 normalizedCoords = GetNormalizedDeviceCoords(mx, my);
			glm::vec4 clipCoords = { normalizedCoords.x, normalizedCoords.y, -1, 1 };
			glm::vec4 eyeCoords = ClipToEyeSpace(clipCoords);
			glm::vec3 ray = EyeToWorldSpace(eyeCoords);

			glm::vec3 camPos = CameraManagement::GetCameraTransform().GetPosition();
			glm::vec3 scaledPos = 10.f * ray;

			glm::vec3 finalPosition = camPos + scaledPos;

			_mPos = finalPosition;

			if ( Input::OnMouseButtonDown(CDL_MOUSE_BUTTON_1) ) {

				Blueprint& added = ECS::New("added");
				added.AddComponent<Transform>(finalPosition).SetScale({ 0.05, 0.05, 0 });
				added.AddComponent<SpriteRenderer>();

				LineRenderSystem::AddLine(camPos, finalPosition);
			}

		}


		void OnEditor() override
		{
			ImGui::InputFloat3("Mouse Pos", glm::value_ptr(_mPos), 3, ImGuiInputTextFlags_ReadOnly);
		}


	private:	

		glm::vec3 _mPos = glm::vec3(0);

		glm::vec2 GetNormalizedDeviceCoords(double mx, double my)
		{
			double retX = ( 2. * mx ) / CDL_APP_WIDTH - 1;
			double retY = 1. - ( 2. * my ) / CDL_APP_HEIGHT;
			return { retX, retY };
		}


		glm::vec4 ClipToEyeSpace(glm::vec4& clipCoords)
		{
			glm::mat4 invProjection = glm::inverse(CameraManagement::GetProjectionMatrix());
			glm::vec4 eyeCoords = invProjection * clipCoords;
			return { eyeCoords.x, eyeCoords.y, -1, 0 };
		}

		
		glm::vec3 EyeToWorldSpace(glm::vec4& eyeCoords)
		{
			glm::mat4 invView = glm::inverse(CameraManagement::GetViewMatrix());
			glm::vec4 rayWorld = invView * eyeCoords;
			return glm::vec3(rayWorld);
		}

};
