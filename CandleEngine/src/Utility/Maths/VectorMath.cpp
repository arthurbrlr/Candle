#include "cdlpch.h"
#include "VectorMath.h"

namespace Candle {

	double Maths_Vec2_DistanceSquared(const glm::vec2& a, const glm::vec2& b)
	{
		return ( b.x - a.x ) * ( b.x - a.x ) + ( b.y - a.y ) * ( b.y - a.y );
	}

	void Maths_Vec2_Rotate(glm::vec2& toRotate, double angleInDegree, const glm::vec2& origin)
	{
		double x = toRotate.x - origin.x;
		double y = toRotate.y - origin.y;

		double c = (double)std::cos(glm::radians((float)angleInDegree));
		double s = (double)std::sin(glm::radians((float)angleInDegree));

		toRotate.x = ( x * c ) - ( y * s ) + origin.x;
		toRotate.y = ( x * s ) + ( y * c ) + origin.y;
	}

}