#pragma once

#include "Candle/CandleCore.h"
#include "Node.h"

namespace Candle {

	class PostProcessingPipeline : public Graph<PostProcessingNode> {

		public:
			void Init() override;
			void Build() override;

			void AddNode(Shared<PostProcessingNode> node, bool& success);

				// PostProcessing Specific
			void Process(Shared<Texture2D> sceneTexture);

			void SetRenderViewport(const glm::vec4 & viewport) { _renderViewport = viewport; }
			void SetNode(uint32_t nodeID, bool state) { _graph[nodeID]->SetState(state); }
			void SetFinalNode(uint32_t nodeID);

			uint32_t FinalNodeID() const { return _finalNodeID; }
			Shared<Texture2D> GetFinalTexture() 
			{ 
				if ( !UsePostProcessing() ) return nullptr;
				return _graph[_finalNodeID]->GetRenderTexture(); 
			}

			bool UsePostProcessing()
			{
				return ( _finalNodeID != -1 && _usePostProcessing );
			}

		private:
			glm::vec4 _renderViewport;
			uint32_t _finalNodeID = 0;
			bool _usePostProcessing = true;

	};


	class PostProcessing {

		public:
			static void Init() { _pipeline->Init(); _pipeline->Build(); }
			static void Process(Shared<Texture2D> sceneTexture) { _pipeline->Process(sceneTexture); }
			static void SetRenderViewport(const glm::vec4 & viewport) { _pipeline->SetRenderViewport(viewport); }
	 
			static void AddNode(Shared<PostProcessingNode> node, bool& validNode) { _pipeline->AddNode(node, validNode); }
			static void SetNode(uint32_t nodeID, bool state) { _pipeline->SetNode(nodeID, state); }
			static void SetFinalNode(uint32_t nodeID) { _pipeline->SetFinalNode(nodeID); }
			
			static PostProcessingNode& GetNode(uint32_t nodeID) { return _pipeline->GetNode(nodeID); }
			static std::vector<std::string> GetAllNodesName() { return _pipeline->GetAllNodesName(); }
			static uint32_t GetFinalNodeID() { return _pipeline->FinalNodeID(); }

			static Shared<Texture2D> GetFinalTexture() { return _pipeline->GetFinalTexture(); }
			static std::unordered_map<uint32_t, Shared<PostProcessingNode>> GetPipeline() { return _pipeline->GetGraph(); }

			static bool Use() { return _pipeline->UsePostProcessing(); }

		private:	
			static Unique<PostProcessingPipeline> _pipeline;

	};

}