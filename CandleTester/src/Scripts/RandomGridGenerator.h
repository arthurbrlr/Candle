#pragma once

#include <Candle.h>
using namespace Candle;

class RandomGridGenerator : public Script {
public:
	void OnAwake() override
	{
	}

	void OnUpdate() override
	{
	}


	void OnEvent(Event& event) override
	{
	}


	void OnEditor() override
	{
		if ( ImGui::Button("Generate Map") ) {
			if ( _entity.HasComponent<HierarchyComponent>() ) DeleteChilds();
			GenerateMap();
		}

		if ( ImGui::Button("Reset") ) {
			DeleteChilds();
		}
	}


private:
	void GenerateMap()
	{
		for ( int i = 0; i < 10; i++ )
			for ( int j = 0; j < 10; j++ ) {
				std::string id = std::to_string(i) + "_" + std::to_string(j);
				Entity newEntity= ECS::New(id);
				newEntity.AddComponent<SpriteRenderer>().SetColor({ i * 0.05, j * 0.05, 0, 1 });
				newEntity.AddComponent<Transform>().SetPosition({ i, j, 0 });
				_entity.AddChild(newEntity);
			}
	}


	void DeleteChilds()
	{
		_entity.GetComponent<HierarchyComponent>().RemoveAllChilds();
	}

CANDLE_DECL_SCRIPT(RandomGridGenerator)
};

CANDLE_SCRIPT(RandomGridGenerator)