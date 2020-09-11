#pragma once

#include "Candle/ECS/Component.h"

namespace Candle {

	class Rigidbody2D : public Component {
		public:
			Rigidbody2D();

			void Serialize(std::fstream& sceneFile) override {}
			void Deserialize(std::fstream& sceneFile) override {}
			void OnEditor() override {}

		private:

	};

}