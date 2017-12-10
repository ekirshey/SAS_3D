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
		~ShaderProgram();

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
		T GetInputModule(unsigned long long id) {
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

	private:
		GLuint _createShader(const std::string &filename, GLenum shadertype);
		bool _compileShader(GLuint shader);

		GLuint _shaderprogram;
		std::unordered_map<unsigned long long, std::unique_ptr<InputModule>> _inputmodules;
	};
}
