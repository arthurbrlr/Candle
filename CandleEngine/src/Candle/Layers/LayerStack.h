#pragma once

#include "Candle/CandleCore.h"
#include "Layer.h"

namespace Candle {

	class LayerStack {
	
		public:
			LayerStack();
			~LayerStack();

			void PushLayer(Layer* layer);
			void PushOverlay(Layer* overlay);
			void PopLayer(Layer* layer);
			void PopOverlay(Layer* overlay);

			std::vector<Layer*>::iterator begin() { return _layers.begin(); }
			std::vector<Layer*>::iterator end() { return _layers.end(); }

		private:
			std::vector<Layer*> _layers;
			uint32_t _layerInsert = 0;

	};

}

