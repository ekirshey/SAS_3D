#pragma once
#include <vector>
#include <functional>

namespace SAS_3D {
	template<typename T>
	class Subject {
	public:
		void RegisterObserver(std::function<void(const T&)> obs) {
			_observers.push_back(obs);
		}

		void SignalObservers(const T& subject) {
			for (auto& o : _observers) {
				o(subject);
			}
		}

	private:
		std::vector<std::function<void(const T&)>> _observers;
	};
}
