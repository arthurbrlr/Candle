#pragma once

#include "Candle/CandleCore.h"
#include "Component.h"
#include "Script.h"

#include "imgui.h"

namespace Candle {

	class CANDLE_API Blueprint {

		public:	
			Blueprint();
			virtual ~Blueprint() {}

			template<typename T, typename... TArgs> T& AddComponent(TArgs... args)
			{
				T* component = new T(std::forward<TArgs>(args)...);
				component->SetParent(this);
				Unique<Component> compPtr(component);
				_components.emplace_back(std::move(compPtr));

				_compArray[GetComponentID<T>()] = component;
				_compBitset[GetComponentID<T>()] = true;

				return *component;
			}


			template<typename T, typename... TArgs> T& AddScript(TArgs... args)
			{
				T* script = new T(std::forward<TArgs>(args)...);
				script->SetParent(this);
				Unique<Script> scrPtr(script);
				_scripts.emplace_back(std::move(scrPtr));

				script->OnAwake();

				return *script;
			}


			inline const void Awake() { _isAwake = true; }
			inline const void Destroy() { _isAlive = false; }
			virtual void OnEvent(Event & event) {};

			template<typename T> bool HasComponent() const { return _compBitset[GetComponentID<T>()]; }
			template<typename T> T& GetComponent() const
			{
				CASSERT(HasComponent<T>(), "Blueprint has no component of type T");
				auto compPointer = _compArray[GetComponentID<T>()];
				return *static_cast<T*>(compPointer);
			}

			bool HasComponentOfID(size_t id) const { return _compBitset[id]; }
			Component* GetComponentOfID(size_t id) const
			{
				CASSERT(HasComponentOfID(id), "Blueprint has no component of type T");
				auto compPointer = _compArray[id];
				return static_cast<Component*>(compPointer);
			}

			std::vector<Unique<Component>> & Components() { return _components; }
			std::vector<Unique<Script>> & Scripts() { return _scripts; }

			/*
			void Add(const std::string & childName, Blueprint & child)
			{
				_childs[childName] = std::move(child);
				_hasChildren = true;
			}
			*/

			inline void SetName(const std::string & name) { _name = name; }
			inline void SetViewInEditor(bool state) { _viewInEditor = state; }

			inline const long GetID() { return _blueprintID; }
			inline const std::string & GetName() { return _name; }
			inline const bool HasChildren() { return _hasChildren; }
			inline bool IsAlive() const { return _isAlive; }
			inline bool IsAwake() const { return _isAwake; }

				/* --- Debug --- */
			inline const bool & GetViewInEditor() { return _viewInEditor; }

		protected:
			std::string _name;
			long _blueprintID;
			static long blueprintCount;

			bool _isAlive = true;
			bool _isAwake = false;
			bool _hasChildren = false;

			Blueprint * _parent = nullptr;
			std::unordered_map<std::string, Shared<Blueprint>> _childs;

			std::vector<Unique<Component>> _components;
			std::vector<Unique<Script>> _scripts;

			ComponentArray _compArray;
			ComponentBitSet _compBitset;


			// Debug
			bool _viewInEditor = true;

	};

}