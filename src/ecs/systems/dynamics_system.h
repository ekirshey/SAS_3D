#pragma once
#include <vector>
#include "subsystems/animation_engine/anim_state.h"
#include "ecs/ecs_defines.h"
#include "ecs/framework/message.h"

namespace SAS_3D {
	class EntityManager;
	class SubsystemController;

	class DynamicsSystem
	{
	public:
		DynamicsSystem();
		~DynamicsSystem();

		void Update(long long elapsedtime,
			const std::vector<EntityID>& entities,
			EntityManager* em,
			SubsystemController* subsystems);

		void HandleCallback(const Message m) {}
	};
}