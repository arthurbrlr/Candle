#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Core/Logger.h"

namespace Candle {

	struct WavLoader {

		char type[4];
		unsigned long size, chunkSize;
		short formatType, channels;

		unsigned long sampleRate, avgBytesPerSec;
		short bytesPerSample, bitsPerSample;

		unsigned long dataSize;
		unsigned char* wavData;

		WavLoader(const std::string & filePath)
		{
			FILE* file = NULL;
			file = fopen(filePath.c_str(), "rb");

			if (file == NULL) return;

			fread(type, sizeof(char), 4, file);
			if (type[0] != 'R' || type[1] != 'I' || type[2] != 'F' || type[3] != 'F') {
				CASSERT(false, "File has no Riff");
			}
			fread(&size, sizeof(unsigned long), 1, file);

			fread(type, sizeof(char), 4, file);
			if (type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E') {
				CASSERT(false, "File is not .WAV");
			}

			fread(type, sizeof(char), 4, file);
			if (type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ') {
				CASSERT(false, "File has no fmt");
			}

			fread(&chunkSize, sizeof(unsigned long), 1, file);
			fread(&formatType, sizeof(short), 1, file);
			fread(&channels, sizeof(short), 1, file);
			fread(&sampleRate, sizeof(unsigned long), 1, file);
			fread(&avgBytesPerSec, sizeof(unsigned long), 1, file);
			fread(&bytesPerSample, sizeof(short), 1, file);
			fread(&bitsPerSample, sizeof(short), 1, file);

			fread(type, sizeof(char), 4, file);
			if (type[0] != 'd' || type[1] != 'a' || type[2] != 't' || type[3] != 'a') {
				CASSERT(false, "Missing WAV Data");
			}

			fread(&dataSize, sizeof(unsigned long), 1, file);

			wavData = new unsigned char[dataSize];
			fread(wavData, sizeof(unsigned char), dataSize, file);

			fclose(file);
		}

	};

}