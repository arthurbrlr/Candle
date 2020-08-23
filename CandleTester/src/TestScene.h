#pragma once

#include <Candle.h>
using namespace Candle;

#include "TestECS.h"
#include "Scripts/RandomGridGenerator.h"
#include "Scripts/UUIDTest.h"

static uint32_t testSceneID;

class TestScene : public Scene {

public:
	TestScene() : Scene()
	{
		testSceneID = _sceneID;
		_sceneName = "TestScene";
	}

	void Load() override
	{
		Entity camera = ECS::New("camera");
		camera.AddComponent<Transform>(glm::vec3{ 0, 0, 10 });
		camera.AddComponent<CameraHandler>(CameraType::Perspective, CDL_APP_WIDTH, CDL_APP_HEIGHT).SetAsMainCamera(true);

		Entity master = ECS::New("map generator");
		master.AddScript<RandomGridGenerator>();
	}


	void OnUpdate() override
	{
	}


	void OnEditor() override
	{
	}


private:


};