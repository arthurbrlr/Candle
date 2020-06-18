#include "cdlpch.h"
#include "CameraHandler.h"

#include "Candle/Renderer/CameraManagement.h"
#include "Candle/ECS/Blueprint.h"
#include "Transform.h"


namespace Candle {

	CameraHandler::CameraHandler(CameraType type, double horizontalSize, double verticalSize)
	{
		_name = "CameraHandler";
		_type = type;

		switch ( type ) {
		case CameraType::Orthographic:
			_camera = std::make_unique<OrthographicCamera>(-horizontalSize / 2, horizontalSize / 2, -verticalSize / 2, verticalSize / 2);
			break;
		case CameraType::Perspective:
			_camera = std::make_unique<PerspectiveCamera>(horizontalSize, verticalSize);
			break;
		}
	}


	void CameraHandler::OnEditor()
	{
		std::string cameraType = _type == Orthographic ? "Orthographic Camera" : "Perspective Camera";

		ImGui::Text(cameraType.c_str());
	}


	CameraHandler& CameraHandler::SetAsMainCamera(bool state)
	{
		_isMainCamera = state;
		if ( state ) CameraManagement::RegisterMainCamera(_blueprint->GetID());
		return *this;
	}


	glm::mat4 CameraHandler::UpdatePVMatrix()
	{
		ComputeViewMatrix();
		return GetProjectionView();
	}


	void CameraHandler::ComputeViewMatrix()
	{
		Transform& transform = _blueprint->GetComponent<Transform>();

		_viewMatrix = glm::mat4(1.0f);

		// Rotation
		_viewMatrix = glm::rotate(_viewMatrix, glm::radians(transform.GetRotation().x), glm::vec3(1.f, 0.f, 0.f));
		_viewMatrix = glm::rotate(_viewMatrix, glm::radians(transform.GetRotation().y), glm::vec3(0.f, 1.f, 0.f));
		_viewMatrix = glm::rotate(_viewMatrix, glm::radians(transform.GetRotation().z), glm::vec3(0.f, 0.f, 1.f));

		// Translation
		glm::vec3 negativeCameraPos = glm::vec3(-transform.GetPosition().x, -transform.GetPosition().y, -transform.GetPosition().z);
		_viewMatrix = glm::translate(_viewMatrix, negativeCameraPos);
	}


		/* Getters */

	glm::mat4 CameraHandler::GetProjectionView()
	{
		return _camera->GetProjection() * _viewMatrix;
	}

	glm::mat4 CameraHandler::GetViewProjection()
	{
		return _viewMatrix * _camera->GetProjection();
	}

	glm::mat4& CameraHandler::GetView()
	{
		return _viewMatrix;
	}

	glm::mat4& CameraHandler::GetProjection()
	{
		return _camera->GetProjection();
	}

}