#pragma once

#include <Candle.h>
using namespace Candle;

class RandomGridGenerator : public Script {

	public:
		RandomGridGenerator()
		{
			_name = __FILE__;
		}


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
				if ( _blueprint->HasChildren() ) DeleteChilds();
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
					Blueprint& element = ECS::New(id);
					element.AddComponent<SpriteRenderer>().SetColor({ i * 0.05, j * 0.05, 0, 1 });
					element.AddComponent<Transform>().SetPosition({ i, j, 0 });
					_blueprint->AddChild(&element);
				}
		}


		void DeleteChilds()
		{
			auto tiles = _blueprint->GetChilds();
			for ( auto tile : tiles ) {
				tile.second->Destroy();
			}
		}
};

AUTO_REGISTER_SCRIPT(RandomGridGenerator)