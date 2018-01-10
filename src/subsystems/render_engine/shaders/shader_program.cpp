#include <iostream>
#include "core/sas_io.h"
#include "core/error_codes.h"
#include "shader_program.h"

namespace SAS_3D {
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
//			throw BadShaderException("Could not create shader program!");
		}
	}

	ShaderProgram::~ShaderProgram() {

	}

	void ShaderProgram::Load(std::string vertexfile, std::string fragfile) {
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
			//			throw BadShaderException("Could not create shader program!");
		}
	}

	void ShaderProgram::UseProgram() const
	{
		glUseProgram(_shaderprogram);
	}

	GLint ShaderProgram::GetUniformLocation(std::string name) const {
		return glGetUniformLocation(_shaderprogram, name.c_str());
	}


	GLuint ShaderProgram::_createShader(const std::string &filename, GLenum shadertype) {
		// Read file as std::string 
		std::string str = ReadFile(filename);
		if (GetError() != ErrorCode::NO_ERROR) {
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


	void ShaderProgram::ApplyModules() {
		for (auto& module : _inputmodules) {
			module.second->ApplyToShader(this);
		}
	}
}