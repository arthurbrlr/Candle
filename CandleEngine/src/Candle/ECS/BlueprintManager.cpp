#include "cdlpch.h"
#include "BlueprintManager.h"

#include "imgui.h"

namespace Candle {

	std::unordered_map<long, Shared<Blueprint>> BlueprintManager::_blueprints;
	std::vector<Shared<Blueprint>> BlueprintManager::_batchBlueprints;

	Blueprint& BlueprintManager::Add(const std::string & name)
	{
		Blueprint* bp = new Blueprint();
		bp->SetName(name);
		Shared<Blueprint> bpPtr(bp);
		_blueprints[bp->GetID()] = std::move(bpPtr);
		return *bp;
	}


	void BlueprintManager::Add(Shared<Blueprint> blueprint)
	{
		_blueprints[blueprint->GetID()] = std::move(blueprint);
	}


	void BlueprintManager::Remove(long blueprintID)
	{
		_blueprints.erase(blueprintID);
	}


	void BlueprintManager::Update()
	{
		std::vector<long> _idToDelete;

		for ( auto& bp : _blueprints ) {
			if ( !bp.second->IsAlive() ) {
				_idToDelete.push_back(bp.first);
			}
		}

		for ( long i : _idToDelete ) {
			_blueprints.erase(i);
		}


		for ( auto& bp : _blueprints ) {
			if ( !bp.second->IsAwake() ) bp.second->Awake();
		}
	}


		/* -------------- */
		/* Script Manager */
		/* -------------- */
	void ScriptManager::Update()
	{
		for (auto& bp : BlueprintManager::All()) {
			if ( bp.second == nullptr || !bp.second->IsAwake() ) continue;
			
			for (auto& script : bp.second->Scripts()) {
				script->OnUpdate();
			}

				// multithreaded, might not work later in developpment
			//std::async(std::launch::async, updateBlueprint, bp.second);
		}
	}

}