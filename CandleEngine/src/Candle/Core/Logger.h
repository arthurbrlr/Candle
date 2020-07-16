#pragma once

#include "Candle/CandleCore.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Candle {

	class Logger
	{
		public:
			Logger();
			~Logger();

			static void Init();

			inline static std::shared_ptr<spdlog::logger> & GetCoreLogger() { return _coreLogger; };
			inline static std::shared_ptr<spdlog::logger> & GetClientLogger() { return _clientLogger; };

		private:
			static std::shared_ptr<spdlog::logger> _coreLogger;
			static std::shared_ptr<spdlog::logger> _clientLogger;

	};
}


#define CDL_TRACE(...)		::Candle::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define CDL_INFO(...)		::Candle::Logger::GetClientLogger()->info(__VA_ARGS__)
#define CDL_WARN(...)		::Candle::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define CDL_ERROR(...)		::Candle::Logger::GetClientLogger()->error(__VA_ARGS__)

#ifdef CANDLE_RELEASE
	#define CTRACE(...)
	#define CINFO(...)	
	#define CWARN(...)	
	#define CERROR(...)
#else
	#define CTRACE(...) ::Candle::Logger::GetCoreLogger()->trace(__VA_ARGS__)
	#define CINFO(...)	::Candle::Logger::GetCoreLogger()->info(__VA_ARGS__)
	#define CWARN(...)	::Candle::Logger::GetCoreLogger()->warn(__VA_ARGS__)
	#define CERROR(...) ::Candle::Logger::GetCoreLogger()->error(__VA_ARGS__)
#endif