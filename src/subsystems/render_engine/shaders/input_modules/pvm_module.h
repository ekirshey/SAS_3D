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
			GLint pvLoc = shader->GetUniformLocation("pv");
			glUniformMatrix4fv(pvLoc, 1, GL_FALSE, glm::value_ptr(_pv));

			GLint mLoc = shader->GetUniformLocation("m");
			glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(_m));
		}

		InputID UniqueBits() const { return ID; }

		void SetPVM(const glm::mat4& pv, const glm::mat4& m) { 
			_pv = pv;
			_m = m;
		}

	private:
		glm::mat4 _pv;
		glm::mat4 _m;
	};
}
