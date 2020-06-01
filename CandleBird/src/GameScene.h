#pragma once

#include <Candle.h>
using namespace Candle;

#include "TestECS.h"
#include "Scripts/PlayerScript.h"
#include "Scripts/MouseClickScript.h"
#include "Scripts/RandomMovementAgent.h"

static unsigned int gameSceneID;
static unsigned int menuSceneID;

class GameScene : public Scene {

	public:
		GameScene() : Scene()
		{
			gameSceneID = _sceneID;
		}

		void Load() override 
		{
			/*
			Blueprint& player = ECS::New("playerBlueprint");
			player.AddComponent<SpriteRenderer>(AssetManager::GetTexture2D("dodo"));
			player.AddComponent<Transform>();
			//player.AddScript<Gravity>();
			player.AddScript<RandomMovementAgent>();
			*/

			Blueprint& master = ECS::New("gameMaster");
			master.AddComponent<Transform>(glm::vec3(0, 0, 10));
			master.AddComponent<CameraHandler>(CameraType::Perspective, CDL_APP_WIDTH, CDL_APP_HEIGHT).SetAsMainCamera(true);

			Blueprint& cursor = ECS::New("cursor");
			cursor.AddComponent<Transform>();
			cursor.AddComponent<SpriteRenderer>();
			cursor.AddScript<MouseClick>();
			
			
		}


		void OnUpdate() override
		{
			if (Input::IsKeyPressed(CDL_KEY_N))
				SceneManagement::LoadScene(menuSceneID);

			static double timer = 0;
			timer -= Time::FixedDeltaTime();

			if ( Input::IsKeyPressed(CDL_KEY_F) && timer < 0 ) {
				Application::SetFullScreen(!Application::IsFullScreen());
				timer = 0.5;
			}
		}


		void OnEditor() override
		{
			ImGui::Begin("GameScene");
			{
				ImGui::Text("Test");
			}
			ImGui::End();
		}


	private:
		

};


class MenuScene : public Scene {

	public:
		MenuScene() : Scene()
		{
			menuSceneID = _sceneID;
		}

		void Load() override 
		{
			Blueprint& menuBg = ECS::New("menuBg");
			menuBg.AddComponent<SpriteRenderer>(AssetManager::GetTexture2D("tt"));

		}


		void OnUpdate() override
		{
			if (Input::IsKeyPressed(CDL_KEY_P))
				SceneManagement::LoadScene(gameSceneID);
		}

};