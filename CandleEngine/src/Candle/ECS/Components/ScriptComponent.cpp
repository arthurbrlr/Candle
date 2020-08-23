#include "cdlpch.h"
#include "ScriptComponent.h"

#include "Candle/ECS/Script.h"

namespace Candle {

	ScriptComponent::~ScriptComponent()
	{
		for ( auto script : _scripts ) {
			delete script;
		}
	}


	void ScriptComponent::OnUpdate()
	{
		for ( auto script : _scripts ) {
			script->OnUpdate();
		}
	}


	void ScriptComponent::OnEditor()
	{
		for ( auto script : _scripts ) {
			if ( ImGui::TreeNode(typeid( *script ).name()) ) {
				script->OnEditor();
				ImGui::TreePop();
			}
		}
	}

}