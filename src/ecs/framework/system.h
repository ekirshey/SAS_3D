#pragma once

#include <cstdint>
#include <vector>
#include <queue>
#include <string>

namespace SAS_3D {
	class ECSManager;

	class System
	{
	public:
		static const unsigned long long COMPONENTIDS = 0x0;
		System();
		System(std::string systemname, ECSManager* ecsmanager);

		virtual ~System() {}

		void Update(int elapsedtime);
		virtual void ProcessEntities() = 0;

		virtual unsigned long long ComponentBits() { return COMPONENTIDS; }

		virtual void AddEntity(unsigned long long entityid);
		virtual void RemoveEntity(unsigned long long entityid);

		bool ContainsEntity(unsigned long long entityid);    // TODO Not sure if needed
		bool ValidEntity(unsigned long long componentbits, unsigned long long SYSTEMID);

		ECSManager* GetECSManager() { return _ecsmanager; }

		unsigned long long GetEntityCount() { return _entitycount; }
		void EntityCount(unsigned long long entitycount) { _entitycount = entitycount; }

		std::vector<unsigned long long> RelevantEntities();

		int FrameTime() { return _frametime; }
		void SetFrameTime(int frametime) { _frametime = frametime; }

		int TimeRunning() { return _elapsedtime; }
		void UpdateTimeRunning(int runtime) { _elapsedtime += runtime; }

		std::string SystemName() { return _systemname; }
		void SetSystemName(std::string name) { _systemname = name; }

		template<typename T>
		T GetEntityComponent(unsigned long long entityID, unsigned long long componentID) {
			return _ecsmanager->GetEntityComponent<T>(entityID, componentID);
		}

	private:
		std::string _systemname;	// For debugging purposes
		ECSManager* _ecsmanager;
		int _elapsedtime;
		int _frametime;
		unsigned long long _entitycount;
		std::vector<unsigned long long> _relevantentities;
	};
}