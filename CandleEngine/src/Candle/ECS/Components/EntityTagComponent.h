#pragma once

#include "Candle/ECS/Component.h"

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

			const std::string GetName()
			{
				return std::string(_entityName);
			}

			const UUID& GetUUID()
			{
				return _entityUUID;
			}

			void OnEditor() override
			{
				ImGui::InputText("Name", &_entityName[0], 32);
				ImGui::Text("UUID : %s", _entityUUID.c_str());
			}

		private:
			char _entityName[32];
			UUID _entityUUID;

			CANDLE_DECL_COMPONENT(EntityTagComponent)
	};

}