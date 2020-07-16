#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Audio/AudioEmitter.h"

#include <al.h>
#include <alc.h>


namespace Candle {

	// S�parer AudioEmitter (source) et le buffer (buffer) puis cr�er une classe AudioBuffer
	// De cette fa�on on peut avoir plusieurs �m�teurs qui utilisent le m�me son

	class ALAudioEmitter : public AudioEmitter {

		public:
			ALAudioEmitter(const std::string & filePath);
			ALAudioEmitter(const std::string & filePath, bool loop, double pitch, double gain, const glm::vec3 & position, const glm::vec3 velocity);

			~ALAudioEmitter();

			AudioEmitter& SetBuffer(Shared<AudioBuffer> buffer) override
			{
				alSourcei(_sourceID, AL_BUFFER, buffer->GetBufferID());
				return *this;
			}

			void Play() override
			{
				alSourcePlay(_sourceID);
			}

			void Pause() override
			{
				alSourcePause(_sourceID);
			}

			void Stop() override
			{
				alSourceStop(_sourceID);
			}


		private:
			uint32_t _sourceBuffer = 0;
			uint32_t _sourceID = 0;
			uint32_t _frequency = 0;
			ALenum _format = 0;

			void Update() override;
	};


}