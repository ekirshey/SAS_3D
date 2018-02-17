#include "dynamics_system.h"
#include "subsystems/subsystem_controller.h"
#include "ecs/framework/entity_manager.h"
#include "ecs/components/dynamics_component.h"

namespace SAS_3D {
	DynamicsSystem::DynamicsSystem() {


	}

	DynamicsSystem::~DynamicsSystem() {

	}

	void DynamicsSystem::Update(long long elapsedtime,
		const std::vector<EntityID>& entities,
		EntityManager* em,
		SubsystemController* subsystems) {

		for (const auto& entity : entities) {
			auto dynamics = em->GetEntityComponent<DynamicsComponent>(entity, DYNAMICS_COMPONENT);

			dynamics->m_acceleration = dynamics->m_force / dynamics->m_mass;
			dynamics->m_velocity = dynamics->m_velocity + dynamics->m_acceleration * (float)elapsedtime / 1000000.0f;
			dynamics->m_position = dynamics->m_position + dynamics->m_velocity;
		}
	}

}