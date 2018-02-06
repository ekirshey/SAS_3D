#pragma once
#include <vector>
#include <memory>
#include <cstdint>
#include "System.h"
#include "ecs/ecs_defines.h"

namespace SAS_3D {
	// Forward Declarations
	class EntityManager;
	class SubsystemController;

	class SystemManager
	{
	public:
		SystemManager();
		virtual ~SystemManager();

		template<typename T>
		SystemID AddSystem(T x, std::string systemname, int componentids) {
			_systemlist.emplace_back(System(x, systemname, componentids));
			return _systemlist.back().GetID();
		}

		void CallSystemCallback(SystemID id, Message& message);

		void AddEntityToSystem(EntityID entity, EntityID entitycomponents);

		void RemoveEntityFromSystems(EntityID entity);
		void RemoveEntityFromSystems(EntityID entity, EntityID entitycomponents);

		void Update(long long elapsedtime, EntityManager* em, SubsystemController* subsystems);
	private:
		std::vector<System> _systemlist;
		EntityID _systemcount;

	};
}
