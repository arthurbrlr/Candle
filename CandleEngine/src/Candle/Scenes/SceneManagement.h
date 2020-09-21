#pragma once

#include "Candle/CandleCore.h"
#include "Scene.h"

namespace Candle {

	class SceneManagement {

		public:
			static void Init();

			static void CreateEmptyScene();
			static void LoadScene(const std::string& scenePath);
			static void SaveCurrentScene(const std::string& scenePath = "");
			static void Reload();

			static void OnUpdate();
			static void OnRender(bool useDefaultFrameBuffer = true);
			static void OnEvent(Event& event);

			static Scene* CurrentScene() { return _currentScene; }

			static Shared<Texture2D> FinalSceneTexture() { return _finalTexture; }
			static Shared<Texture2D> SceneTexture() { return _currentScene->GetTexture(); }

			static bool HasNoFilePath() { return _currentSceneFilePath == ""; }

		private:
			static Scene* _currentScene;
			static std::string _currentSceneFilePath;
			static Shared<Texture2D> _finalTexture;

			static void RenderScene();
	};
}