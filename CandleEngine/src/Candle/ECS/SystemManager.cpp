#include "cdlpch.h"
#include "SystemManager.h"

namespace Candle {

	std::vector<Shared<System>> SystemManager::_systems;

	void SystemManager::Add(System* system)
	{
		for (auto& s : _systems) {
			if (false) {
				CASSERT(false, "There is already a similar system in the manager");
				return;
			}
		}

		Shared<System> sysPtr(system);
		_systems.emplace_back(std::move(sysPtr));
	}


	void SystemManager::Update()
	{
		/*
		auto systemUpdate = [](Shared<System> system) {
			system->OnUpdate();
		};
		*/

		for ( auto& s : _systems ) s->OnUpdate(); 

			// multithreaded, might not work later in developpment
		/*
		for ( auto& sys : _systems ) {
			std::async(std::launch::async, systemUpdate, sys);
		}
		*/
	}

}