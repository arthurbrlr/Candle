#include "cdlpch.h"
#include "Entity.h"

#include "Script.h"

namespace Candle {

	Script* Entity::AddScript(Script* script)
	{
		script->AttachTo(*this);
		script->OnAwake();

		if ( HasComponent<ScriptComponent>() ) {
			return GetComponent<ScriptComponent>().AddScript(std::forward<Script*>(script));
		} else {
			return AddComponent<ScriptComponent>().AddScript(std::forward<Script*>(script));
		}
	}

}