#include "cdlpch.h"
#include "CameraHandler.h"

#include "Candle/ECS/Entity.h"
#include "Transform.h"
#include "Candle/Renderer/CameraManagement.h"

#include "Utility/System/SerializeUtility.h"

namespace Candle {

	CameraHandler::CameraHandler()
	{
		_camera = new PerspectiveCamera(1920, 1080);
	}

	CameraHandler::CameraHandler(CameraType type, double horizontalSize, double verticalSize)
		: _type(type)
	{
		switch ( type ) {
		case CameraType::Orthographic:
			_camera = new OrthographicCamera(-horizontalSize / 2, horizontalSize / 2, -verticalSize / 2, verticalSize / 2);
			break;
		case CameraType::Perspective:
			_camera = new PerspectiveCamera(horizontalSize, verticalSize);
			break;
		}
	}

	CameraHandler::~CameraHandler()
	{
		if ( _camera ) delete _camera;
		_camera = nullptr;
	}

	void CameraHandler::Serialize(std::fstream& sceneFile)
	{
		SerializeInt(sceneFile, "mainCamera", (int)_isMainCamera);
		SerializeInt(sceneFile, "camtype", _type);
		
		switch ( _type ) {
			case Orthographic:
				SerializeFloat(sceneFile, "camWidth", (float)( (OrthographicCamera*)_camera )->GetHorizontalValue());
				SerializeFloat(sceneFile, "camheight", (float)( (OrthographicCamera*)_camera )->GetVerticalValue());
				break;
			case Perspective:
				SerializeFloat(sceneFile, "camWidth", (float)( (PerspectiveCamera*)_camera )->GetWidth());
				SerializeFloat(sceneFile, "camheight", (float)( (PerspectiveCamera*)_camera )->GetHeight());
				break;
		}
	}

	void CameraHandler::Deserialize(std::fstream& sceneFile)
	{
		GetSerializedInt(sceneFile, (int&)_isMainCamera);
		GetSerializedInt(sceneFile, (int&)_type);
		_type = (CameraType)_type;

		float camWidth, camHeight;
		switch ( _type ) {
			case Orthographic:
				if ( _camera ) delete _camera;
				GetSerializedFloat(sceneFile, camWidth);
				GetSerializedFloat(sceneFile, camHeight);
				_camera = new OrthographicCamera(-camWidth, camWidth, -camHeight, camHeight);
				break;
			case Perspective:
				GetSerializedFloat(sceneFile, camWidth);
				GetSerializedFloat(sceneFile, camHeight);
				( (PerspectiveCamera*)_camera )->SetWidth(camWidth);
				( (PerspectiveCamera*)_camera )->SetHeight(camHeight);
				break;
		}

		if ( _isMainCamera ) SetAsMainCamera(true);
	}

	void CameraHandler::OnEditor()
	{
		std::string cameraType = _type == Orthographic ? "Orthographic Camera" : "Perspective Camera";
		static bool orthoSelected;
		orthoSelected = ( _type == Orthographic );
		bool previousSelected = orthoSelected;
		if ( ImGui::BeginCombo("Type", _type == Orthographic ? "Orthographic" : "Perspective") ) {
			if ( ImGui::Selectable("Orthographic", orthoSelected) ) orthoSelected = true;
			if ( ImGui::Selectable("Perspective", !orthoSelected) ) orthoSelected = false;

			if ( previousSelected != orthoSelected ) {
				if ( orthoSelected ) {
					delete _camera;
					_camera = new OrthographicCamera(-1.78, 1.78, -1., 1.);
					_type = Orthographic;
				} else {
					delete _camera;
					_camera = new PerspectiveCamera(1920, 1080);
					_type = Perspective;
				}
			}

			ImGui::EndCombo();
		}

		ImGui::Text(cameraType.c_str());

		static float camWidth;
		static float camHeight;

		if ( !_camera ) {
			CERROR("Error: CameraHandler has no Camera pointer attached to it!");
			return;
		}

		switch ( _type ) {
			case Orthographic:
				camWidth = (float)( (OrthographicCamera*)_camera )->GetHorizontalValue();
				camHeight = (float)( (OrthographicCamera*)_camera )->GetVerticalValue();
				ImGui::DragFloat("Horizontal", &camWidth, 0.5, 0, 10);
				ImGui::DragFloat("Vertical", &camHeight, 0.5, 0, 10);
				( (OrthographicCamera*)_camera )->SetProjection(-camWidth, camWidth, -camHeight, camHeight);
				break;
			case Perspective:
				camWidth = (float)( (PerspectiveCamera*)_camera )->GetWidth();
				camHeight = (float)( (PerspectiveCamera*)_camera )->GetHeight();
				ImGui::DragFloat("Width", &camWidth, 1, 0, 3840);
				ImGui::DragFloat("Height", &camHeight, 1, 0, 3160);
				( (PerspectiveCamera*)_camera )->SetWidth(camWidth);
				( (PerspectiveCamera*)_camera )->SetHeight(camHeight);
				break;
		}

		static bool isMain;
		isMain = ( CameraManagement::GetMainCameraID() == _entity );
		if ( ImGui::Checkbox("Main Camera", &isMain) && isMain) {
			SetAsMainCamera(true);
		}
	}


	CameraHandler& CameraHandler::SetAsMainCamera(bool state)
	{
		_isMainCamera = state;
		if ( state && ( CameraManagement::GetMainCameraID() != _entity ) ) CameraManagement::RegisterMainCamera(_entity);
		return *this;
	}


	glm::mat4 CameraHandler::UpdatePVMatrix()
	{
		ComputeViewMatrix();
		return GetProjectionView();
	}


	void CameraHandler::ComputeViewMatrix()
	{
		Entity support = { SceneManagement::CurrentScene(), _entity };
		if ( !support.HasComponent<Transform>() ) RequireComponent<Transform>();
		Transform& transform = support.GetComponent<Transform>();

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