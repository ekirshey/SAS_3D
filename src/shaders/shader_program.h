/*
* shader_program.h
*
* Interface for shader programs
*
* Author: Erik Kirshey
* Date: 10/4/17
*/
#pragma once
#include <string>
#include "assets/model.h"

namespace SAS_3D {
	namespace Shaders {
		class ShaderProgram {
			public:
				ShaderProgram(std::string vertexfile, std::string fragfile);
				~ShaderProgram();

				void UseProgram();
				GLint GetUniformLocation(std::string name);
			private:
				GLuint _createShader(const std::string &filename, GLenum shadertype);
				bool _compileShader(GLuint shader);

				GLuint _shaderprogram;
		};
	}
}
