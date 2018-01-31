#pragma once
#include "module_defines.h"

namespace SAS_3D {
	class ShaderProgram;
	using InputID = unsigned long long;
	class InputModule {
	public:
		virtual ~InputModule() {}
		virtual InputID UniqueBits() const = 0;
		virtual void ApplyToShader(ShaderProgram* shader) = 0;
	};


	/*
	Alternative:

	string type pair
	tuple<pair<string, type>...>
	
	*/
}