#pragma once

#include "Candle/CandleCore.h"

#include "Candle/Events/Event.h"
#include "Candle/Renderer/FrameBuffer.h"
#include "Candle/Assets/Texture2D.h"

namespace Candle {

	static unsigned int sceneID = 0;


	class CANDLE_API Scene {

		public:
			Scene();
			virtual ~Scene() {}

			virtual void Load() = 0;
			virtual void Unload() {}

			virtual void OnUpdate() = 0;
			virtual void OnEvent(Event & event) {}
			virtual void OnEditor() {}

			Shared<FrameBuffer> & GetFrameBuffer() { return _sceneBuffer; }
			Shared<Texture2D> GetTexture() { return _sceneTexture; }
			unsigned int GetID() { return _sceneID; }

		protected:
			unsigned int _sceneID = 0;
			Shared<FrameBuffer> _sceneBuffer;
			Shared<Texture2D> _sceneTexture;

	};

}