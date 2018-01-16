#pragma once

namespace SAS_3D {
	using EntityID = unsigned long long;
	using ComponentID = unsigned long long;
	using SystemID = unsigned long long;

	// Component defines
	#define PHYSICAL_COMPONENT			0x01
	#define ANIMATION_COMPONENT			0x02
	#define RENDER_COMPONENT			0x04
}
