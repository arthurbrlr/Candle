#include "Candle/Core/FileSystem.h"

#ifdef CANDLE_PLATFORM_WINDOWS

#include <Windows.h>

namespace Candle {

    std::string FileSystem::StringFromWChar(const wchar_t* toConvert)
    {
        size_t origsize = wcslen(toConvert) + 1;
        size_t convertedChars = 0;
        char converted[256] = { 0 };
        wcstombs_s(&convertedChars, converted, origsize, toConvert, _TRUNCATE);

        return std::string(converted);
    }


    void FileSystem::GetExecutablePath(std::string& executablePath)
    {
        wchar_t pBuf[256] = { 0 };
        size_t len = 256;

#ifdef CANDLE_PLATFORM_WINDOWS
        int bytes = GetModuleFileName(NULL, pBuf, len);
#else
            // Linux
        int bytes = MIN(readlink("/proc/self/exe", pBuf, len), len - 1);
        if ( bytes >= 0 )
            pBuf[bytes] = '\0';
        CTRACE(pBuf);
#endif
        executablePath = FileSystem::StringFromWChar(pBuf);
        size_t lastBackSlash = executablePath.find_last_of('\\');
        executablePath = executablePath.substr(0, lastBackSlash);
    }


	bool FileSystem::OpenFile(std::string& filePath, const wchar_t* fileExtension, const wchar_t* dialogFilter)
	{
        OPENFILENAME ofn;
        wchar_t fileName[MAX_PATH] = L"";
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = NULL;
        ofn.lpstrFilter = dialogFilter;
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        ofn.lpstrDefExt = fileExtension;

        if ( GetOpenFileName(&ofn) ) {
            filePath = StringFromWChar(fileName);
            return true;
        }
        return false;
	}


    bool FileSystem::SaveFile(std::string& filePath, const wchar_t* fileExtension)
    {
        OPENFILENAME ofn;
        wchar_t fileName[MAX_PATH] = L"";
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = NULL;
        ofn.lpstrFilter = L"All Files\0*.*\0";
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        ofn.lpstrDefExt = fileExtension;

        if ( GetSaveFileName(&ofn) ) {
            filePath = StringFromWChar(fileName);
            return true;
        }
        return false;
    }

}

#endif