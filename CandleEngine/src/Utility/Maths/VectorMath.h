#pragma once

#include "glm/glm.hpp"

namespace Candle {

	double Maths_Vec2_DistanceSquared(const glm::vec2& a, const glm::vec2& b);

	void Maths_Vec2_Rotate(glm::vec2& toRotate, double angleInDegree, const glm::vec2& origin);

}