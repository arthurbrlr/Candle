#include "cdlpch.h"
#include "AudioMixer.h"

#include "platform/OpenAL/ALMixer.h"


namespace Candle {

	AudioDevice AudioMixer::_device = AudioDevice::OpenAL;
	Unique<AudioMixer> Audio::_audioAPI = AudioMixer::Create();


	Unique<AudioMixer> AudioMixer::Create()
	{
		switch (_device)
		{
			case Candle::OpenAL: return std::make_unique<ALMixer>();
		}

		CASSERT(false, "No Audio Device choosen !");
		return nullptr;
	}

}