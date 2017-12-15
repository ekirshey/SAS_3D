#pragma once
#include "input_module.h"
#include "render_engine/shaders/shader_program.h"
#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace SAS_3D {
	class AnimationModule : public InputModule {
	public:
		static const unsigned long long ID = AnimationModuleID;

		void ApplyToShader(ShaderProgram* shader) {
			GLint bonesLoc = shader->GetUniformLocation("Bones");
			glUniformMatrix4fv(bonesLoc, _bones->size(), GL_FALSE, glm::value_ptr(_bones->at(0)));
		}

		unsigned long long UniqueBits() const { return ID; }

		void SetBones(std::vector<glm::mat4>* bones) {
			_bones = bones;
		}

	private:
		std::vector<glm::mat4>* _bones;
	};
}
