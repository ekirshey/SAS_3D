#pragma once
#include <vector>
#include <functional>
#include "message.h"

namespace SAS_3D {
	class Subject {
	public:
		void RegisterObserver(std::function<void(Message&)> obs) {
			_observers.push_back(obs);
		}

		void SignalObservers(Message& subject) {
			for (auto& o : _observers) {
				o(subject);
			}
		}

	private:
		std::vector<std::function<void(Message&)>> _observers;
	};
}
