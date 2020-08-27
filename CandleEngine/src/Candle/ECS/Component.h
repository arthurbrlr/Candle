#pragma once

#include "Burst/Component.h"
#include "Candle/Scenes/SceneManagement.h"

#include "imgui.h"

namespace Candle {

#define CANDLE_DECL_COMPONENT(type) \
								public: \
									const std::string GetComponentName() override { return #type; } \
									BURST_DECL_COMPONENT(type)

#define CANDLE_COMPONENT(type)	BURST_COMPONENT(type)

	class Component : public Burst::Component {

		public:
			Component() : Burst::Component() {}
			virtual ~Component() {}

			virtual void OnEditor() = 0;
			virtual void Serialize(std::fstream& sceneFile) = 0;
			virtual void Deserialize(std::fstream& sceneFile) = 0;
			virtual const std::string GetComponentName() = 0;

			bool IsActive() const { return _isActive; }
			void SetActive(bool state) { _isActive = state; }

			virtual bool IsSerializable() { return true; }

			template<typename T>
			void RequireComponent()
			{
				Entity thisEntity = { SceneManagement::CurrentScene(), _entity };
				if ( !thisEntity.HasComponent<T>() ) {
					thisEntity.AddComponent<T>();
				}
			}

		protected:
			bool _isActive = true;
	};
}