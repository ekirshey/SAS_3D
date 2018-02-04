#include <algorithm>
#include <iostream>
#include "ecs/framework/ecs_manager.h"
#include "ecs/framework/system_manager.h"
#include "ecs/framework/system.h"
#include "subsystems/subsystem_controller.h"

namespace SAS_3D {
	ECSManager::ECSManager() : _status(true), _errorcode(1) {

	}

	ECSManager::~ECSManager() {
		//dtor
	}


	SystemID ECSManager::AddSystem(std::unique_ptr<System> system, int priority) {
		//system->SetECSManager(this);
		return _systemmanager.AddSystem(std::move(system), priority);
	}


	System* ECSManager::GetSystem(SystemID systemid) {
		return _systemmanager.GetSystem(systemid);
	}

	EntityID ECSManager::CreateEntity() {
		return _entitymanager.GetNewEntityUUID();
	}

	void ECSManager::RemoveEntity(EntityID entity) {
		std::vector<std::string> entitytags;
		_entitymanager.RemoveEntity(entity);
		_systemmanager.RemoveEntityFromSystems(entity);

		entitytags = GetAssociatedTags(entity);

		for (unsigned int i = 0; i < entitytags.size(); i++) {
			RemoveEntityFromTag(entity, entitytags[i]);
		}
	}

	std::vector<Component*> ECSManager::GetAllEntityComponents(EntityID entity) {
		return _entitymanager.GetAllEntityComponents(entity);
	}

	void ECSManager::AddComponentToEntity(EntityID entity, std::unique_ptr<Component> componenttoadd) {
		if (_entitymanager.AddComponent(entity, std::move(componenttoadd))) {
			_systemmanager.AddEntityToSystem(entity, _entitymanager.GetEntityComponentBits(entity));
		}
	}


	void ECSManager::RemoveComponentFromEntity(EntityID entity, EntityID componentid) {
		if (_entitymanager.RemoveComponent(entity, componentid)) {
			_systemmanager.RemoveEntityFromSystems(entity, _entitymanager.GetEntityComponentBits(entity));
		}
	}

	void ECSManager::Update(long long elapsedtime, SubsystemController* subsystems) {
		_systemmanager.Update(elapsedtime, subsystems, &this->_entitymanager);
	}

	// Tag functions
	void ECSManager::AssignEntityTag(EntityID entity, std::string tag) {
		_tagmanager[tag].push_back(entity);
	}

	void ECSManager::RemoveEntityFromTag(EntityID entity, std::string tag) {
		auto result = std::find(_tagmanager[tag].begin(), _tagmanager[tag].end(), entity);

		if (result != std::end(_tagmanager[tag])) {
			_tagmanager[tag].erase(result);
		}
	}

	void ECSManager::RemoveTag(std::string tag) {
		_tagmanager.erase(tag);
	}

	void ECSManager::AddTag(std::string tag) {
		if (_tagmanager.find(tag) == _tagmanager.end()) {
			_tagmanager[tag] = std::vector<EntityID>();
		}
	}

	std::vector<std::string> ECSManager::GetAssociatedTags(EntityID entity) {
		std::vector<std::string> tags;
		for (auto it : _tagmanager) {
			auto result = std::find(it.second.begin(), it.second.end(), entity);
			if (result != std::end(it.second)) {
				tags.push_back(it.first);
			}
		}

		return tags;
	}

	void ECSManager::ClearTagVector(std::string tag) {
		_tagmanager[tag].clear();
	}

	std::vector<EntityID> ECSManager::GetAssociatedEntities(std::string tag) {
		return _tagmanager[tag];
	}
}
