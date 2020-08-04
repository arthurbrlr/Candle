#pragma once

#include "Candle/CandleCore.h"
#include "OpenSimplexNoise.h"

#include <random>
#include <sstream>

namespace Candle {

    typedef const char* UUID;

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


            static std::string UUID()
            {
                static std::random_device              rd;
                static std::mt19937                    gen(rd());
                static std::uniform_int_distribution<> dis(0, 15);
                static std::uniform_int_distribution<> dis2(8, 11);

                std::stringstream ss;
                int i;
                ss << std::hex;
                for ( i = 0; i < 8; i++ ) {
                    ss << dis(gen);
                }
                //ss << "-";
                for ( i = 0; i < 4; i++ ) {
                    ss << dis(gen);
                }
                //ss << "-";
                for ( i = 0; i < 4; i++ ) {
                    ss << dis(gen);
                }
                //ss << "-";
                ss << dis2(gen);
                for ( i = 0; i < 3; i++ ) {
                    ss << dis(gen);
                }
                //ss << "-";
                for ( i = 0; i < 12; i++ ) {
                    ss << dis(gen);
                };
                return ss.str();
            }

		private:
			static OpenSimplexNoise _noise;
	};

}