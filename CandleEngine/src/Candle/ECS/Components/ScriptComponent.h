#pragma once

#include "Candle/ECS/Component.h"

namespace Candle {

	class Script;

	class ScriptComponent : public Component {

	public:
		ScriptComponent()
		{
		}

		~ScriptComponent();

		template<typename T, typename... Args>
		T* AddScript(Args&&... args)
		{
			_scripts.push_back(new T(args...));
			return (T*)_scripts[_scripts.size() - 1];
		}

		Script* AddScript(Script* script)
		{
			_scripts.emplace_back(std::move(script));
			return _scripts[_scripts.size() - 1];
		}

		void OnUpdate();

		void OnEditor() override;

	private:
		std::vector<Script*> _scripts;

		CANDLE_DECL_COMPONENT(ScriptComponent)
	};

}