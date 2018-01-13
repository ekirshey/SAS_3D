#include <cmath>
#include <iostream> //Debugging
#include <chrono>
#include "ecs/framework/entity_manager.h"

namespace SAS_3D {
	static const int MAX_COMPONENT_LIST = 64;

	EntityManager::EntityManager() : _entitylistsize(MAXOBJECTS) {
		_entitycount = 0;

		for (unsigned int i = 0; i < _entitylistsize; i++) {
			_entitylist[i] = std::vector<std::unique_ptr<Component>>();
			_entitylist[i].resize(MAX_COMPONENT_LIST);
			_entitycomponentbits[i] = 0;
		}

	}

	EntityManager::~EntityManager() {
	}

	EntityID EntityManager::GetNewEntityUUID() {
		EntityID entityid = 0;

		if (!_availableids.empty()) {
			entityid = *_availableids.begin();
			_availableids.erase(_availableids.begin());

			// Reset the component vector TODO probably not an issue now but idk if this is slow
			_entitylist[entityid] = std::vector<std::unique_ptr<Component>>();
			_entitylist[entityid].resize(MAX_COMPONENT_LIST);
		}
		else {
			entityid = _entitycount++;
		}

		return entityid;
	}

	void EntityManager::RemoveEntity(EntityID UUID) {
		// EKTEMP I think if you are moving new unique_ptr's into it then you dont need to clear
		//_entitylist[UUID].clear();
		_entitycomponentbits[UUID] = 0;

		_availableids.insert(UUID);
	}

	// If a component is ADDED to an entity then the SystemManager needs to be made aware
	// so it can ADD the entity to the appropriate system if it NOW meets the conditions
	bool EntityManager::AddComponent(EntityID UUID, std::unique_ptr<Component> componenttoadd) {
		int componentid;
		bool returnvalue = false;

		// Make sure if the entity is within the vector bounds and that it is an actual entity
		if ((UUID < MAXOBJECTS) && (_entitylist[UUID].size() > 0)) {
			if (!(_entitycomponentbits[UUID] & componenttoadd->UniqueBits())) {
				componentid = fastlog(componenttoadd->UniqueBits());
				_entitycomponentbits[UUID] |= componenttoadd->UniqueBits();

				// I'm moving the pointer so either eaccess the entitylist or do this last if you need the ID bits
				_entitylist[UUID].at(componentid) = std::move(componenttoadd);

				returnvalue = true;
			}
			else {
				std::cout << "Duplicate Component ID " << componenttoadd->UniqueBits() << std::endl;
			}
		}
		else {
			std::cout << "Adding Component " << componenttoadd->UniqueBits() << " to UUID " << UUID << " failed, Entity does not exist! " << _entitylistsize << std::endl;
		}

		return returnvalue;
	}

	// If a component is REMOVED from an entity then the SystemManager needs to be made aware
	// so it can REMOVE the entity to the appropriate system if it NO LONGER meets the conditions
	bool EntityManager::RemoveComponent(EntityID UUID, EntityID componentid) {
		// Remove component from an entity
		_entitylist[UUID].at(fastlog(componentid)) = nullptr;
		_entitycomponentbits[UUID] &= (componentid ^ 0xFFFFFFFFFFFFFFFF);

		return true;
	}

	std::vector<Component*> EntityManager::GetAllEntityComponents(EntityID UUID) {
		std::vector<Component*> components;

		for (unsigned int i = 0; i < _entitylist[UUID].size(); i++) {
			if (_entitylist[UUID][i] != nullptr) {
				components.push_back(_entitylist[UUID].at(i).get());
			}
		}

		return components;
	}
}