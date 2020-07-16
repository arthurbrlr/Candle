#pragma once

#include "Candle/CandleCore.h"

namespace Candle {

	class RendererLayers {

		// Use that shit in the renderer plz

		public:
			static void AddLayer(int renderOrder, std::string name) { _layers[renderOrder] = name; }
			static void RemoveLayer(int renderOrder) { _layers.erase(renderOrder); }

			static std::map<int, std::string> & GetLayers() { return _layers; }

		private:
			static std::map<int, std::string> _layers;

	};

}