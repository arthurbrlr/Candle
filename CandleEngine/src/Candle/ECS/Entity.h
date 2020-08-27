#pragma once

#include "Candle/Scenes/Scene.h"
#include "Burst/Types.h"

#include "Utility/System/Identifier.h"

#include "Components/EntityTagComponent.h"
#include "Components/HierarchyComponent.h"
#include "Components/ScriptComponent.h"

namespace Candle {

	class Entity {

		public:
			Entity() = default;
			Entity(Scene* scene, Burst::Entity entity = -1)
				: _scene(scene), _entity(entity)
			{
				if ( entity == -1 ) {
					UUID uuid = NewUUID();
					_entity = scene->_sceneRegistery.NewEntity(uuid);
					AddComponent<EntityTagComponent>("Entity", uuid);
				}
			}


			Entity(Scene* scene, const std::string& name, const UUID& uuid)
				: _scene(scene)
			{
				Burst::Entity temp = scene->_sceneRegistery.GetEntityFromExternID(uuid);
				if ( temp == -1 )
					_entity = scene->_sceneRegistery.NewEntity(uuid);
				else
					_entity = temp;

				if ( !HasComponent<EntityTagComponent>() )
					AddComponent<EntityTagComponent>(name, uuid);
				else {
					SetName(name);
				}
			}


			template<typename T, typename... TArgs>
			T& AddComponent(TArgs&&... args)
			{
				return *_scene->_sceneRegistery.AddComponent<T>(_entity, std::forward<TArgs>(args)...);
			}


			void RemoveComponent(Burst::Component* component)
			{
				_scene->_sceneRegistery.RemoveComponent(_entity, component);
			}


			template<typename T, typename... TArgs>
			T& AddScript(TArgs&&... args)
			{
				T* newScript = new T(std::forward<TArgs>(args)...);
				return (T&)*AddScript(newScript);
			}

			Script* AddScript(Script* script);

			template<typename T> 
			bool HasComponent()
			{
				return _scene->_sceneRegistery.HasComponent<T>(_entity);
			}

			template<typename T>
			T& GetComponent()
			{
				return *_scene->_sceneRegistery.GetComponent<T>(_entity);
			}

			auto GetAllComponents()
			{
				return _scene->_sceneRegistery.View(_entity);
			}


			void AddChild(Entity child)
			{
				if ( HasComponent<HierarchyComponent>() ) {
					GetComponent<HierarchyComponent>().AddChild(child);
				} else {
					AddComponent<HierarchyComponent>().AddChild(child);
				}
			}

			const std::string GetName()
			{
				if ( _entity == -1 ) return "placeholder";
				return GetComponent<EntityTagComponent>().GetName();
			}

			void SetName(const std::string& entityName)
			{
				if ( HasComponent<EntityTagComponent>() ) {
					GetComponent<EntityTagComponent>().SetName(entityName);
				} else {
					CERROR("Couldn't find TagComponent on entity {0}", entityName);
				}
			}

			const UUID& GetUUID()
			{
				return GetComponent<EntityTagComponent>().GetUUID();
			}

			const Burst::Entity GetNativeEntity()
			{
				return _entity;
			}


		private:
			Scene* _scene = nullptr;
			Burst::Entity _entity = -1;
	};

}