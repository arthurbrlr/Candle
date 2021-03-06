#include "cdlpch.h"
#include "PostProcessing.h"
#include "Candle/Renderer/Renderer2D.h"

namespace Candle {


	void PostProcessingPipeline::Init()
	{
		NodeData brigthFilterData = {
			{0},
			{1}
		};
		AddNode(PostProcessingNode::New("brigthFilter", 960, 540, brigthFilterData), _usePostProcessing);

		NodeData verticalBlurData = {
			{1},
			{2}
		};
		AddNode(PostProcessingNode::New("verticalBlur", 960, 540, verticalBlurData), _usePostProcessing);

		NodeData horizontalBlurData = {
			{2},
			{3}
		};
		AddNode(PostProcessingNode::New("horizontalBlur", 960, 540, horizontalBlurData), _usePostProcessing);

		NodeData bloomData = {
			{0, 3},
			{}
		};
		AddNode(PostProcessingNode::New("bloom", 1920, 1080, bloomData), _usePostProcessing);
	}


	void PostProcessingPipeline::Build()
	{
		uint32_t finalNodeId = 0;
		for (auto& it : _graph) {
			if (it.second->Data().outputs.size() == 0) _finalNodeID = it.second->GetID();
		}
	}


	void PostProcessingPipeline::AddNode(Shared<PostProcessingNode> node, bool& success)
	{
		success = node->IsValid();
		_graph[node->GetID()] = std::move(node);
	}


	void PostProcessingPipeline::Process(Shared<Texture2D> sceneTexture)
	{
		if ( !_usePostProcessing ) return;
		for (auto& it : _graph) {
			it.second->ResetNode();
		}

		for (auto& it : _graph) {
			if (it.second->HasInput(0)) it.second->UploadTexture(sceneTexture);
		}
		
	}


	void PostProcessingPipeline::SetFinalNode(uint32_t nodeID)
	{
		_finalNodeID = nodeID;
	}

	Unique<PostProcessingPipeline> PostProcessing::_pipeline = std::make_unique<PostProcessingPipeline>();



}