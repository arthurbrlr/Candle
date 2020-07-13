#include "cdlpch.h"
#include "Node.h"

#include "Candle/Application.h"
#include "Candle/Core/Time.h"
#include "Candle/Renderer/Renderer2D.h"
#include "Candle/Renderer/RenderCommands.h"
#include "PostProcessing.h"

namespace Candle {


	PostProcessingNode::PostProcessingNode(const std::string & name, uint32_t width, uint32_t height, NodeData data, const std::vector<FrameBufferType> & bufferTypes)
		: _textureWidth(width), _textureHeight(height), _data(data)
	{

		int textureSamplers[4] = { 0, 1, 2, 3 };
		Shared<Shader> shader = Assets::GetShader(name);
		if (shader != nullptr) {
			shader->Bind();
			shader->SetIntArray("u_textures", textureSamplers, 4);
		}
		
		_nodeID = postProcessingNodeID++;
		_nodeName = name;
		_buffer = FrameBuffer::Create(bufferTypes, width, height);

		/*
		for (auto& type : bufferTypes) {
			_renderTextures[type] = Texture2D::Create(_buffer->Get(type), width, height);
		}
		*/
		_renderTexture = Texture2D::Create(_buffer->Get(FrameBufferType::Texture), width, height);

		_requiredInputs = (int)data.inputs.size();
	}



	void PostProcessingNode::UploadTexture(Shared<Texture2D> texture)
	{
		_inputTextures.push_back(texture);

		if (++_receivedInputs == _requiredInputs) {

			if (_used) {

				_buffer->Bind();

				Shared<Shader> nodeShader = Assets::GetShader(_nodeName);
				if (nodeShader != nullptr) {
					nodeShader->Bind();
					nodeShader->SetFloat2("u_winDim", { _textureWidth, _textureHeight });
					nodeShader->SetFloat("u_time", (float)Time());
				}

				for (int i = 0; i < _inputTextures.size(); i++) {
					_inputTextures[i]->Bind(i);
				}

				Renderer2D::DrawFullScreenQuad();

				_buffer->Unbind(CDL_APP_WIDTH, CDL_APP_HEIGHT);

			}

			if (PostProcessing::GetFinalNodeID() == _nodeID) {
				return;
			}

			for (auto& id : _data.outputs) {
				PostProcessing::GetNode(id).UploadTexture( _used ? _renderTexture : texture);
			}

		}


	}


	void PostProcessingNode::ResetNode()
	{
		_receivedInputs = 0;
		_inputTextures.clear();
	}

}