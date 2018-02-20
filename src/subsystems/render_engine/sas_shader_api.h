#pragma once
/*
Class describing the acceptable unfiform values and attributes that 
can be used in a shader.

It seems its easier to enforce convention on a set of arguments rather
than make this all encompassing generic shader object
*/

// Everything I look at, suggests models have materials which hold which shaders
// that they use. I don't do that. Right now I have pretty global shader lists.
// Also how do you have multiple material shaders per object?
// Shouldn't be difficult to implement considering we only use an index to a model
// which in turn has it's list of materials
namespace SAS_3D {
	class SASShaderAPI {

	};
}
