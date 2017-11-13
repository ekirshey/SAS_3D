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
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace SAS_3D {
	namespace Shaders {
		class ShaderProgram {
			public:
				ShaderProgram(std::string vertexfile, std::string fragfile);
				~ShaderProgram();

				void UseProgram() const;
				GLint GetUniformLocation(std::string name) const;

				void ApplyMVP( const glm::mat4& model
							 , const glm::mat4& view
							 , const glm::mat4& projection) const;
			private:
				GLuint _createShader(const std::string &filename, GLenum shadertype);
				bool _compileShader(GLuint shader);

				GLuint _shaderprogram;
		};
	}
}
