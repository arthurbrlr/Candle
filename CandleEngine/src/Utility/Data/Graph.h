#pragma once

#include "Candle/CandleCore.h"

#include <string>

namespace Candle {

	struct Edge {
		unsigned int source;
		unsigned int output;
	};


	class CANDLE_API Node {
	public:
		virtual ~Node() {}

		virtual const unsigned int GetID() const { return _nodeID; }
		virtual const std::string GetName() const { return _nodeName; }

	protected:
		unsigned int _nodeID = 0;
		std::string _nodeName = "Node";
	};


	template<class GraphNode>
	class CANDLE_API Graph {

	public:
		virtual ~Graph() {}

		virtual void Init() = 0;
		virtual void Build() = 0;

		virtual void AddNode(Shared<GraphNode> node)
		{
			_graph[node->GetID()] = std::move(node);
		}

		std::unordered_map<unsigned int, Shared<GraphNode>> GetGraph() { return _graph; }
		GraphNode& GetNode(unsigned int nodeID) { return *_graph[nodeID]; }
		std::vector<std::string> GetAllNodesName()
		{
			std::vector<std::string> allNames;

			for ( auto& it : _graph ) {
				allNames.push_back(it.second->GetName());
			}

			return allNames;
		}

	protected:
		std::unordered_map<unsigned int, Shared<GraphNode>> _graph;
	};


}
