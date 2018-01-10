#pragma once
#include "module_defines.h"

namespace SAS_3D {
	class ShaderProgram;
	class InputModule {
	public:
		virtual ~InputModule() {}
		virtual unsigned long long UniqueBits() const = 0;
		virtual void ApplyToShader(ShaderProgram* shader) = 0;
	};
}