#pragma once

#include "Candle/CandleCore.h"

#include "BlueprintManager.h"
#include "SystemManager.h"

namespace Candle {

	class CANDLE_API ECS {
	
		public:

			static void Init();
			
			static Blueprint& New(const std::string & name = "candleBlueprint");
			static void Add(Shared<Blueprint> blueprint);
			static void Add(System * system);
			static void Remove(unsigned int blueprintID);
			static void ClearBlueprints();

			static Shared<Blueprint> GetBlueprint(long blueprintID);

			static void Update();
			static void OnEvent(Event & event);
			static void Render();
			
			static void OnEditor();
	};


}