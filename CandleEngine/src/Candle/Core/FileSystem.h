#pragma once

#include <string>

namespace Candle {

	class FileSystem {
	public:
		static std::string StringFromWChar(const wchar_t* toConvert);

		static void GetExecutablePath(std::string& executablePath);
		static bool OpenFile(std::string& filePath, const wchar_t* fileExtension, const wchar_t* dialogFilter = L"All Files\0*.*\0");
		static bool SaveFile(std::string& filePath, const wchar_t* fileExtension);

	};

}