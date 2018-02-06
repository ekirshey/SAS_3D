#include <iostream>
#include "ecs/framework/ecs_manager.h"
#include "ecs/framework/system_manager.h"
#include "subsystems/subsystem_controller.h"

namespace SAS_3D {

	SystemManager::SystemManager() {

	}

	SystemManager::~SystemManager() {
	}

	void SystemManager::CallSystemCallback(SystemID id, Message& message) {
		for (auto& system : _systemlist) {
			if (id == system.GetID()) {
				system.HandleCallback(message);
			}
		}
	}

	void SystemManager::AddEntityToSystem(EntityID entity, EntityID entitycomponents) {
		for (auto& system : _systemlist) {
			if (system.ValidEntity(entitycomponents)) {
				system.AddEntity(entity);
			}
		}
	}

	// TODO Test. I don't think this logically works...
	void SystemManager::RemoveEntityFromSystems(EntityID entity, EntityID entitycomponents) {
		for (auto& system : _systemlist) {
			if (system.ContainsEntity(entity) && !system.ValidEntity(entitycomponents)) {
				system.RemoveEntity(entity);
			}
		}
	}

	void SystemManager::RemoveEntityFromSystems(EntityID entity) {
		for (auto& system : _systemlist) {
			system.RemoveEntity(entity);
		}
	}

	void SystemManager::Update(long long elapsedtime, EntityManager* em, SubsystemController* subsystems) {
		for (auto& system: _systemlist) {
			system.Update(elapsedtime, em, subsystems);
		}
	}
}
