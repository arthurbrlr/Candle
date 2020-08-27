#pragma once

#include "Candle/CandleCore.h"

#include "Candle/Events/Event.h"
#include "Candle/Renderer/FrameBuffer.h"
#include "Candle/Assets/Texture2D.h"

#include "Burst/Registery.h"
#include "Utility/System/Identifier.h"

namespace Candle {

	static uint32_t sceneID = 0;

	class Entity;
	class SceneManagement;
	class ECS;

	class Scene {

		public:
			Scene();
			virtual ~Scene() {}

			virtual void Load(const std::string& sceneFilePath);
			void Unload(const std::string& sceneFilePath = "");
			void Save(const std::string& sceneFilePath);

			void OnUpdate() {};
			void OnEvent(Event & event) {}
			void OnEditor() {}

			Shared<FrameBuffer> & GetFrameBuffer() { return _sceneBuffer; }
			Shared<Texture2D> GetTexture() { return _sceneTexture; }
			uint32_t GetID() { return _sceneID; }

		protected:
			uint32_t _sceneID = 0;
			Shared<FrameBuffer> _sceneBuffer;
			Shared<Texture2D> _sceneTexture;

			Burst::Registery<UUID> _sceneRegistery;

			friend class Entity;
			friend class SceneManagement;
			friend class ECS;
	};

}