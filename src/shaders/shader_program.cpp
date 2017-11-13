#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include "core/file_io.h"
#include "core/error_codes.h"
#include "shaders/shader_program.h"

namespace SAS_3D {
	namespace Shaders {
		ShaderProgram::ShaderProgram(std::string vertexfile, std::string fragfile) {
			GLuint vertexShader = _createShader(vertexfile, GL_VERTEX_SHADER);
			_compileShader(vertexShader);

			GLuint fragmentShader = _createShader(fragfile, GL_FRAGMENT_SHADER);
			_compileShader(fragmentShader);

			// Link shaders
			GLint success;
			GLchar infoLog[512];
			_shaderprogram = glCreateProgram();
			glAttachShader(_shaderprogram, vertexShader);
			glAttachShader(_shaderprogram, fragmentShader);
			glLinkProgram(_shaderprogram);
			// Check for linking errors
			glGetProgramiv(_shaderprogram, GL_LINK_STATUS, &success);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			if (!success) {
				glGetProgramInfoLog(_shaderprogram, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
				throw Core::BadShaderException("Could not create shader program!");
			}
		}

		ShaderProgram::~ShaderProgram() {

		}

		void ShaderProgram::UseProgram() const
		{
			glUseProgram(_shaderprogram);
		}

		GLint ShaderProgram::GetUniformLocation(std::string name) const {
			return glGetUniformLocation(_shaderprogram, name.c_str());
		}
		
		// Base Case shaderprogram will take in MVP matrices
		// I haven't learned enough to think of a situation where 
		// that isn't true
		void ShaderProgram::ApplyMVP(const glm::mat4& model
					 , const glm::mat4& view
					 , const glm::mat4& projection) const
		{
			UseProgram();

			GLint modelLoc = GetUniformLocation("model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			GLint viewLoc = GetUniformLocation("view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

			GLint projectionLoc = GetUniformLocation("projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		}

		GLuint ShaderProgram::_createShader(const std::string &filename, GLenum shadertype) {
			// Read file as std::string 
			std::string str = Core::ReadFile(filename);
			if (Core::GetError() != Core::ErrorCode::NO_ERROR) {
				std::cerr << "Failed to open file: " << filename << std::endl;
			}

			// c_str() gives us a const char*, but we need a non-const one
			char* src = const_cast<char*>(str.c_str());
			GLint size = (GLint)str.length();

			// Create an empty vertex shader handle
			GLuint shaderId = glCreateShader(shadertype);

			// Send the vertex shader source code to OpenGL
			glShaderSource(shaderId, 1, &src, &size);

			return shaderId;
		}

		bool ShaderProgram::_compileShader(GLuint shader) {
			// Build and compile our shader program
			// Vertex shader
			glCompileShader(shader);
			// Check for compile time errors
			GLint success;
			GLchar infoLog[512];
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
				return false;
			}

			return true;
		}
	}
}