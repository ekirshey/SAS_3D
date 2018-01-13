#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "input_module.h"
#include "subsystems/render_engine/shaders/shader_program.h"


namespace SAS_3D {
	class PVMModule : public InputModule {
	public:
		static const InputID ID = PVMModuleID;

		void ApplyToShader(ShaderProgram* shader) {
			GLint pvmLoc = shader->GetUniformLocation("pvm");
			glUniformMatrix4fv(pvmLoc, 1, GL_FALSE, glm::value_ptr(_pvm));
		}

		InputID UniqueBits() const { return ID; }

		void SetPVM(const glm::mat4& pvm) { _pvm = pvm; }
	private:
		glm::mat4 _pvm;
	};
}
