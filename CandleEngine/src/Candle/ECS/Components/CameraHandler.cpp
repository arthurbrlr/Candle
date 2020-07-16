#include "cdlpch.h"
#include "CameraHandler.h"

#include "Candle/Renderer/CameraManagement.h"
#include "Candle/ECS/Blueprint.h"
#include "Transform.h"


namespace Candle {

	CameraHandler::CameraHandler()
		: _type(Perspective)
	{
		_name = "CameraHandler";
		_camera = std::make_unique<PerspectiveCamera>(1920, 1080);
	}


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
		static char* camProjCombo[2];
		static bool  orthoSelected;
		bool previousSelected = orthoSelected;
		camProjCombo[0] = (char*)"Orthographic";
		camProjCombo[1] = (char*)"Perspective";
		if ( ImGui::BeginCombo("Type", _type == Orthographic ? "Orthographic" : "Perspective") ) {
			if ( ImGui::Selectable("Orthographic", &orthoSelected) ) orthoSelected = true;
			if ( ImGui::Selectable("Perspective", !&orthoSelected) ) orthoSelected = false;

			if ( previousSelected != orthoSelected ) {
				if ( orthoSelected ) {
					_camera.reset(new OrthographicCamera(-1.78, 1.78, -1, 1));
					_type = Orthographic;
				} else {
					_camera.reset(new PerspectiveCamera(1920, 1080));
					_type = Perspective;
				}
			}

			ImGui::EndCombo();
		}

		ImGui::Text(cameraType.c_str());

		static float camWidth;
		static float camHeight;

		switch ( _type ) {
			case Orthographic:
				camWidth  = ( float )dynamic_cast<OrthographicCamera*>( _camera.get() )->GetHorizontalValue();
				camHeight =	( float )dynamic_cast<OrthographicCamera*>( _camera.get() )->GetVerticalValue();
				ImGui::DragFloat("Horizontal", &camWidth, 0.1);
				ImGui::DragFloat("Vertical", &camHeight, 0.1);
				dynamic_cast<OrthographicCamera*>( _camera.get() )->SetProjection(-camWidth, camWidth, -camHeight, camHeight);
				break;
			case Perspective:
				camWidth = (float)dynamic_cast<PerspectiveCamera*>( _camera.get() )->GetWidth();
				camHeight = (float)dynamic_cast<PerspectiveCamera*>( _camera.get() )->GetHeight();
				ImGui::DragFloat("Width", &camWidth);
				ImGui::DragFloat("Height", &camHeight);
				dynamic_cast<PerspectiveCamera*>( _camera.get() )->SetWidth(camWidth);
				dynamic_cast<PerspectiveCamera*>( _camera.get() )->SetHeight(camHeight);
				break;
		}

		static bool isMain;
		isMain = CameraManagement::GetMainCameraID() == _blueprint->GetID();
		ImGui::Checkbox("Main Camera", &isMain);
		if ( isMain ) CameraManagement::RegisterMainCamera(_blueprint->GetID());
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