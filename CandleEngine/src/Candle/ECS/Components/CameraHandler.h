#pragma once

#include "Candle/CandleCore.h"

#include "Transform.h"
#include "Candle/ECS/Blueprint.h"
#include "Candle/Renderer/CameraManagement.h"
#include "glm/glm.hpp"

namespace Candle {

	class CANDLE_API CameraHandler : public Component {

		public:
			CameraHandler(CameraType type, double horizontalSize, double verticalSize)
			{
				_name = "CameraHandler";
			
				switch(type)
				{
					case CameraType::Orthographic:
						_camera = std::make_unique<OrthographicCamera>(-horizontalSize / 2, horizontalSize / 2, -verticalSize / 2, verticalSize / 2);
						break;
					case CameraType::Perspective:
						_camera = std::make_unique<PerspectiveCamera>(horizontalSize, verticalSize);
						break;
				}
			}


			void OnEditor() override
			{
				ImGui::Text("Placeholder camera handler");

				float parentPos[3] = { _parent->GetComponent<Transform>().GetPosition().x, _parent->GetComponent<Transform>().GetPosition().y, _parent->GetComponent<Transform>().GetPosition().z };
				ImGui::InputFloat3("Position", parentPos);
			}


			CameraHandler& SetAsMainCamera(bool state) 
			{
				_isMainCamera = state;
				if (state) CameraManagement::RegisterMainCamera(_parent->GetID()); 
				return *this; 
			}


			inline glm::mat4 UpdatePVMatrix()
			{
				ComputeViewMatrix();
				return GetProjectionView();
			}


			inline glm::mat4 GetProjectionView() 
			{
				return _camera->GetProjection() * _viewMatrix; 
			}


			inline glm::mat4 GetViewProjection()
			{
				return _viewMatrix * _camera->GetProjection();
			}


			inline glm::mat4& GetView()
			{
				return _viewMatrix;
			}


			inline glm::mat4& GetProjection()
			{
				return _camera->GetProjection();
			}
			

		private:
			Unique<RawCamera> _camera;
			glm::mat4 _viewMatrix = glm::mat4(0);
			bool _isMainCamera = true;

			void ComputeViewMatrix()
			{
				Transform& transform = _parent->GetComponent<Transform>();

				_viewMatrix = glm::mat4(1.0f);

				// Rotation
				_viewMatrix = glm::rotate(_viewMatrix, glm::radians(transform.GetRotation().x), glm::vec3(1.f, 0.f, 0.f));
				_viewMatrix = glm::rotate(_viewMatrix, glm::radians(transform.GetRotation().y), glm::vec3(0.f, 1.f, 0.f));
				_viewMatrix = glm::rotate(_viewMatrix, glm::radians(transform.GetRotation().z), glm::vec3(0.f, 0.f, 1.f));

				// Translation
				glm::vec3 negativeCameraPos = glm::vec3(-transform.GetPosition().x, -transform.GetPosition().y, -transform.GetPosition().z);
				_viewMatrix = glm::translate(_viewMatrix, negativeCameraPos);
			}

	};

}