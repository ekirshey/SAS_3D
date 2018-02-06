#pragma once
#include <memory>
#include <vector>
#include <unordered_set>
#include "ecs/framework/component.h"

#define MAXOBJECTS 100000

namespace SAS_3D {
	inline int fastlog(EntityID i) {
		int result = 0;
		while (i >>= 1) result++;
		return result;
	}

	class EntityManager {
	public:
		EntityManager();
		virtual ~EntityManager();

		EntityID GetNewEntityUUID();
		void RemoveEntity(EntityID UUID);

		bool AddComponent(EntityID UUID, std::unique_ptr<Component> componenttoadd);
		bool RemoveComponent(EntityID UUID, EntityID componentid);

		EntityID EntityCount() { return _entitycount; }

		template<typename T>
		T* GetEntityComponent(EntityID UUID, EntityID componentid)
		{
			int convertedcomponentid = fastlog(componentid);
			if (_entitylist[UUID][convertedcomponentid] != nullptr) {
				return static_cast<T*>(_entitylist[UUID][convertedcomponentid].get());
			}
			else {
				return nullptr;
			}

		}

		std::vector<Component*> GetAllEntityComponents(EntityID UUID);

		EntityID GetEntityComponentBits(EntityID UUID) { return _entitycomponentbits[UUID]; }

	private:
		// The index of the vector will be the UUID of the entity
		// The component vector will have a size of 64
		// I'll know what slot to place an added component in by ddoing logbase2 of UniqueBits returned from the component
		std::vector<std::unique_ptr<Component>> _entitylist[MAXOBJECTS];    // unique_ptr due to object slicing
		EntityID _entitycomponentbits[MAXOBJECTS];                     // Keeps a running update of whcih components are in the entity
																			 // Used specifically for adding entities to systems
		std::unordered_set<EntityID> _availableids;
		EntityID _entitycount;
		int _entitylistsize;
	};

	// Helper function for accessing components from an entity
}
