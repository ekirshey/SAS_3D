#include <algorithm>
#include <iostream>
#include "subsystems/subsystem_controller.h"
#include "entity_manager.h"
#include "system.h"

namespace SAS_3D {
	/*
	* Core of the class. It updates the time and passes all relevant information to the wrapped object
	*/
	void System::Update(long long elapsedtime, EntityManager* em, SubsystemController* subsystems) {
		SetFrameTime(elapsedtime);
		UpdateTimeRunning(elapsedtime);

		_self->Update(TimeRunning(), _relevantentities, em, subsystems);
	}

	void System::HandleCallback(Message& m) {
		_self->HandleCallback(m);
	}

	bool System::ValidEntity(EntityID entitycomponents) {
		return (_componentids & entitycomponents);
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