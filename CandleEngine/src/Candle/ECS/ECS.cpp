#include "cdlpch.h"
#include "ECS.h"

#include "Components/EngineComponents.h"
#include "Systems/EngineSystems.h"

namespace Candle {

	ECSStats ECS::_stats;

	void ECS::Init()
	{
		Add(new AnimationSystem());
	}


	Blueprint& ECS::New(const std::string & name)
	{
		return BlueprintManager::Add(name);
	}


	void ECS::Add(Shared<Blueprint> blueprint)
	{
		BlueprintManager::Add(blueprint);
	}


	void ECS::Add(System* system)
	{
		SystemManager::Add(system);
	}


	void ECS::Remove(size_t blueprintID)
	{
		BlueprintManager::Remove(blueprintID);
	}


	void ECS::ClearBlueprints()
	{
		BlueprintManager::Clear();
	}


	Shared<Blueprint> ECS::GetBlueprint(size_t blueprintID)
	{
		if ( BlueprintManager::All()[blueprintID] != nullptr) return BlueprintManager::All()[blueprintID];
		return nullptr;
	}


	void ECS::Update()
	{
		_stats.ResetUpdateStats();
		double t1 = Time::Milliseconds();
		BlueprintManager::Update();
		double t2 = Time::Milliseconds();
		ScriptManager::Update();
		double t3 = Time::Milliseconds();
		SystemManager::Update();
		double t4 = Time::Milliseconds();

		_stats.bpManagerUpdateTime = t2 - t1;
		_stats.scriptManagerUpdateTime = t3 - t2;
		_stats.systemManagerUpdateTime = t4 - t3;
	}


	void ECS::Render()
	{
		SystemManager::OnRender();
	}


	void ECS::OnEditor()
	{
		SystemManager::OnEditor();
	}

}