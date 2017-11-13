/*
* texture_shader.h
*
* shader for handling textures
*
* Author: Erik Kirshey
* Date: 11/11/17
*/
#pragma once
#include "shaders/shader_program.h"

namespace SAS_3D {
	namespace Shaders {
		class TextureShader : public ShaderProgram {
		public:
			TextureShader(std::string shader_root_path);
			~TextureShader();
			void ApplyTexture(unsigned int texture_unit) const;
		};

	}
}