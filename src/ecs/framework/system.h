#pragma once

#include <cstdint>
#include <vector>
#include <queue>
#include <string>
#include "ecs/ecs_defines.h"

namespace SAS_3D {
	class EntityManager;
	class SubsystemController;

	class System
	{
	public:
		static const EntityID COMPONENTIDS = 0x0;
		System(std::string systemname, SystemID uuid);

		virtual ~System() {}

		void Update(int elapsedtime, SubsystemController* subsystems, EntityManager* em);
		SystemID GetUUID() const { return _uuid; }

		virtual void BeforeEntityProcessing(SubsystemController* subsystems) = 0;
		virtual void ProcessEntity(SubsystemController* subsystems, EntityManager* em, EntityID entity) = 0;
		virtual void AfterEntityProcessing(SubsystemController* subsystems) = 0;

		virtual EntityID ComponentBits() { return COMPONENTIDS; }

		virtual void AddEntity(EntityID entityid);
		virtual void RemoveEntity(EntityID entityid);

		bool ContainsEntity(EntityID entityid);    // TODO Not sure if needed
		bool ValidEntity(EntityID componentbits, EntityID SYSTEMID);

		EntityID GetEntityCount() { return _entitycount; }
		void EntityCount(EntityID entitycount) { _entitycount = entitycount; }

		int FrameTime() { return _frametime; }
		void SetFrameTime(int frametime) { _frametime = frametime; }

		int TimeRunning() { return _elapsedtime; }
		void UpdateTimeRunning(int runtime) { _elapsedtime += runtime; }

		std::string SystemName() { return _systemname; }
		void SetSystemName(std::string name) { _systemname = name; }

		template<typename T>
		T* GetEntityComponent(EntityManager* em, EntityID entityID, ComponentID componentID) {
			return static_cast<T*>(em->GetEntityComponent(entityID, componentID));
		}

	private:
		std::string _systemname;	// For debugging purposes
		SystemID _uuid;
		int _elapsedtime;
		int _frametime;
		EntityID _entitycount;
		std::vector<EntityID> _relevantentities;
	};
}