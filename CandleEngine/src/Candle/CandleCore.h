#pragma once

#include <memory>

enum SystemEnum {
	WIN64, MACOS, LINUX
};

#ifdef _WIN64
	static SystemEnum SYSTEM = WIN64;
	#ifdef CANDLE_DYNAMIC_LINK
		#ifdef CANDLE_BUILD_DLL
			#define	CANDLE_API _declspec(dllexport)
		#else
			#define	CANDLE_API _declspec(dllimport)
		#endif
	#else
		#define CANDLE_API
	#endif
#elif __APPLE__
	static SystemEnum SYSTEM = MACOS;
	#error Candle is only supported on windows
#elif __linux__
	static SystemEnum SYSTEM = LINUX;
	#error Candle is only supported on windows
#endif

#define BIT(x) (1 << x)

#ifndef CANDLE_RELEASE
	#define CDL_ASSERT(x, ...) { if (!(x)) { CDL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CASSERT(x, ...) { if (!(x)) { CERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CDL_ASSERT(x, ...)
	#define CASSERT(x, ...)
#endif

namespace Candle {
	
	template<typename T>
	using Unique = std::unique_ptr<T>;

	template<typename T>
	using Shared = std::shared_ptr<T>;

}