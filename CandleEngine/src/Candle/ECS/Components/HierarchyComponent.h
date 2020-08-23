#pragma once

#include "Candle/ECS/Component.h"
#include "Burst/Types.h"

namespace Candle {

	class Entity;

	class HierarchyComponent : public Component {

		public:
			HierarchyComponent() = default;
			HierarchyComponent(Burst::Entity parent);

			void AddChild(Entity childEntity);
			void RemoveChild(Entity childEntity);

			void RemoveAllChilds();
			void Clear();
			
			Entity GetParent();
			const std::vector<Burst::Entity>& GetChilds();

			bool HasChilds();
			bool HasParent();

			void OnEditor() override;

		private:
			Burst::Entity _parent = -1;
			std::vector<Burst::Entity> _childs;

			CANDLE_DECL_COMPONENT(HierarchyComponent)
	};

}