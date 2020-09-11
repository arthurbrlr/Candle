#include "cdlpch.h"
#include "CollisionDetection2D.h"

#include "Candle/ECS/Components/Collider2D.h"
#include "Utility/Maths/VectorMath.h"

namespace Candle {

	static double EPSILON = 10e-5;

	bool AreEquals(const double value, const double target)
	{
		return ( std::abs(value - target) < EPSILON * std::max(1.0, std::max(std::abs(value), std::abs(target))) );
	}



		/***********************/
		/* Point vs primitives */
		/***********************/

	bool IsPointOnLine2D(const glm::vec2& point, const glm::vec2& lineSupport, const glm::vec2& lineDirection)
	{
		if ( AreEquals(lineDirection.x, 0.0) ) {
			return AreEquals(point.x, 0.0);
		}

		double a = lineDirection.y / lineDirection.x;
		double b = lineSupport.y - a * lineSupport.x;

		return AreEquals(point.y, a * point.x + b);
	}

	bool IsPointInCircle(const glm::vec2& point, const CircleCollider& circle)
	{
		double distanceSquared = Maths_Vec2_DistanceSquared(point, circle.GetPosition());
		double radiusSquared = circle.GetRadius() * circle.GetRadius();
		return distanceSquared <= radiusSquared;
	}

	bool IsPointInAABB(const glm::vec2& point, const AABB& aabb)
	{
		glm::vec2 min = aabb.GetMin();
		glm::vec2 max = aabb.GetMax();
		return ( point.x >= min.x && point.x <= max.x ) && ( point.y >= min.y && point.y <= max.y );
	}

	bool IsPointInBox2D(const glm::vec2& point, const BoxCollider& box)
	{
		glm::vec2 boxCenter = box.GetPosition();
		double boxAngle = box.GetRotation();

		glm::vec2 pointInBoxSpace = point;
		Maths_Vec2_Rotate(pointInBoxSpace, -boxAngle, boxCenter);

		glm::vec2* boxVertices = box.GetVertices();
		glm::vec2 min{ INFINITY, INFINITY }, max{ -INFINITY, -INFINITY };

		for ( int i = 0; i < 4; i++ ) {
			Maths_Vec2_Rotate(boxVertices[i], -boxAngle, boxCenter);
			if ( boxVertices[i].x > max.x ) max.x = boxVertices[i].x;
			if ( boxVertices[i].y > max.y ) max.y = boxVertices[i].y;
			if ( boxVertices[i].x < min.x ) min.x = boxVertices[i].x;
			if ( boxVertices[i].y < min.y ) min.y = boxVertices[i].y;
		}

		return	( pointInBoxSpace.x >= min.x && pointInBoxSpace.x <= max.x ) && 
				( pointInBoxSpace.y >= min.y && pointInBoxSpace.y <= max.y );
	}



		/**********************/
		/* Line vs primitives */
		/**********************/

	bool IsLineIntersectingCircle(const glm::vec2& lineSupport, const glm::vec2& lineDirection, const CircleCollider& circle)
	{
		if ( IsPointInCircle(lineSupport, circle) || IsPointInCircle(lineSupport + lineDirection, circle) ) return true;

		glm::vec2 circleCenter = circle.GetPosition();
		glm::vec2 supportToCenter = circleCenter - lineSupport;

		float p = glm::dot(lineDirection, supportToCenter);
		glm::vec2 closerPointToCircle = lineSupport + p * lineDirection;

		return IsPointInCircle(closerPointToCircle, circle);
	}

	bool IsLineIntersectingAABB(const glm::vec2& lineSupport, const glm::vec2& lineDirection, const AABB& aabb)
	{
		if ( IsPointInAABB(lineSupport, aabb) || IsPointInAABB(lineSupport + lineDirection, aabb) ) return true;

		glm::vec2 lineUnitVector = glm::normalize(lineDirection);
		lineUnitVector.x = ( !AreEquals(lineUnitVector.x, 0.0) ) ? 1.0 / lineUnitVector.x : 0.0;
		lineUnitVector.y = ( !AreEquals(lineUnitVector.y, 0.0) ) ? 1.0 / lineUnitVector.y : 0.0;

		glm::vec2 min = aabb.GetMin();
		min -= lineSupport;
		min *= lineUnitVector;

		glm::vec2 max = aabb.GetMax();
		max -= lineSupport;
		max *= lineUnitVector;

		double tmin = std::max( std::min(min.x, max.x), std::min(min.y, max.y) );
		double tmax = std::min( std::max(min.x, max.x), std::max(min.y, max.y) );

		return tmin < tmax;
	}

	bool IsLineIntersectingBox2D(const glm::vec2& lineSupport, const glm::vec2& lineDirection, const BoxCollider& box)
	{
		return false;
	}



		/***************************/
		/* Privitive vs primitives */
		/***************************/

	bool AreCirclesOverlapping(const CircleCollider& circle1, const CircleCollider& circle2)
	{
		double distanceBetweenCenter = glm::distance(circle1.GetPosition(), circle2.GetPosition());
		return ( distanceBetweenCenter < circle1.GetRadius() + circle2.GetRadius() );
	}

}