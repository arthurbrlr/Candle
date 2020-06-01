#pragma once

#include "Candle/CandleCore.h"

#include "Utility/Data/Graph.h"
#include "Candle/Assets/AssetManager.h"
#include "Candle/Renderer/FrameBuffer.h"

namespace Candle {

	static unsigned int postProcessingNodeID = 0;

	struct NodeData {
		std::vector<unsigned int> inputs;
		std::vector<unsigned int> outputs;
	};


	class CANDLE_API PostProcessingNode : public Node {

		public:

			PostProcessingNode(const std::string & name, unsigned int width, unsigned int height, NodeData data, const std::vector<FrameBufferType> & bufferTypes);

			static Shared<PostProcessingNode> New(const std::string & name, unsigned int width, unsigned int height, NodeData data, const std::vector<FrameBufferType> & bufferTypes = { FrameBufferType::Texture })
			{
				return std::make_shared<PostProcessingNode>(name, width, height, data, bufferTypes);
			}


			void ResetNode();
			void UploadTexture(Shared<Texture2D> texture);
			void SetState(bool state) { _used = state; }

			Shared<Texture2D> GetRenderTexture() const { return _renderTexture; }
			bool HasInput(unsigned int nodeID) const
			{
				for (auto& it : _data.inputs) if (it == nodeID) return true;
				return false;
			}

			const NodeData Data() const { return _data; }
			bool IsUsed() const { return _used; }


		private:
			bool _used = true;
			NodeData _data;
		
			std::vector<Shared<Texture2D>> _inputTextures;
			int _requiredInputs = 1;
			int _receivedInputs = 0;

			Shared<FrameBuffer> _buffer = nullptr;
			Shared<Texture2D> _renderTexture = nullptr;
			unsigned int _textureWidth, _textureHeight;
	};

}