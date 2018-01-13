#pragma once
#include "glm/glm.hpp"
#include "ecs/framework/component.h"

namespace SAS_3D {
	// Per instance animation data
	struct PhysicalComponent : Component {
		static const ComponentID ID = PHYSICAL_COMPONENT;

		ComponentID UniqueBits() const { return ID; }

		PhysicalComponent(glm::mat4 mat)
			: modeltransform(mat)
		{}

		glm::mat4 modeltransform;
	};
}