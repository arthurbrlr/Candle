#pragma once

#include "Candle/CandleCore.h"

#include "Entity.h"

namespace Candle {

	struct ECSStats {
		double bpManagerUpdateTime = 0;
		double scriptManagerUpdateTime = 0;
		double systemManagerUpdateTime = 0;

		void ResetUpdateStats()
		{
			bpManagerUpdateTime = 0;
			scriptManagerUpdateTime = 0;
			systemManagerUpdateTime = 0;
		}
	};

	class ECS {
	
		public:

			static void Init();
			
			static Entity New(const std::string & name = "candleEntity");
			//static void Add(Shared<Blueprint> blueprint);
			//static void Add(System * system);
			static void Remove(Burst::Entity nativeEntity);
			static void Clear();

			//static Shared<Blueprint> GetBlueprint(size_t blueprintID);
			static Entity ViewEntity(Burst::Entity entityID);
			static std::unordered_map<Burst::Entity, Burst::Entity> ViewAllEntities();

			static void Update();
			static void Render();
			
			static void OnEditor();
			static ECSStats& GetStats() { return _stats; }

		private:
			static ECSStats _stats;
	};

#define ecsStats ECS::GetStats()


}