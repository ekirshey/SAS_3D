#pragma once
#include <memory>
#include <vector>
#include <unordered_set>
#include "ecs/framework/component.h"

#define MAXOBJECTS 100000

namespace SAS_3D {
	inline int fastlog(unsigned long long i) {
		int result = 0;
		while (i >>= 1) result++;
		return result;
	}

	class EntityManager {
	public:
		EntityManager();
		virtual ~EntityManager();

		unsigned long long GetNewEntityUUID();
		void RemoveEntity(unsigned long long UUID);

		bool AddComponent(unsigned long long UUID, std::unique_ptr<Component> componenttoadd);
		bool RemoveComponent(unsigned long long UUID, unsigned long long componentid);

		unsigned long long EntityCount() { return _entitycount; }

		inline Component* EntityManager::GetEntityComponent(unsigned long long UUID, unsigned long long componentid)
		{
			int convertedcomponentid = fastlog(componentid);
			if (_entitylist[UUID][convertedcomponentid] != nullptr) {
				return _entitylist[UUID][convertedcomponentid].get();
			}
			else {
				return nullptr;
			}

		}

		std::vector<Component*> GetAllEntityComponents(unsigned long long UUID);

		unsigned long long GetEntityComponentBits(unsigned long long UUID) { return _entitycomponentbits[UUID]; }

	private:
		// The index of the vector will be the UUID of the entity
		// The component vector will have a size of 64
		// I'll know what slot to place an added component in by ddoing logbase2 of UniqueBits returned from the component
		std::vector<std::unique_ptr<Component>> _entitylist[MAXOBJECTS];    // unique_ptr due to object slicing
		unsigned long long _entitycomponentbits[MAXOBJECTS];                     // Keeps a running update of whcih components are in the entity
																			 // Used specifically for adding entities to systems
		std::unordered_set<unsigned long long> _availableids;
		unsigned long long _entitycount;
		int _entitylistsize;
	};
}
