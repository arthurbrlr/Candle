#include "cdlpch.h"
#include "ECS.h"

#include "Candle/Core/Time.h"
#include "Components/EngineComponents.h"
#include "Candle/Physics/CollisionDetection2D.h"

namespace Candle {

	ECSStats ECS::_stats;

	void ECS::Init()
	{
		//Add(new AnimationSystem());
	}


	Entity ECS::New(const std::string & name)
	{
		Entity newEntity = Entity{ SceneManagement::CurrentScene() };
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
		Entity toRemove{ SceneManagement::CurrentScene(), nativeEntity };
		if ( toRemove.HasComponent<HierarchyComponent>() ) {
			toRemove.GetComponent<HierarchyComponent>().Clear();
		}
		SceneManagement::CurrentScene()->_sceneRegistery.RemoveEntity(toRemove.GetUUID());
	}


	void ECS::Clear()
	{
	}


	Entity ECS::EntityFromUUID(const UUID& uuid)
	{
		Burst::Entity nativeEntity = SceneManagement::CurrentScene()->_sceneRegistery.GetEntityFromExternID(uuid);
		if ( nativeEntity != -1 )
			return Entity{ SceneManagement::CurrentScene(), nativeEntity };
		return Entity{};
	}


	std::unordered_map<UUID, Burst::Entity> ECS::ViewAllEntities()
	{
		return SceneManagement::CurrentScene()->_sceneRegistery.ViewEntities();
	}


	void ECS::Update()
	{
		_stats.ResetUpdateStats();
		double t1 = Time::Milliseconds();
		//BlueprintManager::Update();
		double t2 = Time::Milliseconds();
		
		for ( auto [nativeEntity, scriptComponent] : SceneManagement::CurrentScene()->_sceneRegistery.View<ScriptComponent>() ) {
			( (ScriptComponent*) scriptComponent )->OnUpdate();
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