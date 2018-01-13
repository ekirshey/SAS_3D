#pragma once
#include "ecs/ecs_defines.h"

namespace SAS_3D {
	class Component
	{
	public:
		virtual ~Component() {}
		virtual ComponentID UniqueBits() const = 0;
	};
}

