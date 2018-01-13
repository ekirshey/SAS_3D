#pragma once
#include <vector>
#include <memory>
#include <cstdint>
#include "ecs/ecs_defines.h"

namespace SAS_3D {
	// Forward Declarations
	class System;
	class EntityManager;
	class SubsystemController;

	class SystemManager
	{
	public:
		SystemManager();
		virtual ~SystemManager();

		int AddSystem(std::unique_ptr<System> system, int priority);
		System* GetSystem(int priority);
		void RemoveSystem(int systemid, int priority);

		void AddEntityToSystem(EntityID entity, EntityID entitycomponents);

		void RemoveEntityFromSystems(EntityID entity);
		void RemoveEntityFromSystems(EntityID entity, EntityID entitycomponents);

		void Update(int elapsedtime, SubsystemController* subsystems, EntityManager* em);
	private:
		std::vector<std::unique_ptr<System>> _systemlist;
		EntityID _systemcount;

	};
}
