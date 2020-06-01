#include <cdlpch.h>

#include "Logger.h"

namespace Candle {

	Shared<spdlog::logger> Logger::_coreLogger;
	Shared<spdlog::logger> Logger::_clientLogger;


	Logger::Logger()
	{

	}


	Logger::~Logger()
	{

	}

	
	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		_coreLogger = spdlog::stdout_color_mt("CandleEngine");
		_coreLogger->set_level(spdlog::level::trace);
		_clientLogger = spdlog::stdout_color_mt("Application");
		_clientLogger->set_level(spdlog::level::trace);
	}

}