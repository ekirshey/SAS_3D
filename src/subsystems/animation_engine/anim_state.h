#pragma once
#include <vector>
#include <tuple>
#include <glm/glm.hpp>

namespace SAS_3D {
	using BoneMatrix = std::vector<std::vector<glm::mat4>>;
	struct AnimationState {
		AnimationState()
			: id(0)
			, index(0)
			, internalid(0)
			, frameindices{ 0,0,0 }
		{}

		int id;
		int index;
		int internalid;	// id associated with 
		std::tuple<int, int, int> frameindices;
		BoneMatrix bones;
	};
}
