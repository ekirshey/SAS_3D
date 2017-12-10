#pragma once
#include <vector>
#include <memory>
#include <cstdint>

namespace SAS_3D {
	// Forward Declarations
	class System;
	class ECSManager;

	class SystemManager
	{
	public:
		SystemManager();
		virtual ~SystemManager();

		int AddSystem(std::unique_ptr<System> system, int priority);
		System* GetSystem(int priority);
		void RemoveSystem(int systemid, int priority);

		void AddEntityToSystem(unsigned long long entity, unsigned long long entitycomponents);

		void RemoveEntityFromSystems(unsigned long long entity);
		void RemoveEntityFromSystems(unsigned long long entity, unsigned long long entitycomponents);

		void Update(int elapsedtime);
	private:
		std::vector<std::unique_ptr<System>> _systemlist;
		unsigned long long _systemcount;

	};
}
