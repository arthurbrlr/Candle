#pragma once

#include "Candle/CandleCore.h"
#include "Scene.h"

namespace Candle {

	class SceneManagement {

		public:
			static void Init();
			static void AddScene(Scene* scene);

			static void LoadScene(uint32_t sceneID);
			static void OnUpdate();
			static void OnRender();
			static void OnEditorRender();
			static void OnEvent(Event& event);

			static Shared<Scene> CurrentScene() { return _scenes[_currentScene]; }
			static Shared<Texture2D> FinalSceneTexture() { return _finalTexture; }
			static Shared<Texture2D> SceneTexture() { return _scenes[_currentScene]->GetTexture(); }
			static std::unordered_map<uint32_t, Shared<Scene>>& AllScenes() { return _scenes; }

		private:
			static std::unordered_map<uint32_t, Shared<Scene>> _scenes;
			static uint32_t _currentScene;
			static Shared<Texture2D> _finalTexture;

			static void RenderScene();
	};
}