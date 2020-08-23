#include "cdlpch.h"
#include "ECS.h"

#include "Candle/Core/Time.h"
#include "Components/EngineComponents.h"

namespace Candle {

	ECSStats ECS::_stats;

	void ECS::Init()
	{
		//Add(new AnimationSystem());
	}


	Entity ECS::New(const std::string & name)
	{
		Entity newEntity = Entity{ SceneManagement::CurrentScene().get() };
		newEntity.SetName(name);
		return newEntity;
	}


	//void ECS::Add(Shared<Blueprint> blueprint)
	//{
	//	BlueprintManager::Add(blueprint);
	//}
	

	//void ECS::Add(System* system)
	//{
	//	SystemManager::Add(system);
	//}


	void ECS::Remove(Burst::Entity nativeEntity)
	{
		Entity toRemove{ SceneManagement::CurrentScene().get(), nativeEntity };
		if ( toRemove.HasComponent<HierarchyComponent>() ) {
			toRemove.GetComponent<HierarchyComponent>().Clear();
		}
		SceneManagement::CurrentScene()->_sceneRegistery.RemoveEntity(nativeEntity);
	}


	void ECS::Clear()
	{
	}


	Entity ECS::ViewEntity(Burst::Entity entityID)
	{
		// TODO : Check if the entityID exist in scene registery
		return Entity{ SceneManagement::CurrentScene().get(), entityID };
	}


	std::unordered_map<Burst::Entity, Burst::Entity> ECS::ViewAllEntities()
	{
		return SceneManagement::CurrentScene()->_sceneRegistery.ViewEntities();
	}


	void ECS::Update()
	{
		_stats.ResetUpdateStats();
		double t1 = Time::Milliseconds();
		//BlueprintManager::Update();
		double t2 = Time::Milliseconds();
		
		for ( auto scriptComponent : SceneManagement::CurrentScene()->_sceneRegistery.View<ScriptComponent>() ) {
			( (ScriptComponent*) scriptComponent.second )->OnUpdate();
		}

		double t3 = Time::Milliseconds();
		//SystemManager::Update();
		double t4 = Time::Milliseconds();

		_stats.bpManagerUpdateTime = t2 - t1;
		_stats.scriptManagerUpdateTime = t3 - t2;
		_stats.systemManagerUpdateTime = t4 - t3;
	}


	void ECS::Render()
	{
		//SystemManager::OnRender();
	}


	void ECS::OnEditor()
	{
		//SystemManager::OnEditor();
	}

}