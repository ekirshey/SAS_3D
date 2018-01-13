#pragma once
#include "ecs/framework/component.h"

namespace SAS_3D {
	// Per instance animation data
	struct RenderComponent : Component {
		static const ComponentID ID = RENDER_COMPONENT;

		ComponentID UniqueBits() const { return ID; }

		RenderComponent()
			: modelidx(0)
		{}

		int modelidx;
	};
}