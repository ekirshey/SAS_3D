#pragma once
#include <vector>
#include <tuple>
#include <glm/glm.hpp>

namespace SAS_3D {
	using BoneMatrix = std::vector<std::vector<glm::mat4>>;
	using FrameIndices = std::vector<std::tuple<int, int, int>>;
	struct AnimationState {
		AnimationState()
			: id(0)
			, index(0)
			, internalid(0)
		{}

		int id;
		int index;
		int internalid;	// id associated with 
		FrameIndices frameindices;
		BoneMatrix bones;
	};
}
