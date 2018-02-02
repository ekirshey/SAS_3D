#pragma once
#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "input_module.h"
#include "subsystems/render_engine/shaders/shader_program.h"


namespace SAS_3D {
	class AnimationModule : public InputModule {
	public:
		static const InputID ID = AnimationModuleID;

		AnimationModule() 
			: _bones(nullptr)
		{
		}

		void ApplyToShader(ShaderProgram* shader) {
			GLint bonesLoc = shader->GetUniformLocation("Bones");
			if (_bones != nullptr) {
				glUniformMatrix4fv(bonesLoc, _bones->size(), GL_FALSE, glm::value_ptr(_bones->at(0)));
			}
		}

		InputID UniqueBits() const { return ID; }

		void SetBones(const std::vector<glm::mat4>* bones) {
			_bones = bones;
		}

	private:
		const std::vector<glm::mat4>* _bones;
	};
}
