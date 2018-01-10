#pragma once
#include "animator.h"

namespace SAS_3D {
	class AnimationContainer
	{
	public:
		AnimationContainer();

		void AddAnimation(const SceneInfo& scene);
	private:
		std::vector<Animator> _animations;
	};
}

