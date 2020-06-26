#pragma once

#include "Candle/CandleCore.h"
#include "Blueprint.h"


namespace Candle {

	class CANDLE_API BlueprintManager {

		public:
			static Blueprint& Add(const std::string & name);
			static void Add(Shared<Blueprint> blueprint);
			static void Remove(size_t blueprintID);
			static void Update();
			static void Clear() { _blueprints.clear(); }

			static std::unordered_map<size_t, Shared<Blueprint>> & All() { return _blueprints; }
			
			template<typename T>
			static std::vector<Shared<Blueprint>> & Get()
			{
				_batchBlueprints.clear();

				for (auto& bp : _blueprints) {

					if ( bp.second == nullptr ) {
						continue;
					}
					if (bp.second->HasComponent<T>()) _batchBlueprints.push_back(bp.second);

				}

				return _batchBlueprints;
			}

		private:
			static std::unordered_map<size_t, Shared<Blueprint>> _blueprints;
			static std::vector<Shared<Blueprint>> _batchBlueprints;
	};


	class CANDLE_API ScriptManager {

		public:
			static void Update();

	};


}