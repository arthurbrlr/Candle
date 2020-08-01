#include <Candle.h>
#include <Candle/EntryPoint.h>

class GameApplication : public Candle::Application {
public:
	GameApplication(const std::string& appName)
		: Candle::Application(appName)
	{
	}

	~GameApplication()
	{

	}

private:
};


Candle::Application* Candle::CreateApplication()
{
	return new GameApplication("CandleTemplate");
}