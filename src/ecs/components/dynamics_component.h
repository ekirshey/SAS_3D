#pragma once
#include "glm/glm.hpp"
#include "ecs/framework/component.h"

namespace SAS_3D {
	// Per instance animation data
	struct DynamicsComponent : Component {
		static const ComponentID ID = DYNAMICS_COMPONENT;

		ComponentID UniqueBits() const { return ID; }

		DynamicsComponent( glm::vec3 initialposition, float mass)
			: m_position(initialposition)
			, m_mass(mass)
			, m_velocity(glm::vec3(0.0,0.0,0.0))
			, m_acceleration(glm::vec3(0.0, 0.0, 0.0))
			, m_force(glm::vec3(0.0, 0.0, 0.0))
		{}

		glm::vec3 m_position;		// Current 3d position of the objects center
		float m_mass;
		glm::vec3 m_velocity;
		glm::vec3 m_acceleration;
		glm::vec3 m_force;		//total force applied to the entity
	};
}