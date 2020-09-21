#include <Candle.h>
#include <Candle/EntryPoint.h>

#include "EditorLayer.h"

class CandleEditor : public Candle::Application {
public:
	CandleEditor(const std::string& appName)
		: Candle::Application(appName)
	{
		PushLayer(new EditorLayer());
	}

	~CandleEditor()
	{

	}

private:
};


Candle::Application* Candle::CreateApplication()
{
	return new CandleEditor("CandleEditor");
}