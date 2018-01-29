#pragma once
#include "ecs/framework/component.h"
#include "subsystems/render_engine/scene.h"

namespace SAS_3D {
	// Per instance animation data
	struct LightComponent : Component {
		static const ComponentID ID = LIGHT_COMPONENT;

		ComponentID UniqueBits() const { return ID; }

		LightComponent() {}
		LightComponent(const Light& light) : m_light(light) {}

		Light m_light;
	};
}