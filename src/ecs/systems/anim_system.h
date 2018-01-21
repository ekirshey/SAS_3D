#pragma once
#include <vector>
#include "ecs/framework/system.h"
#include "subsystems/animation_engine/anim_state.h"

namespace SAS_3D {
	class AnimationSystem : public System
	{
	public:
		static const EntityID COMPONENTIDS = ANIMATION_COMPONENT;

		AnimationSystem(std::string systemname, SystemID uuid);
		~AnimationSystem();

		EntityID ComponentBits() { return AnimationSystem::COMPONENTIDS; }    // TODO: Is this necessary or is just accessing the variable directly better?

		void BeforeEntityProcessing(int elapsedtime, SubsystemController* subsystems);
		void ProcessEntity(SubsystemController* subsystems, EntityManager* em, EntityID entity);
		void AfterEntityProcessing(SubsystemController* subsystems);

	private:
		double _time;
		std::vector<AnimationState> _animations;
	};

}