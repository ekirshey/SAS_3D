#pragma once
#include <glm/glm.hpp>

namespace SAS_3D {
	struct AABB {
		AABB(float width, float height, float length, glm::vec3 topleft);
		bool Intersects(const AABB& b);
	private:
		float _width;
		float _height;
		float _length;
		glm::vec3 _topleft;
	};
}

