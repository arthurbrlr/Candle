#pragma once

#include <Candle.h>
using namespace Candle;

#include "TestECS.h"
#include "Scripts/RandomGridGenerator.h"

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
		Blueprint& camera = ECS::New("camera");
		camera.AddComponent<Transform>();
		camera.AddComponent<CameraHandler>(CameraType::Perspective, CDL_APP_WIDTH, CDL_APP_HEIGHT).SetAsMainCamera(true);

		Blueprint& master = ECS::New("map generator");
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