#include "cdlpch.h"
#include "ECS.h"

#include "Components/EngineComponents.h"
#include "Systems/EngineSystems.h"

namespace Candle {

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


	void ECS::Remove(unsigned int blueprintID)
	{
		BlueprintManager::Remove(blueprintID);
	}


	void ECS::ClearBlueprints()
	{
		BlueprintManager::Clear();
	}


	Shared<Blueprint> ECS::GetBlueprint(long blueprintID)
	{
		return BlueprintManager::All()[blueprintID];
	}


	void ECS::Update()
	{
		BlueprintManager::Update();
		ScriptManager::Update();
		SystemManager::Update();
	}


	void ECS::OnEvent(Event & event)
	{
		BlueprintManager::OnEvent(event);
		ScriptManager::OnEvent(event);
		SystemManager::OnEvent(event);
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