/*
* base_shader.h
*
* Very simple vertex and fragment shader.
* Used when you just want to get vertices on the screen
*
* Author: Erik Kirshey
* Date: 10/4/17
*/
#pragma once
#include "shaders/shader_program.h"

namespace SAS_3D {
	namespace Shaders {
		class BaseShader : public ShaderProgram {
			public:
				BaseShader();
				~BaseShader();
		};

		void ApplyShader(  BaseShader& bs
						 , const glm::mat4& model
						 , const glm::mat4& view
						 , const glm::mat4& projection);
	}
}