#pragma once
#include "ecs/framework/component.h"

namespace SAS_3D {
	// Per instance animation data
	struct AnimationComponent : Component {
		static const ComponentID ID = ANIMATION_COMPONENT;

		ComponentID UniqueBits() const { return ID; }

		AnimationComponent() 
			: animationstate(0)
		{}

		int animationstate;
	};
}