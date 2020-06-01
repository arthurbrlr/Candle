#include "cdlpch.h"
#include "CameraManagement.h"
#include "Candle/ECS/ECS.h"
#include "Candle/ECS/Components/CameraHandler.h"

namespace Candle {

	CameraManagement CameraManagement::_instance;
	long CameraManagement::_mainCameraBlueprintID = -1;
	glm::mat4 CameraManagement::_viewProjectionMatrix = glm::mat4(0);
	glm::mat4 CameraManagement::_dummy = glm::mat4(0);


	void CameraManagement::Init()
	{
		_mainCameraBlueprintID = -1;
	}


	void CameraManagement::UpdateView()
	{
		if ( _mainCameraBlueprintID != -1 ) {
			_viewProjectionMatrix = ECS::GetBlueprint(_mainCameraBlueprintID)->GetComponent<CameraHandler>().UpdatePVMatrix();
		}
	}


	void CameraManagement::RegisterMainCamera(long mainCameraBlueprintID)
	{
		if (_mainCameraBlueprintID != -1 )
			ECS::GetBlueprint(_mainCameraBlueprintID)->GetComponent<CameraHandler>().SetAsMainCamera(false);
		_mainCameraBlueprintID = mainCameraBlueprintID;
	}


	glm::mat4& CameraManagement::GetProjectionMatrix()
	{
		if ( _mainCameraBlueprintID != -1 ) {
			return ECS::GetBlueprint(_mainCameraBlueprintID)->GetComponent<CameraHandler>().GetProjection();
		}

		return _dummy;
	}


	glm::mat4& CameraManagement::GetViewMatrix()
	{
		if ( _mainCameraBlueprintID != -1 ) {
			return ECS::GetBlueprint(_mainCameraBlueprintID)->GetComponent<CameraHandler>().GetView();
		}

		return _dummy;
	}


	Transform& CameraManagement::GetCameraTransform()
	{
		return ECS::GetBlueprint(_mainCameraBlueprintID)->GetComponent<Transform>();
	}

}