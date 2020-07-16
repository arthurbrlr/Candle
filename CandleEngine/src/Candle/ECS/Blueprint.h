#pragma once

#include "Candle/CandleCore.h"
#include "Component.h"
#include "Script.h"

#include "imgui.h"

namespace Candle {

	class Blueprint {

		public:	
			Blueprint();
			virtual ~Blueprint() {}

			template<typename T, typename... TArgs> T& AddComponent(TArgs... args)
			{
				T* component = new T(std::forward<TArgs>(args)...);
				component->AttachToBlueprint(this);
				Unique<Component> compPtr(component);
				_components.emplace_back(std::move(compPtr));

				_compArray[GetComponentID<T>()] = component;
				_compBitset[GetComponentID<T>()] = true;

				return *component;
			}


			template<typename T, typename... TArgs> T& AddScript(TArgs... args)
			{
				T* script = new T(std::forward<TArgs>(args)...);
				script->AttachToBlueprint(this);
				Unique<Script> scrPtr(script);
				_scripts.emplace_back(std::move(scrPtr));

				script->OnAwake();

				return *script;
			}


			inline const void Awake() { _isAwake = true; }
			inline const void Sleep() { _isAwake = false; }
			inline const void Destroy() { _isAlive = false; }

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
			std::unordered_map<size_t, Blueprint*> GetChilds() { return _childs; }
			Blueprint* GetParent() { return _parent; }

			void AddChild(Blueprint* child) 
			{ 
				_childs[child->GetID()] = child;
				_hasChildren = true;
				child->SetParent(this);
			}

			void RemoveChild(size_t childID)
			{
				try {
					_childs.erase(childID);
				} catch ( std::exception e ) {
					CDL_ERROR(e.what());
				}
			}

			void SetParent(Blueprint* parent)
			{
				_parent = parent;
			}

			inline void SetName(const std::string & name) { _name = name; }

			inline const size_t GetID() { return _blueprintID; }
			inline const std::string & GetName() { return _name; }
			inline const bool HasParent() { return _parent != nullptr; }
			inline const bool HasChildren() { return _hasChildren; }
			inline bool IsAlive() const { return _isAlive; }
			inline bool IsAwake() const { return _isAwake; }

				/* --- Debug --- */
			inline void SetViewInEditor(bool state) { _viewInEditor = state; }
			inline const bool & GetViewInEditor() { return _viewInEditor; }

		protected:
			std::string _name;
			size_t _blueprintID;
			static size_t blueprintCount;

			bool _isAlive = true;
			bool _isAwake = false;
			bool _hasChildren = false;

			Blueprint * _parent = nullptr;
			std::unordered_map<size_t, Blueprint*> _childs;

			std::vector<Unique<Component>> _components;
			std::vector<Unique<Script>> _scripts;

			ComponentArray _compArray;
			ComponentBitSet _compBitset;


			// Debug
			bool _viewInEditor = true;

	};

}