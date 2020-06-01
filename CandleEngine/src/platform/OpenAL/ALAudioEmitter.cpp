#include "cdlpch.h"
#include "ALAudioEmitter.h"

#include "Utility/System/AudioLoader.h"


namespace Candle {

	ALAudioEmitter::ALAudioEmitter(const std::string & filePath)
	{
		// Generate an AL Buffer
		alGenBuffers(1, &_sourceBuffer);

		// WAV Reading
		{
			WavLoader loader(filePath);

			// OpenAL values from WAV file
			_frequency = loader.sampleRate;

			if (loader.bitsPerSample == 8 && loader.channels == 1) _format = AL_FORMAT_MONO8;
			if (loader.bitsPerSample == 8 && loader.channels == 2) _format = AL_FORMAT_STEREO8;
			if (loader.bitsPerSample == 16 && loader.channels == 1) _format = AL_FORMAT_MONO16;
			if (loader.bitsPerSample == 16 && loader.channels == 2) _format = AL_FORMAT_STEREO16;

			alBufferData(_sourceBuffer, _format, loader.wavData, loader.dataSize, _frequency);
		}

		// Generate a Source to playback the Buffer
		alGenSources(1, &_sourceID);

		// Attach Source to Buffer
		alSourcei(_sourceID, AL_BUFFER, _sourceBuffer);
		
		Update();
	}

	ALAudioEmitter::ALAudioEmitter(const std::string & filePath, bool loop, double pitch, double gain, const glm::vec3 & position, const glm::vec3 velocity)
	{
		// Generate an AL Buffer
		alGenBuffers(1, &_sourceBuffer);

		// WAV Reading
		{
			WavLoader loader(filePath);

			// OpenAL values from WAV file
			_frequency = loader.sampleRate;

			if (loader.bitsPerSample == 8 && loader.channels == 1) _format = AL_FORMAT_MONO8;
			if (loader.bitsPerSample == 8 && loader.channels == 2) _format = AL_FORMAT_STEREO8;
			if (loader.bitsPerSample == 16 && loader.channels == 1) _format = AL_FORMAT_MONO16;
			if (loader.bitsPerSample == 16 && loader.channels == 2) _format = AL_FORMAT_STEREO16;

			alBufferData(_sourceBuffer, _format, loader.wavData, loader.dataSize, _frequency);

			_loop = loop;
			_pitch = pitch;
			_gain = gain;
			_position = position;
			_velocity = velocity;
		}

		// Generate a Source to playback the Buffer
		alGenSources(1, &_sourceID);

		// Attach Source to Buffer
		alSourcei(_sourceID, AL_BUFFER, _sourceBuffer);
		
		Update();
	}


	ALAudioEmitter::~ALAudioEmitter()
	{
		alDeleteSources(1, &_sourceID);
		alDeleteBuffers(1, &_sourceBuffer);
	}


	void ALAudioEmitter::Update()
	{
		alSourcei(_sourceID, AL_LOOPING, _loop);
		alSourcef(_sourceID, AL_PITCH, _pitch);
		alSourcef(_sourceID, AL_GAIN, _gain);
		alSourcefv(_sourceID, AL_POSITION, glm::value_ptr(_position));
		alSourcefv(_sourceID, AL_VELOCITY, glm::value_ptr(_velocity));
	}
	
}