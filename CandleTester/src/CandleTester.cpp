#include <Candle.h>
#include <Candle/EntryPoint.h>

#include "Scripts/PlayerScript.h"
#include "Scripts/MouseClickScript.h"
#include "Scripts/RandomMovementAgent.h"
#include "Scripts/RandomGridGenerator.h"
#include "Scripts/UUIDTest.h"
#include "Scripts/VectorTests.h"

class GameApplication : public Candle::Application
{
public:
	GameApplication(const std::string & appName)
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
	return new GameApplication("CandleTester");
}