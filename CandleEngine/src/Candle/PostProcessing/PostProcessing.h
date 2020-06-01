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
			void SetNode(unsigned int nodeID, bool state) { _graph[nodeID]->SetState(state); }
			void SetFinalNode(unsigned int nodeID);

			unsigned int FinalNodeID() const { return _finalNodeID; }
			Shared<Texture2D> GetFinalTexture() 
			{ 
				if ( _finalNodeID == -1 ) return nullptr;
				return _graph[_finalNodeID]->GetRenderTexture(); 
			}

		private:
			glm::vec4 _renderViewport;
			unsigned int _finalNodeID = 0;

	};


	class CANDLE_API PostProcessing {

		public:
			static void Init() { _pipeline->Init(); _pipeline->Build(); }
			static void Process(Shared<Texture2D> sceneTexture) { _pipeline->Process(sceneTexture); }
			static void SetRenderViewport(const glm::vec4 & viewport) { _pipeline->SetRenderViewport(viewport); }
	 
			static void AddNode(Shared<PostProcessingNode> node) { _pipeline->AddNode(node); }
			static void SetNode(unsigned int nodeID, bool state) { _pipeline->SetNode(nodeID, state); }
			static void SetFinalNode(unsigned int nodeID) { _pipeline->SetFinalNode(nodeID); }
			
			static PostProcessingNode& GetNode(unsigned int nodeID) { return _pipeline->GetNode(nodeID); }
			static std::vector<std::string> GetAllNodesName() { return _pipeline->GetAllNodesName(); }
			static unsigned int GetFinalNodeID() { return _pipeline->FinalNodeID(); }

			static Shared<Texture2D> GetFinalTexture() { return _pipeline->GetFinalTexture(); }
			static std::unordered_map<unsigned int, Shared<PostProcessingNode>> GetPipeline() { return _pipeline->GetGraph(); }

		private:	
			static Unique<PostProcessingPipeline> _pipeline;

	};

}