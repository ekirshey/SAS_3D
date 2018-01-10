#include <algorithm>
#include <iostream>
#include "ecs/framework/ecs_manager.h"
#include "ecs/framework/system_manager.h"
#include "ecs/framework/system.h"

namespace SAS_3D {
	ECSManager::ECSManager() : _status(true), _errorcode(1) {

	}

	ECSManager::~ECSManager() {
		//dtor
	}


	int ECSManager::AddSystem(std::unique_ptr<System> system, int priority) {
		//system->SetECSManager(this);
		return _systemmanager.AddSystem(std::move(system), priority);
	}


	System* ECSManager::GetSystem(int systemid) {
		return _systemmanager.GetSystem(systemid);
	}

	unsigned long long ECSManager::CreateEntity() {
		return _entitymanager.GetNewEntityUUID();
	}

	void ECSManager::RemoveEntity(unsigned long long entity) {
		std::vector<std::string> entitytags;
		_entitymanager.RemoveEntity(entity);
		_systemmanager.RemoveEntityFromSystems(entity);

		entitytags = GetAssociatedTags(entity);

		for (unsigned int i = 0; i < entitytags.size(); i++) {
			RemoveEntityFromTag(entity, entitytags[i]);
		}
	}

	std::vector<Component*> ECSManager::GetAllEntityComponents(unsigned long long entity) {
		return _entitymanager.GetAllEntityComponents(entity);
	}

	void ECSManager::AddComponentToEntity(unsigned long long entity, std::unique_ptr<Component> componenttoadd) {
		if (_entitymanager.AddComponent(entity, std::move(componenttoadd))) {
			_systemmanager.AddEntityToSystem(entity, _entitymanager.GetEntityComponentBits(entity));
		}
	}


	void ECSManager::RemoveComponentFromEntity(unsigned long long entity, unsigned long long componentid) {
		if (_entitymanager.RemoveComponent(entity, componentid)) {
			_systemmanager.RemoveEntityFromSystems(entity, _entitymanager.GetEntityComponentBits(entity));
		}
	}

	void ECSManager::Update(int elapsedtime) {
		_systemmanager.Update(elapsedtime, &this->_entitymanager);
	}

	// Tag functions
	void ECSManager::AssignEntityTag(unsigned long long entity, std::string tag) {
		_tagmanager[tag].push_back(entity);
	}

	void ECSManager::RemoveEntityFromTag(unsigned long long entity, std::string tag) {
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
			_tagmanager[tag] = std::vector<unsigned long long>();
		}
	}

	std::vector<std::string> ECSManager::GetAssociatedTags(unsigned long long entity) {
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

	std::vector<unsigned long long> ECSManager::GetAssociatedEntities(std::string tag) {
		return _tagmanager[tag];
	}
}
