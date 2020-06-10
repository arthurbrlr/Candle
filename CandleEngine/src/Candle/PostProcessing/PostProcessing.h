#pragma once

#include "Candle/CandleCore.h"
#include "Node.h"

namespace Candle {

	class CANDLE_API PostProcessingPipeline : public Graph<PostProcessingNode> {

		public:
			void Init() override;
			void Build() override;

				// PostProcessing Specific
			void Process(Shared<Texture2D> sceneTexture);

			void SetRenderViewport(const glm::vec4 & viewport) { _renderViewport = viewport; }
			void SetNode(uint32_t nodeID, bool state) { _graph[nodeID]->SetState(state); }
			void SetFinalNode(uint32_t nodeID);

			uint32_t FinalNodeID() const { return _finalNodeID; }
			Shared<Texture2D> GetFinalTexture() 
			{ 
				if ( _finalNodeID == -1 ) return nullptr;
				return _graph[_finalNodeID]->GetRenderTexture(); 
			}

		private:
			glm::vec4 _renderViewport;
			uint32_t _finalNodeID = 0;

	};


	class CANDLE_API PostProcessing {

		public:
			static void Init() { _pipeline->Init(); _pipeline->Build(); }
			static void Process(Shared<Texture2D> sceneTexture) { _pipeline->Process(sceneTexture); }
			static void SetRenderViewport(const glm::vec4 & viewport) { _pipeline->SetRenderViewport(viewport); }
	 
			static void AddNode(Shared<PostProcessingNode> node) { _pipeline->AddNode(node); }
			static void SetNode(uint32_t nodeID, bool state) { _pipeline->SetNode(nodeID, state); }
			static void SetFinalNode(uint32_t nodeID) { _pipeline->SetFinalNode(nodeID); }
			
			static PostProcessingNode& GetNode(uint32_t nodeID) { return _pipeline->GetNode(nodeID); }
			static std::vector<std::string> GetAllNodesName() { return _pipeline->GetAllNodesName(); }
			static uint32_t GetFinalNodeID() { return _pipeline->FinalNodeID(); }

			static Shared<Texture2D> GetFinalTexture() { return _pipeline->GetFinalTexture(); }
			static std::unordered_map<uint32_t, Shared<PostProcessingNode>> GetPipeline() { return _pipeline->GetGraph(); }

		private:	
			static Unique<PostProcessingPipeline> _pipeline;

	};

}