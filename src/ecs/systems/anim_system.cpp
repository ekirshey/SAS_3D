#include <thread>
#include "anim_system.h"

namespace SAS_3D {
	AnimationSystem::AnimationSystem(std::string systemname) {

	}

	AnimationSystem::~AnimationSystem() {

	}

	void AnimationSystem::ProcessEntity(int elapsedtime, SubsystemController* subsystems, EntityManager* em, EntityID entity) {

	}

	void AnimationSystem::BeforeEntityProcessing(SubsystemController* subsystems) {

	}

	void AnimationSystem::AfterEntityProcessing(SubsystemController* subsystems) {
		// Check if it's the frame before we need to calculate new bone matrices

	}
}