#pragma once
#include <chrono>

namespace SAS_3D {
	inline unsigned long long GenerateUUID() {
		// For now I'm going to use microseconds since epoch. 
		// I'll have to look into this more
		auto t = std::chrono::system_clock::now();
		return std::chrono::duration_cast<std::chrono::microseconds>(t.time_since_epoch()).count();
	}
}