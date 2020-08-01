#include "GameScene.h"
#include "TestScene.h"
#include <Candle/EntryPoint.h>


class GameApplication : public Candle::Application
{
public:
	GameApplication(const std::string & appName)
		: Candle::Application(appName)
	{
		Candle::ECS::Add(new RenderSystem()); // temporary
		Candle::ECS::Add(new LineRenderSystem()); // temporary
		SceneManagement::AddScene(new GameScene());
		SceneManagement::AddScene(new TestScene());
		SceneManagement::LoadScene(testSceneID);
	}

	~GameApplication()
	{

	}

private:
};


Candle::Application* Candle::CreateApplication()
{
	return new GameApplication("CandleBird");
}