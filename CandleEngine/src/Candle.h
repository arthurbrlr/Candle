#pragma once

	// For use by Candle applications

	// --- Candle Engine ---

#include "Candle/Application.h"
#include "Candle/Core/Logger.h"
#include "Candle/Core/Time.h"

#include "Candle/Layers/Layer.h"

#include "Candle/Core/Input.h"
#include "Candle/Core/MouseButtons.h"
#include "Candle/Core/Keycodes.h"

#include "Candle/Audio/AudioMixer.h"

#include "Candle/Renderer/Renderer.h"
#include "Candle/Renderer/Renderer2D.h"
#include "Candle/Renderer/RenderCommands.h"
#include "Candle/Renderer/FrameBuffer.h"
#include "Candle/Renderer/CameraManagement.h"


#include "Candle/Assets/Assets.h"
#include "Candle/Assets/Texture.h"
#include "Candle/Assets/Shader.h"

#include "Candle/ECS/ECS.h"
#include "Candle/ECS/Components/EngineComponents.h"
#include "Candle/ECS/Systems/EngineSystems.h"

// ECS Refactor
#include "Candle/Scenes/Scene.h"
#include "Candle/Scenes/SceneManagement.h"

#include "Candle/Physics/Raycast.h"
#include "Candle/Physics/CollisionWorld.h"


	// --- Utility ---
#include "Utility/Maths/Noise.h"
