#include <algorithm>
#include <iostream>
#include "ecs/framework/system.h"

namespace SAS_3D {
	System::System() : _systemname(""), _ecsmanager(nullptr), _elapsedtime(0), _frametime(0), _entitycount(0) {

	}

	System::System(std::string systemname, ECSManager* ecsmanager) : _systemname(systemname), _ecsmanager(ecsmanager), _elapsedtime(0), _frametime(0), _entitycount(0) {

	}

	void System::Update(int elapsedtime) {
		SetFrameTime(elapsedtime);
		UpdateTimeRunning(elapsedtime);

		// Here is where the magic happens
		ProcessEntities();
	}

	bool System::ValidEntity(unsigned long long componentbits, unsigned long long SYSTEMID) {
		return ((componentbits & SYSTEMID) == SYSTEMID);
	}

	void System::AddEntity(unsigned long long entityid) {
		// TODO: BE MORE CLEVER maybe find a way that you dont have to loop thru every time you add/remove a component
		if (!ContainsEntity(entityid)) {
			_relevantentities.push_back(entityid);
			_entitycount++;
		}
	}

	std::vector<unsigned long long> System::RelevantEntities() {
		return _relevantentities;
	}

	void System::RemoveEntity(unsigned long long entityid) {
		if (_relevantentities.size() > 0) {
			auto result = std::find(_relevantentities.begin(), _relevantentities.end(), entityid);

			if (result != std::end(_relevantentities)) {
				_relevantentities.erase(result);
				_entitycount--;
			}

		}
	}

	bool System::ContainsEntity(unsigned long long entityid) {
		if (std::find(_relevantentities.begin(), _relevantentities.end(), entityid) != _relevantentities.end()) {
			return true;
		}
		else {
			return false;
		}
	}
}