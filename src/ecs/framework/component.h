#pragma once

namespace SAS_3D {
	class Component
	{
	public:
		virtual ~Component() {}
		virtual unsigned long long UniqueBits() const = 0;
	};
}

