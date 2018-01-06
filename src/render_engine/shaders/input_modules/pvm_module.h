#pragma once
#include "input_module.h"
#include "render_engine/shaders/shader_program.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace SAS_3D {
	class PVMModule : public InputModule {
	public:
		static const unsigned long long ID = PVMModuleID;

		void ApplyToShader(ShaderProgram* shader) {
			GLint pvmLoc = shader->GetUniformLocation("pvm");
			glUniformMatrix4fv(pvmLoc, 1, GL_FALSE, glm::value_ptr(_pvm));
		}

		unsigned long long UniqueBits() const { return ID; }

		void SetPVM(const glm::mat4& pvm) { _pvm = pvm; }
	private:
		glm::mat4 _pvm;
	};
}
