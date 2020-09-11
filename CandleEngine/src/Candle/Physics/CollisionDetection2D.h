#pragma once

namespace Candle {
	
	class CircleCollider;
	class BoxCollider;
	class AABB;

	bool AreEquals(const double value, const double target);

		/* Point vs primitives */
	bool IsPointOnLine2D(const glm::vec2& point, const glm::vec2& lineSupport, const glm::vec2& lineDirection);
	bool IsPointInCircle(const glm::vec2& point, const CircleCollider& circle);
	bool IsPointInAABB  (const glm::vec2& point, const AABB& aabb);
	bool IsPointInBox2D (const glm::vec2& point, const BoxCollider& box);

		/* Line vs primitives */
	// TODO : Add line segments ( start to end )
	bool IsLineIntersectingCircle(const glm::vec2& lineSupport, const glm::vec2& lineDirection, const CircleCollider& circle);
	bool IsLineIntersectingAABB(const glm::vec2& lineSupport, const glm::vec2& lineDirection, const AABB& aabb);
	bool IsLineIntersectingBox2D(const glm::vec2& lineSupport, const glm::vec2& lineDirection, const BoxCollider& box);

		/* Primitive vs primitives */
	bool AreCirclesOverlapping(const CircleCollider& circle1, const CircleCollider& circle2);

}