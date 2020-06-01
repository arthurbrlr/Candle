#include "cdlpch.h"
#include "RendererLayers.h"

namespace Candle {

	std::map<int, std::string> RendererLayers::_layers = {
		{ -1, "Background" },
		{  0, "Default" },
		{  1, "Foreground" }
	};

	// Take into account rendererLayers into the renderer
	// Render from back to front

}