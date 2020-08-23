#pragma once

#include "Candle/CandleCore.h"

#include "Candle/Events/Event.h"
#include "Candle/Renderer/FrameBuffer.h"
#include "Candle/Assets/Texture2D.h"

#include "Burst/Registery.h"
#include "Utility/Maths/Noise.h"

namespace Candle {

	typedef std::string UUID;

	static uint32_t sceneID = 0;

	class Entity;
	class SceneManagement;
	class ECS;

	class Scene {

		public:
			Scene();
			virtual ~Scene() {}

			virtual void Load() = 0;
			virtual void Unload();

			virtual void OnUpdate() = 0;
			virtual void OnEvent(Event & event) {}
			virtual void OnEditor() {}

			void SetName(const char* name) { _sceneName = name; }

			Shared<FrameBuffer> & GetFrameBuffer() { return _sceneBuffer; }
			Shared<Texture2D> GetTexture() { return _sceneTexture; }
			uint32_t GetID() { return _sceneID; }
			const std::string& GetName() { return _sceneName; }

		protected:
			uint32_t _sceneID = 0;
			std::string _sceneName;
			Shared<FrameBuffer> _sceneBuffer;
			Shared<Texture2D> _sceneTexture;

			Burst::Registery<> _sceneRegistery;

			friend class Entity;
			friend class SceneManagement;
			friend class ECS;
	};

}