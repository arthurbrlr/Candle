#include "cdlpch.h"
#include "ALMixer.h"

#include <fstream>


namespace Candle {


	void ALMixer::Init()
	{

		_alDevice = alcOpenDevice(NULL); // open default device
		if (_alDevice != NULL) {
			CINFO("OpenAL Device : {0}", alcGetString(_alDevice, ALC_DEVICE_SPECIFIER));
			_alContext = alcCreateContext(_alDevice, NULL); // create context
			if (_alContext != NULL) {
				alcMakeContextCurrent(_alContext); // set active context

				if (alcGetError(_alDevice) == ALC_NO_ERROR) CINFO("OpenAL Context Created");
			} else {
				CERROR("Couldn't create OpenAL Context!");
				Clear();
			}
		}

		SetListener(AudioListener());
		CTRACE("OpenAL Version : {0}", alGetString(AL_VERSION));
		CTRACE("OpenAL Renderer : {0}", alGetString(AL_RENDERER));
		CTRACE("OpenAL Vendor : {0}", alGetString(AL_VENDOR));

			// temp
		//_emitters["footsteps"] = std::make_shared<ALAudioEmitter>("res/audio/footsteps.wav");
		//_emitters["finalFantasy"] = std::make_shared<ALAudioEmitter>("res/audio/ff7r2.wav");

	}


	void ALMixer::Clear()
	{
		if (_alContext) {
			alcMakeContextCurrent(0);
			alcDestroyContext(_alContext);
			_alContext = 0;
		}

		if (_alDevice) {
			alcCloseDevice(_alDevice);
			_alDevice = 0;
		}
	}


	Shared<AudioEmitter> ALMixer::LoadSound(const std::string & filePath)
	{
		return std::make_shared<ALAudioEmitter>(filePath);
	}


	void ALMixer::SetListener(const AudioListener & listener)
	{
		alListener3f(AL_POSITION, listener.Position().x, listener.Position().y, listener.Position().z);
		alListener3f(AL_VELOCITY, listener.Velocity().x, listener.Velocity().y, listener.Velocity().z);
		alListener3f(AL_ORIENTATION, listener.Orientation().x, listener.Orientation().y, listener.Orientation().z);
	}


	void ALMixer::Play(const std::string & sourceName)
	{
		_emitters[sourceName]->Play();
	}


	void ALMixer::Pause(const std::string & sourceName)
	{
		_emitters[sourceName]->Pause();
	}


	void ALMixer::Stop(const std::string & sourceName)
	{
		_emitters[sourceName]->Stop();
	}

}