#pragma once

#include "Candle/CandleCore.h"
#include "OpenSimplexNoise.h"

#include <random>
#include <sstream>

namespace Candle {

	class Noise {

		public:
			static double Random()
			{
				std::random_device dev;
				std::mt19937 rng(dev());
				std::uniform_real_distribution<> rand(0, 1);
				return rand(rng);
			}


			static double Random(double min, double max)
			{
				std::random_device dev;
				std::mt19937 rng(dev());
				std::uniform_real_distribution<> rand(min, max);
				return rand(rng);
			}


			static int RandomInt(int min, int max)
			{
				std::random_device dev;
				std::mt19937 rng(dev());
				std::uniform_int_distribution<> rand(min, max);
				return rand(rng);
			}

		private:
			static OpenSimplexNoise _noise;
	};

}