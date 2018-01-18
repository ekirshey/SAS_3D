#pragma once
#include "ecs/framework/component.h"
#include "subsystems/animation_engine/anim_engine.h"

namespace SAS_3D {
	// Per instance animation data
	struct AnimationComponent : Component {
		static const ComponentID ID = ANIMATION_COMPONENT;

		ComponentID UniqueBits() const { return ID; }

		AnimationComponent() {}

		AnimationState animationstate;
	};
}