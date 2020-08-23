#include "cdlpch.h"
#include "CameraManagement.h"
#include "Candle/ECS/ECS.h"
#include "Candle/ECS/Components/CameraHandler.h"

namespace Candle {

	CameraManagement CameraManagement::_instance;
	Burst::Entity CameraManagement::_mainCameraEntity = -1;
	glm::mat4 CameraManagement::_viewProjectionMatrix = glm::mat4(0);
	glm::mat4 CameraManagement::_dummy = glm::mat4(0);


	void CameraManagement::Init()
	{
		_mainCameraEntity = -1;
	}


	void CameraManagement::UpdateView()
	{
		if ( _mainCameraEntity != -1 ) {
			if (ECS::ViewEntity(_mainCameraEntity).HasComponent<CameraHandler>() )
				_viewProjectionMatrix = ECS::ViewEntity(_mainCameraEntity).GetComponent<CameraHandler>().UpdatePVMatrix();
		} else {
			_mainCameraEntity = -1;
		}
	}


	void CameraManagement::RegisterMainCamera(Burst::Entity mainCameraBlueprintID)
	{
		if (_mainCameraEntity != -1 )
			ECS::ViewEntity(_mainCameraEntity).GetComponent<CameraHandler>().SetAsMainCamera(false);
		_mainCameraEntity = mainCameraBlueprintID;
	}


	glm::mat4& CameraManagement::GetProjectionMatrix()
	{
		if ( _mainCameraEntity != -1 ) {
			return ECS::ViewEntity(_mainCameraEntity).GetComponent<CameraHandler>().GetProjection();
		}

		return _dummy;
	}


	glm::mat4& CameraManagement::GetViewMatrix()
	{
		if ( _mainCameraEntity != -1 ) {
			return ECS::ViewEntity(_mainCameraEntity).GetComponent<CameraHandler>().GetView();
		}

		return _dummy;
	}


	Transform& CameraManagement::GetCameraTransform()
	{
		return ECS::ViewEntity(_mainCameraEntity).GetComponent<Transform>();
	}

}