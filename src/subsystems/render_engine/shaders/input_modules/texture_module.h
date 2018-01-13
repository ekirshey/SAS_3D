#pragma once
#include "input_module.h"
#include "subsystems/render_engine/shaders/shader_program.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace SAS_3D {
	class TextureModule : public InputModule {
	public:
		static const InputID ID = TextureModuleID;

		void ApplyToShader(ShaderProgram* shader) {
			GLint sampleloc = shader->GetUniformLocation("texture_diffuse1");
			glUniform1f(sampleloc, _textureunit);
		}

		InputID UniqueBits() const { return ID; }

		void SetTexture(const unsigned int textureunit) { _textureunit = textureunit; }
	private:
		unsigned int _textureunit;
	};
}

