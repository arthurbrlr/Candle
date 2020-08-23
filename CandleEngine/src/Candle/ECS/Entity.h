#pragma once

#include "Candle/Scenes/Scene.h"
#include "Burst/Types.h"

#include "Components/EntityTagComponent.h"
#include "Components/HierarchyComponent.h"
#include "Components/ScriptComponent.h"

#include <utility>

namespace Candle {

	static std::string NewUUID()
	{
		static std::random_device              rd;
		static std::mt19937                    gen(rd());
		static std::uniform_int_distribution<> dis(0, 15);
		static std::uniform_int_distribution<> dis2(8, 11);

		// TODO : keep track of existing UUIDs

		std::stringstream ss;
		int i;
		ss << std::hex;
		for ( i = 0; i < 8; i++ ) {
			ss << dis(gen);
		}
		//ss << "-";
		for ( i = 0; i < 4; i++ ) {
			ss << dis(gen);
		}
		//ss << "-";
		for ( i = 0; i < 4; i++ ) {
			ss << dis(gen);
		}
		//ss << "-";
		ss << dis2(gen);
		for ( i = 0; i < 3; i++ ) {
			ss << dis(gen);
		}
		//ss << "-";
		for ( i = 0; i < 12; i++ ) {
			ss << dis(gen);
		};
		return ss.str();
	}

	class Entity {

		public:
			Entity() = default;
			Entity(Scene* scene, Burst::Entity entity = -1)
				: _scene(scene), _entity(entity)
			{
				if ( entity == -1 ) {
					UUID uuid = NewUUID();
					_entity = scene->_sceneRegistery.NewEntity();
					AddComponent<EntityTagComponent>("Entity", uuid);
				}
			}


			template<typename T, typename... TArgs>
			T& AddComponent(TArgs&&... args)
			{
				return *_scene->_sceneRegistery.AddComponent<T>(_entity, std::forward<TArgs>(args)...);
			}

			template<typename T, typename... TArgs>
			T& AddScript(TArgs&&... args)
			{
				T* newScript = new T(std::forward<TArgs>(args)...);
				return (T&)*AddScript(newScript);
				//if ( HasComponent<ScriptComponent>() ) {
				//	newScript = GetComponent<ScriptComponent>().AddScript<T>(std::forward<TArgs>(args)...);
				//	( (Script*)( newScript ) )->AttachTo(*this);
				//} else {
				//	newScript = AddComponent<ScriptComponent>().AddScript<T>(std::forward<TArgs>(args)...);
				//	( (Script*)( newScript ) )->AttachTo(*this);
				//}
				//return *newScript;
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

			const std::string& GetName()
			{
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
			Scene* _scene;
			Burst::Entity _entity;
	};

}