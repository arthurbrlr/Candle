#include "cdlpch.h"
#include "EngineComponents.h"

namespace Candle {

		// Helpers
	CANDLE_COMPONENT(EntityTagComponent)
	CANDLE_COMPONENT(HierarchyComponent)

		// Basics
	CANDLE_COMPONENT(CameraHandler)
	CANDLE_COMPONENT(Transform)

		// Renderer 2D
	CANDLE_COMPONENT(SpriteRenderer)

		// Physics 2D
	CANDLE_COMPONENT(DebugPointCollider)
	CANDLE_COMPONENT(CircleCollider)
	CANDLE_COMPONENT(BoxCollider)
	CANDLE_COMPONENT(AABB)

		// Animations
	CANDLE_COMPONENT(AnimationController)

		// Scripting
	CANDLE_COMPONENT(ScriptComponent)

}
