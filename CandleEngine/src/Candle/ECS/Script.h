#pragma once

#include "Candle/CandleCore.h"
#include "Candle/Events/Event.h"
#include "Entity.h"

namespace Candle {

	class Script {

		public:
			virtual ~Script() {}

			virtual void OnAwake() {}
			virtual void OnUpdate() {}
			virtual void OnEvent(Event & event) {}
			virtual void OnEditor() {}
			virtual void OnDestroy() {}
			virtual const std::string& GetName() { return "Script"; };

			void AttachTo(Entity entity)
			{
				_entity = entity;
			}

		protected:
			bool isUsed = true;
			Entity _entity;
	};


	template<class T>
	Script* ScriptFactory()
	{
		return new T;
	}

	typedef Script* ( *ScriptCreator )( void );

	class EditorScriptRegistery {
	public:
		typedef std::vector<ScriptCreator>::iterator iterator;

		static EditorScriptRegistery& Get()
		{
			static EditorScriptRegistery instance;
			return instance;
		}

		void Add(const std::string& scriptName, ScriptCreator componentCreator)
		{
			_components[scriptName] = componentCreator;
		}

		std::unordered_map<std::string, ScriptCreator>& Scripts()
		{
			return _components;
		}

	private:
		std::unordered_map<std::string, ScriptCreator> _components;
	};


	class ComponentRegisteration {
	public:
		ComponentRegisteration(const std::string& componentName, ScriptCreator componentCreator)
		{
			EditorScriptRegistery::Get().Add(componentName, componentCreator);
		}
	};

}

#define CANDLE_DECL_SCRIPT(script) public: const std::string& GetName() override { return #script; } private:
#define CANDLE_SCRIPT(script) Candle::ComponentRegisteration _registerScript_##script(#script, &Candle::ScriptFactory<script>);