#include "cdlpch.h"
#include "RenderCommands.h"

#include "platform/OpenGL/GLRendererAPI.h"

namespace Candle {

	RendererAPI* RenderCommands::_rendererAPI = new GLRendererAPI();

}