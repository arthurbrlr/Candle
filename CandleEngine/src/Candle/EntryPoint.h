#pragma once

#ifdef CANDLE_PLATFORM_WINDOWS

extern Candle::Application* Candle::CreateApplication();
#ifndef _DEBUG
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
int main (int argc, char** argv)
{
	Candle::Logger::Init();

	auto app = Candle::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif