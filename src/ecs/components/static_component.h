#pragma once
#include "glm/glm.hpp"
#include "ecs/framework/component.h"

namespace SAS_3D {
	// Per instance animation data
	struct StaticComponent : Component {
		static const ComponentID ID = STATIC_COMPONENT;

		ComponentID UniqueBits() const { return ID; }

		StaticComponent(glm::vec3 initialposition)
			: m_position(initialposition)
		{}

		glm::vec3 m_position;		// Current 3d position of the objects center
	};
}