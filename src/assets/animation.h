#pragma once
#include <unordered_map>

namespace SAS_3D {
	class Animation {
	public:
		Animation();
	private:
		std::unordered_map<std::string, int> _bonemap;
	};
}
