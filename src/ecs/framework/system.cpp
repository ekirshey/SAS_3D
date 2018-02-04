#include <algorithm>
#include <iostream>
#include "ecs/framework/system.h"
#include "subsystems/subsystem_controller.h"

namespace SAS_3D {
	System::System(std::string systemname, SystemID uuid) 
		: _systemname(systemname)
		, _uuid(uuid)
		, _elapsedtime(0)
		, _frametime(0)
		, _entitycount(0) 
	{
		std::cout << "Name: " << systemname << " ID: " << uuid << std::endl;
	}

	void System::Update(long long elapsedtime, SubsystemController* subsystems, EntityManager* em) {
		SetFrameTime(elapsedtime);
		UpdateTimeRunning(elapsedtime);

		BeforeEntityProcessing(subsystems);
		for (unsigned int i = 0; i < _relevantentities.size(); i++) {
			ProcessEntity(subsystems, em, _relevantentities[i]);
		}
		AfterEntityProcessing(subsystems);
	}

	bool System::ValidEntity(EntityID componentbits, EntityID SYSTEMID) {
		return ((componentbits & SYSTEMID) == SYSTEMID);
	}

	void System::AddEntity(EntityID entityid) {
		// TODO: BE MORE CLEVER maybe find a way that you dont have to loop thru every time you add/remove a component
		if (!ContainsEntity(entityid)) {
			_relevantentities.push_back(entityid);
			_entitycount++;
		}
	}

	void System::RemoveEntity(EntityID entityid) {
		if (_relevantentities.size() > 0) {
			auto result = std::find(_relevantentities.begin(), _relevantentities.end(), entityid);

			if (result != std::end(_relevantentities)) {
				_relevantentities.erase(result);
				_entitycount--;
			}

		}
	}

	bool System::ContainsEntity(EntityID entityid) {
		if (std::find(_relevantentities.begin(), _relevantentities.end(), entityid) != _relevantentities.end()) {
			return true;
		}
		else {
			return false;
		}
	}
}