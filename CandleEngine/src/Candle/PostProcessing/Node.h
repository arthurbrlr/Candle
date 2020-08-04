#pragma once

#include "Candle/CandleCore.h"

#include "Utility/DataStruct/Graph.h"
#include "Candle/Assets/Assets.h"
#include "Candle/Renderer/FrameBuffer.h"

namespace Candle {

	static uint32_t postProcessingNodeID = 0;

	struct NodeData {
		std::vector<uint32_t> inputs;
		std::vector<uint32_t> outputs;
	};


	class PostProcessingNode : public Node {

		public:

			PostProcessingNode(const std::string & name, uint32_t width, uint32_t height, NodeData data, const std::vector<FrameBufferType> & bufferTypes);

			static Shared<PostProcessingNode> New(const std::string & name, uint32_t width, uint32_t height, NodeData data, const std::vector<FrameBufferType> & bufferTypes = { FrameBufferType::Texture })
			{
				return std::make_shared<PostProcessingNode>(name, width, height, data, bufferTypes);
			}


			void ResetNode();
			void UploadTexture(Shared<Texture2D> texture);
			void SetState(bool state) { _used = state; }

			Shared<Texture2D> GetRenderTexture() const { return _renderTexture; }
			bool HasInput(uint32_t nodeID) const
			{
				for (auto& it : _data.inputs) if (it == nodeID) return true;
				return false;
			}

			const NodeData Data() const { return _data; }
			bool IsUsed() const { return _used; }
			bool IsValid() const { return _valid; }


		private:
			bool _used = true;
			bool _valid = false;
			NodeData _data;
		
			std::vector<Shared<Texture2D>> _inputTextures;
			int _requiredInputs = 1;
			int _receivedInputs = 0;

			Shared<FrameBuffer> _buffer = nullptr;
			Shared<Texture2D> _renderTexture = nullptr;
			uint32_t _textureWidth, _textureHeight;
	};

}