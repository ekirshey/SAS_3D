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
#include <memory>
#include <unordered_map>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "input_modules/input_module.h"

namespace SAS_3D {
	class InputModule;
	class ShaderProgram {
	public:
		ShaderProgram(std::string vertexfile, std::string fragfile);
		ShaderProgram() {}
		~ShaderProgram();

		void Load(std::string vertexfile, std::string fragfile);
		void UseProgram() const;
		GLint GetUniformLocation(std::string name) const;

		template<typename T, typename... Args>
		void AddInputModule(Args&&... args) {
			static_assert(std::is_base_of<InputModule, std::remove_pointer<T>::type>::value, "T must be derived from InputModule");
			if (_inputmodules.find(T::ID) == _inputmodules.end()) {
				_inputmodules.insert({ T::ID, std::make_unique<T>(std::forward(args)...) });
			}
			else {
				std::cout << "Module already exists in program" << std::endl;
			}
		}

		template<typename T>
		T GetInputModule(InputID id) {
			static_assert(std::is_pointer<T>::value, "T must be a pointer to a type derived from InputModule");
			static_assert(std::is_base_of<InputModule, std::remove_pointer<T>::type>::value, "T must be derived from InputModule");

			if (_inputmodules.find(id) != _inputmodules.end()) {
				return static_cast<T>(_inputmodules[id].get());
			}
			else {
				return nullptr;
			}
		}

		void ApplyModules();

		// utility uniform functions
		void SetBool(const std::string &name, bool value) const {
			glUniform1i(glGetUniformLocation(_shaderprogram, name.c_str()), (int)value);
		}

		void SetInt(const std::string &name, int value) const {
			glUniform1i(glGetUniformLocation(_shaderprogram, name.c_str()), value);
		}

		void SetFloat(const std::string &name, float value) const {
			glUniform1f(glGetUniformLocation(_shaderprogram, name.c_str()), value);
		}

		void SetVec2(const std::string &name, const glm::vec2 &value) const {
			glUniform2fv(glGetUniformLocation(_shaderprogram, name.c_str()), 1, &value[0]);
		}

		void SetVec2(const std::string &name, float x, float y) const {
			glUniform2f(glGetUniformLocation(_shaderprogram, name.c_str()), x, y);
		}

		void SetVec3(const std::string &name, const glm::vec3 &value) const {
			glUniform3fv(glGetUniformLocation(_shaderprogram, name.c_str()), 1, &value[0]);
		}
		void SetVec3(const std::string &name, float x, float y, float z) const {
			glUniform3f(glGetUniformLocation(_shaderprogram, name.c_str()), x, y, z);
		}

		void SetVec4(const std::string &name, const glm::vec4 &value) const {
			glUniform4fv(glGetUniformLocation(_shaderprogram, name.c_str()), 1, &value[0]);
		}

		void SetVec4(const std::string &name, float x, float y, float z, float w) {
			glUniform4f(glGetUniformLocation(_shaderprogram, name.c_str()), x, y, z, w);
		}

		void SetMat2(const std::string &name, const glm::mat2 &mat) const {
			glUniformMatrix2fv(glGetUniformLocation(_shaderprogram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}

		void SetMat3(const std::string &name, const glm::mat3 &mat) const {
			glUniformMatrix3fv(glGetUniformLocation(_shaderprogram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}

		void SetMat4(const std::string &name, const glm::mat4 &mat) const {
			glUniformMatrix4fv(glGetUniformLocation(_shaderprogram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}

	private:
		GLuint _createShader(const std::string &filename, GLenum shadertype);
		bool _compileShader(GLuint shader);

		GLuint _shaderprogram;
		std::unordered_map<InputID, std::unique_ptr<InputModule>> _inputmodules;
	};
}
