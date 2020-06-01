#pragma once

#include "Candle/CandleCore.h"
#include "AudioEmitter.h"
#include "AudioListener.h"

namespace Candle {

	enum AudioDevice {
		OpenAL
	};

	class CANDLE_API AudioMixer {

		public:
			virtual ~AudioMixer() {}

			virtual void Init() = 0;
			virtual void Clear() = 0;

			virtual Shared<AudioEmitter> LoadSound(const std::string & filePath) = 0;
			virtual void Play(const std::string & sourceName) = 0;
			virtual void Pause(const std::string & sourceName) = 0;
			virtual void Stop(const std::string & sourceName) = 0;

			virtual void SetListener(const AudioListener & listener) = 0;

			static Unique<AudioMixer> Create();

		protected:
			static AudioDevice _device;
			glm::vec3 _listenerPosition = { 0, 0, 0 };
	};


	class CANDLE_API Audio {

		public:
			static void Init() { _audioAPI->Init(); }
			static void Clear() { _audioAPI->Clear(); }

			static Shared<AudioEmitter> LoadSound(const std::string & filePath) { return _audioAPI->LoadSound(filePath); }
			static void Play(const std::string & sourceName) { _audioAPI->Play(sourceName); }
			static void Pause(const std::string & sourceName) { _audioAPI->Pause(sourceName); }
			static void Stop(const std::string & sourceName) { _audioAPI->Stop(sourceName); }

			static void SetListener(const AudioListener & listener) { _audioAPI->SetListener(listener); }

		private:
			static Unique<AudioMixer> _audioAPI;

	};

}