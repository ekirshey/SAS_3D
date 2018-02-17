#pragma once
#include "core/sas_io.h"
#include "ecs/components/dynamics_component.h"

namespace SAS_3D {
	void UpdatePlayerInput(const InputState& input, DynamicsComponent* playerdynamics) {
		// Handle player input
		bool moved = false;
		if (input.keyarray[SDL_SCANCODE_UP] == KeyState::PRESSED) {
			playerdynamics->m_force = glm::vec3(0.0f, 0.0f, 10.0f);
			moved = true;
		}
		if (input.keyarray[SDL_SCANCODE_DOWN] == KeyState::PRESSED) {
			playerdynamics->m_force = glm::vec3(0.0f, 0.0f, -10.0f);
			moved = true;
		}
		if (input.keyarray[SDL_SCANCODE_LEFT] == KeyState::PRESSED) {
			playerdynamics->m_force = glm::vec3(-10.0f, 0.0f, 0.0f);
			moved = true;
		}
		if (input.keyarray[SDL_SCANCODE_RIGHT] == KeyState::PRESSED) {
			playerdynamics->m_force = glm::vec3(10.0f, 0.0f, 0.0f);
			moved = true;
		}

		if (!moved) {
			playerdynamics->m_force = glm::vec3(0.0f, 0.0f, 0.0f);
			playerdynamics->m_acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
			playerdynamics->m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}
}
