#pragma once

#include "Candle/CandleCore.h"
#include "Blueprint.h"


namespace Candle {

	class BlueprintManager {

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



		/* Script factory */
	template<class T> Script* script_factory()
	{
		return new T;
	}

	typedef Script* ( *script_creator )( void );

	class ScriptManager {
		public:
			typedef std::vector<script_creator>::iterator iterator;

			static void Update();

			static ScriptManager& Get()
			{
				static ScriptManager instance;
				return instance;
			}

			void Add(script_creator creator)
			{
				_scripts.push_back(creator);
			}

			void Add(std::string str)
			{
				_scriptsNames.push_back(str);
			}

			std::vector<script_creator>& GetScripts()
			{
				return _scripts;
			}

			std::vector<std::string>& GetScriptsNames()
			{
				return _scriptsNames;
			}

		private:
			std::vector<script_creator> _scripts;
			std::vector<std::string> _scriptsNames;

	};

	class ScriptRegistration {
		public:
			ScriptRegistration(script_creator creator)
			{
				ScriptManager::Get().Add(creator);
			}

			ScriptRegistration(std::string str)
			{
				ScriptManager::Get().Add(str);
			}
	};

#define AUTO_REGISTER_SCRIPT(script) \
    ScriptRegistration _Script_registration_ ##script(&script_factory<script>); \
	ScriptRegistration _Script_registration_names_ ##script(#script);

}