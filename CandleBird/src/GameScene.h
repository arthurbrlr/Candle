#pragma once

#include <Candle.h>
using namespace Candle;

#include "TestECS.h"
#include "Scripts/PlayerScript.h"
#include "Scripts/MouseClickScript.h"
#include "Scripts/RandomMovementAgent.h"

static uint32_t gameSceneID;
static uint32_t menuSceneID;

class GameScene : public Scene {

	public:
		GameScene() : Scene()
		{
			gameSceneID = _sceneID;
		}

		void Load() override 
		{
			Blueprint& player = ECS::New("playerBlueprint");
			player.AddComponent<SpriteRenderer>(AssetManager::GetTexture2D("dodo"));
			player.AddComponent<Transform>();
			//player.AddScript<Gravity>();
			player.AddScript<RandomMovementAgent>();

			Blueprint& child = ECS::New("children");
			child.AddComponent<SpriteRenderer>(AssetManager::GetTexture2D("tile")).SetColor({ 1, 1, 0, 1 });
			child.AddComponent<Transform>( glm::vec3(1, 1, 0) );
			player.AddChild(&child);

			Blueprint& child2 = ECS::New("other_children");
			child2.AddComponent<SpriteRenderer>(AssetManager::GetTexture2D("tile")).SetColor({ 1, 0, 1, 1 });
			child2.AddComponent<Transform>(glm::vec3(-1, -1, 0));
			player.AddChild(&child2);

			Blueprint& child_of_child = ECS::New("children_of_children");
			child_of_child.AddComponent<SpriteRenderer>(AssetManager::GetTexture2D("tile")).SetColor({ 0, 0, 1, 1 });
			child_of_child.AddComponent<Transform>(glm::vec3(-2, -1, 0));
			child.AddChild(&child_of_child);
			
			Blueprint& tiles = ECS::New("Tiles");
			int worldSize = 3;
			for ( int i = 0; i < worldSize + 1; i++ ) {
				for ( int j = 0; j < worldSize + 1; j++ ) {
					std::string tileName = "tile_" + std::to_string(i) + "_" + std::to_string(j);
					Blueprint& tile = ECS::New(tileName);
					tile.AddComponent<Transform>(glm::vec3(i - worldSize / 2., j - worldSize / 2., 0), glm::vec3(0, 0, 0), glm::vec3(1.0, 0.5, 0.0));
					tile.AddComponent<SpriteRenderer>(AssetManager::GetTexture2D("tile")).SetColor({ Noise::RandomInt(0, 255) / 255., Noise::RandomInt(0, 255) / 255., Noise::RandomInt(0, 255) / 255., 1 });
					tiles.AddChild(&tile);
				}
			}

			Blueprint& master = ECS::New("gameMaster");
			master.AddComponent<Transform>(glm::vec3(0, 0, 5));
			//master.AddComponent<CameraHandler>(CameraType::Perspective, CDL_APP_WIDTH, CDL_APP_HEIGHT).SetAsMainCamera(true);
			master.AddComponent<CameraHandler>(CameraType::Orthographic, 10 * 16. / 9., 10).SetAsMainCamera(true);
			master.AddScript<MouseClick>(&player);
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