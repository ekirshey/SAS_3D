#pragma once
#include "input_module.h"
#include "render_engine/shaders/shader_program.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace SAS_3D {
	class MVPModule : public InputModule {
	public:
		static const unsigned long long ID = MVPModuleID;

		void ApplyToShader(ShaderProgram* shader) {
			GLint mvpLoc = shader->GetUniformLocation("mvp");
			glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(_mvp));
		}

		unsigned long long UniqueBits() const { return ID; }

		void SetMVP(const glm::mat4& mvp) { _mvp = mvp; }
	private:
		glm::mat4 _mvp;
	};
}
