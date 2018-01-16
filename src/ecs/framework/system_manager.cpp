#include <iostream>
#include "ecs/framework/ecs_manager.h"
#include "ecs/framework/system_manager.h"
#include "ecs/framework/system.h"
#include "subsystems/subsystem_controller.h"

namespace SAS_3D {
	static const int SYSTEM_LIST_STEP_SIZE = 100;

	SystemManager::SystemManager() {
		_systemcount = 0;
		_systemlist.resize(SYSTEM_LIST_STEP_SIZE);

		for (unsigned int i = 0; i < _systemlist.size(); i++) {
			_systemlist[i] = std::unique_ptr<System>();
		}
	}

	SystemManager::~SystemManager() {
	}

	SystemID SystemManager::AddSystem(std::unique_ptr<System> system, int priority) {
		SystemID systemindex = -1;

		// TODO: Resize and shift priorities
		if (_systemlist[priority].get() == nullptr) {
			_systemlist[priority] = std::move(system);
			systemindex = _systemlist[priority]->GetUUID();
			_systemcount++;
		}
		else {
			std::cout << "System with priority " << priority << " exists" << std::endl;
		}

		return systemindex;
	}

	System* SystemManager::GetSystem(SystemID uuid) {
		System* system = nullptr;
		for (int i =0; i < _systemcount; i++) {
			if (uuid == _systemlist[i]->GetUUID()) {
				return _systemlist[i].get();
			}
		}

		return system;
	}

	void SystemManager::RemoveSystem(int systemid, int priority) {
		// TODO No rush, how often are you going to remove a system? I cant even imagine why
	}

	void SystemManager::AddEntityToSystem(EntityID entity, EntityID entitycomponents) {
		for (unsigned int i = 0; i < _systemcount; i++) {
			if (_systemlist[i]->ValidEntity(entitycomponents, _systemlist[i]->ComponentBits())) {
				_systemlist[i]->AddEntity(entity);
			}
		}
	}

	// TODO Test
	void SystemManager::RemoveEntityFromSystems(EntityID entity, EntityID entitycomponents) {
		for (unsigned int i = 0; i < _systemcount; i++) {
			if (_systemlist[i]->ContainsEntity(entity) && !_systemlist[i]->ValidEntity(entitycomponents, _systemlist[i]->ComponentBits())) {
				_systemlist[i]->RemoveEntity(entity);
			}
		}
	}

	void SystemManager::RemoveEntityFromSystems(EntityID entity) {
		for (unsigned int i = 0; i < _systemcount; i++) {
			_systemlist[i]->RemoveEntity(entity);
		}
	}

	void SystemManager::Update(int elapsedtime, SubsystemController* subsystems, EntityManager* em) {
		for (unsigned int i = 0; i < _systemcount; i++) {
			_systemlist[i]->Update(elapsedtime, subsystems, em);
		}
	}
}
