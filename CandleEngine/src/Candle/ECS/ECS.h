#pragma once

#include "Candle/CandleCore.h"

#include "BlueprintManager.h"
#include "SystemManager.h"

namespace Candle {

	struct ECSStats {
		double bpManagerUpdateTime = 0;
		double scriptManagerUpdateTime = 0;
		double systemManagerUpdateTime = 0;

		double bpManagerEventTime = 0;
		double scriptManagerEventTime = 0;
		double systemManagerEventTime = 0;

		void ResetUpdateStats()
		{
			bpManagerUpdateTime = 0;
			scriptManagerUpdateTime = 0;
			systemManagerUpdateTime = 0;
		}

		void ResetEventStats()
		{
			bpManagerEventTime = 0;
			scriptManagerEventTime = 0;
			systemManagerEventTime = 0;
		}

	};

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
			static void Render();
			
			static void OnEditor();
			static ECSStats& GetStats() { return _stats; }

		private:
			static ECSStats _stats;
	};

#define ecsStats ECS::GetStats()


}