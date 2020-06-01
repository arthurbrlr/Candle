#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Audio/AudioMixer.h"

#include "ALAudioEmitter.h"


namespace Candle {

	class CANDLE_API ALMixer : public AudioMixer {
	
		public:
			ALMixer() {}

			void Init() override;
			void Clear() override;

			Shared<AudioEmitter> LoadSound(const std::string & filePath) override;

			void Play(const std::string & sourceName) override;
			void Pause(const std::string & sourceName) override;
			void Stop(const std::string & sourceName) override;

			void SetListener(const AudioListener & listener) override;

		private:
			ALCdevice* _alDevice = nullptr;
			ALCcontext* _alContext = nullptr;

			std::unordered_map<std::string, Shared<ALAudioEmitter>> _emitters;
	
	};

}