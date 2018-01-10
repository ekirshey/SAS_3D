#include "anim_container.h"


namespace SAS_3D {
	AnimationContainer::AnimationContainer() {
	}

	void AnimationContainer::AddAnimation(const SceneInfo& scene) {
		_animations.push_back(Animator(&scene));
	}

}