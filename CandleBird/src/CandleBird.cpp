#include "GameScene.h"
#include <Candle/EntryPoint.h>


class GameApplication : public Candle::Application
{
public:
	GameApplication(const std::string & appName)
		: Candle::Application(appName)
	{
		Candle::ECS::Add(new RenderSystem());
		SceneManagement::AddScene(new MenuScene());
		SceneManagement::AddScene(new GameScene());
		SceneManagement::LoadScene(gameSceneID);
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