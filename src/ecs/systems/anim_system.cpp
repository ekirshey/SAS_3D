#include "anim_system.h"
#include "subsystems/subsystem_controller.h"
#include "ecs/framework/entity_manager.h"
#include "ecs/components/anim_component.h"

namespace SAS_3D {
	AnimationSystem::AnimationSystem(std::string systemname, SystemID uuid)
		: System(systemname, uuid)
	{
	}

	AnimationSystem::~AnimationSystem() {

	}

	void AnimationSystem::BeforeEntityProcessing(SubsystemController* subsystems) {
		auto animengine = subsystems->GetAnimationEngine();
		_animations = std::move(animengine->CollectBoneCalculations());
	}

	void AnimationSystem::ProcessEntity(SubsystemController* subsystems, EntityManager* em, EntityID entity) {
		auto animengine = subsystems->GetAnimationEngine();
		auto anim_comp = GetEntityComponent<AnimationComponent>(em, entity, ANIMATION_COMPONENT);
		AnimationState* comp_state = &anim_comp->animationstate;
		if (_animations.size() > 0) {
			if (comp_state->internalid >= 0) {
				comp_state->bones = std::move(_animations[comp_state->internalid].bones);
			}
			else {
				std::cout << "Invalid internalid for entity " << entity << std::endl;
			}
		}

		// Do some animation index processing
		comp_state->id = 0;
		comp_state->index = 0;

		// Add state info to anim engine
		comp_state->internalid = animengine->AddAnimationInstance(comp_state->id, comp_state->index, comp_state->frameindices);		
	}

	void AnimationSystem::AfterEntityProcessing(SubsystemController* subsystems) {
		auto animengine = subsystems->GetAnimationEngine();
		/* 
			I might need to add the amount of time it took to add the animation states.
			Not sure if it's inconsequential
		*/
		animengine->StartAsyncBoneCalculations(TimeRunning()/1000.0);
	}
}