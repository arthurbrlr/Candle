#pragma once

#include "Candle/ECS/Component.h"
#include "Utility/System/Identifier.h"

namespace Candle {

	class EntityTagComponent : public Component {

		public:
			EntityTagComponent() = default;
			EntityTagComponent(const std::string& name, const UUID& uuid)
				: _entityUUID(uuid)
			{
				strcpy(_entityName, name.c_str());
			}

			void SetName(const std::string& name)
			{
				strcpy(_entityName, name.c_str());
			}

			void SetUUID(const UUID& uuid)
			{
				_entityUUID = uuid;
			}

			const std::string GetName()
			{
				if ( _entityName[0] == '\0' ) return "placeHolderName";
				return std::string(_entityName);
			}

			const UUID& GetUUID()
			{
				return _entityUUID;
			}

			void Serialize(std::fstream& sceneFile) override
			{
				sceneFile << "\t\tname:" << _entityName << std::endl;
				sceneFile << "\t\tuuid:" << _entityUUID << std::endl;
			}

			void Deserialize(std::fstream& sceneFile) override 
			{
				std::string line;

				std::getline(sceneFile, line);
				std::string name = line.substr(line.find_first_of(':') + 1, line.size());
				SetName(name);

				std::getline(sceneFile, line);
				std::string uuid = line.substr(line.find_first_of(':') + 1, line.size());
				SetUUID(uuid);
			}

			void OnEditor() override
			{
				ImGui::InputText("Name", &_entityName[0], 32);
				ImGui::Text("UUID : %s", _entityUUID.c_str());
			}

		private:
			char _entityName[32] = { '\0' };
			UUID _entityUUID;

			CANDLE_DECL_COMPONENT(EntityTagComponent)
	};

}