#pragma once

#include "Candle/ECS/Component.h"

namespace Candle {

	class Collider2D : public Component {
		public:
			Collider2D();
		protected:
			glm::vec2 _offset = { 0, 0 };
	};

	class DebugPointCollider : public Collider2D {
		public:
			DebugPointCollider();

			glm::vec2 GetPosition() const;

			void Serialize(std::fstream& sceneFile) override {}
			void Deserialize(std::fstream& sceneFile) override {}
			void OnEditor() override {}

			CANDLE_DECL_COMPONENT(DebugPointCollider)
	};

	class CircleCollider : public Collider2D {
		public:
			CircleCollider(double radius = 0.5);

			double GetRadius() const;
			glm::vec2 GetPosition() const;

			void Serialize(std::fstream& sceneFile) override {}
			void Deserialize(std::fstream& sceneFile) override {}
			void OnEditor() override;

		private:
			double _radius = 1.0;

		CANDLE_DECL_COMPONENT(CircleCollider)
	};

	class BoxCollider : public Collider2D {
		public: 
			BoxCollider(const glm::vec2& size = { 1, 1 });

			glm::vec2* GetVertices() const;
			glm::vec2 GetSize() const;
			glm::vec2 GetPosition() const;
			double GetRotation() const;

			void Serialize(std::fstream& sceneFile) override {}
			void Deserialize(std::fstream& sceneFile) override {}
			void OnEditor() override;

		private:
			glm::vec2 _size;

		CANDLE_DECL_COMPONENT(BoxCollider)
	};

	class AABB : public Collider2D {
		public:
			AABB(const glm::vec2& size = { 1, 1 });

			glm::vec2 GetMin() const;
			glm::vec2 GetMax() const;
			glm::vec2 GetPosition() const;
			glm::vec2 GetSize() const;

			void Serialize(std::fstream& sceneFile) override {}
			void Deserialize(std::fstream& sceneFile) override {}
			void OnEditor() override {}

		private:
			glm::vec2 _size;

		CANDLE_DECL_COMPONENT(AABB)
	};

}