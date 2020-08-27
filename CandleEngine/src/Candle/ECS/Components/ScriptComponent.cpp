#include "cdlpch.h"
#include "ScriptComponent.h"

#include "Candle/ECS/Script.h"
#include "Utility/System/SerializeUtility.h"

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


	void ScriptComponent::Serialize(std::fstream& sceneFile)
	{
		sceneFile << "\t\tscriptsize:" << _scripts.size() << std::endl;
		for ( auto script : _scripts ) {
			sceneFile << "\t\t\t";
			sceneFile << typeid( *script ).name() << ":";
			//sceneFile.write((char*)script, sizeof(*script)); // unused for now
			sceneFile << std::endl;
		}
	}


	void ScriptComponent::Deserialize(std::fstream& sceneFile)
	{
		int nbscript;
		GetSerializedInt(sceneFile, nbscript);
		
		std::string line;
		for ( int i = 0; i < nbscript; i++ ) {

			bool scriptDeserialized = false;

			std::getline(sceneFile, line);
			std::string scriptName;
			//std::string serializedValueString = line.substr(line.find_first_of(':') + 1, line.size()); // unused for now
			for ( auto [name, creator] : EditorScriptRegistery::Get().Scripts() ) {
				if ( line.find(name) != std::string::npos ) {
					Script* newScript = ( *creator )( );
					newScript->AttachTo(Entity{ SceneManagement::CurrentScene(), _entity });
					_scripts.push_back(newScript);

					scriptDeserialized = true;
					scriptName = name;
					break;
				}
			}

			if ( !scriptDeserialized ) CERROR("Script with type {0} was not deserialized properly", scriptName);

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