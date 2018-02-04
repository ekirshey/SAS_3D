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

		void Update(long long elapsedtime, SubsystemController* subsystems, EntityManager* em);
		SystemID GetUUID() const { return _uuid; }

		virtual void BeforeEntityProcessing(SubsystemController* subsystems) = 0;
		virtual void ProcessEntity(SubsystemController* subsystems, EntityManager* em, EntityID entity) = 0;
		virtual void AfterEntityProcessing(SubsystemController* subsystems) = 0;

		virtual EntityID ComponentBits() { return COMPONENTIDS; }

		virtual void AddEntity(EntityID entityid);
		virtual void RemoveEntity(EntityID entityid);

		bool ContainsEntity(EntityID entityid);    // TODO Not sure if needed
		bool ValidEntity(EntityID componentbits, EntityID SYSTEMID);

		EntityID GetEntityCount() const { return _entitycount; }
		void EntityCount(EntityID entitycount) { _entitycount = entitycount; }

		long long FrameTime() const { return _frametime; }
		void SetFrameTime(long long frametime) { _frametime = frametime; }

		long long TimeRunning() const { return _elapsedtime; }
		void UpdateTimeRunning(long long runtime) { _elapsedtime += runtime; }

		std::string SystemName() const { return _systemname; }
		void SetSystemName(std::string name) { _systemname = name; }

		template<typename T>
		T* GetEntityComponent(EntityManager* em, EntityID entityID, ComponentID componentID) {
			return static_cast<T*>(em->GetEntityComponent(entityID, componentID));
		}

	private:
		std::string _systemname;	// For debugging purposes
		SystemID _uuid;
		long long _elapsedtime;
		long long _frametime;
		EntityID _entitycount;
		std::vector<EntityID> _relevantentities;
	};
}