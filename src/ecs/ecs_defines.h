#pragma once

namespace SAS_3D {
	using EntityID = unsigned long long;
	using ComponentID = unsigned long long;
	using SystemID = unsigned long long;

	// Component defines
	#define DYNAMICS_COMPONENT			0x01
	#define STATIC_COMPONENT			0x02
	#define ANIMATION_COMPONENT			0x04
	#define RENDER_COMPONENT			0x08
	#define LIGHT_COMPONENT				0x10
}
