#include "cdlpch.h"
#include "Scene.h"

#include "Candle/Application.h"

namespace Candle {

	Scene::Scene()
	{
		_sceneID = sceneID++;
		_sceneBuffer = Candle::FrameBuffer::Create({ Candle::FrameBufferType::Texture, Candle::FrameBufferType::DepthBuffer }, CDL_APP_WIDTH, CDL_APP_HEIGHT);
		_sceneBuffer->Unbind(CDL_APP_WIDTH, CDL_APP_HEIGHT);

		_sceneTexture = Candle::Texture2D::Create(_sceneBuffer->Get(Candle::FrameBufferType::Texture), CDL_APP_WIDTH, CDL_APP_HEIGHT);
	}

}