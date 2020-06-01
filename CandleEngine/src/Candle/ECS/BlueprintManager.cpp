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
		/*
		_blueprints.erase(
			std::remove_if(std::begin(_blueprints), std::end(_blueprints), 
			[](const Shared<Blueprint> & blueprint) {
				return !blueprint->IsAlive();
			}),
			std::end(_blueprints)
		);
		*/
		
		for ( auto& bp : _blueprints ) {
			if ( !bp.second->IsAwake() ) bp.second->Awake();
		}
	}

		
	void BlueprintManager::OnEvent(Event & event)
	{
		for (auto& bp : _blueprints) {
			if ( bp.second == nullptr ) continue;
			bp.second->OnEvent(event);
			if (event.IsHandled()) return;
		}
	}

		/* -------------- */
		/* Script Manager */
		/* -------------- */
	void ScriptManager::Update()
	{
		/*
		auto updateBlueprint = [](Shared<Blueprint> bp) {
			for ( auto& script : bp->Scripts() ) {
				script->OnUpdate();
			}
		};
		*/

		for (auto& bp : BlueprintManager::All()) {
			if ( bp.second == nullptr || !bp.second->IsAwake() ) continue;
			
			for (auto& script : bp.second->Scripts()) {
				script->OnUpdate();
			}
			

				// multithreaded, might not work later in developpment
			//std::async(std::launch::async, updateBlueprint, bp.second);
		}
	}


	void ScriptManager::OnEvent(Event & e)
	{
		for (auto& bp : BlueprintManager::All()) {
			if ( bp.second == nullptr ) continue;
			for (auto& script : bp.second->Scripts()) {
				script->OnEvent(e);
			}
		}
	}

}