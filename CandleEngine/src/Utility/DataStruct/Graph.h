#pragma once

#include "Candle/CandleCore.h"

#include <string>

namespace Candle {

	struct Edge {
		uint32_t source;
		uint32_t output;
	};


	class Node {
	public:
		virtual ~Node() {}

		virtual const uint32_t GetID() const { return _nodeID; }
		virtual const std::string GetName() const { return _nodeName; }

	protected:
		uint32_t _nodeID = 0;
		std::string _nodeName = "Node";
	};


	template<class GraphNode>
	class Graph {

	public:
		virtual ~Graph() {}

		virtual void Init() = 0;
		virtual void Build() = 0;

		virtual void AddNode(Shared<GraphNode> node)
		{
			_graph[node->GetID()] = std::move(node);
		}

		std::unordered_map<uint32_t, Shared<GraphNode>> GetGraph() { return _graph; }
		GraphNode& GetNode(uint32_t nodeID) { return *_graph[nodeID]; }
		std::vector<std::string> GetAllNodesName()
		{
			std::vector<std::string> allNames;

			for ( auto& it : _graph ) {
				allNames.push_back(it.second->GetName());
			}

			return allNames;
		}

	protected:
		std::unordered_map<uint32_t, Shared<GraphNode>> _graph;
	};


}
