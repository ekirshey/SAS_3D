#include "AABB.h"

namespace SAS_3D {
	AABB::AABB(float width, float height, float length, glm::vec3 topleft)
		: _width(width)
		, _height(height)
		, _length(length)
		, _topleft(topleft)
	{

	}

	bool AABB::Intersects(const AABB& b) {

		return false;
	}

}