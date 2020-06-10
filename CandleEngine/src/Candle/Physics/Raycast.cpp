#include "cdlpch.h"
#include "Raycast.h"

#include "Candle/Core/Input.h"
#include "Candle/Renderer/CameraManagement.h"

namespace Candle {


	RayCollisionData Ray::Cast(RayData data)
	{
		return RayCollisionData();
	}

	RayCollisionData Ray::CastFromMouse()
	{
		RayCollisionData collisionData;

		glm::vec3 rayDirection = Input::GetMouseInWorldSpace();
		glm::vec3 rayOrigin = CameraManagement::GetCameraTransform().GetPosition();

			// Do something here to 

		collisionData.origin = rayOrigin;
		collisionData.direction = rayDirection;

		return collisionData;
	}


}