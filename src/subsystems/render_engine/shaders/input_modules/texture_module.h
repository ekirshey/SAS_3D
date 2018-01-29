#pragma once
#include "input_module.h"
#include "subsystems/render_engine/shaders/shader_program.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace SAS_3D {
	class MaterialModule : public InputModule {
	public:
		static const InputID ID = MaterialModuleID;

		void ApplyToShader(ShaderProgram* shader) {
			GLint diffuseLoc = shader->GetUniformLocation("material.diffuse");
			glUniform1f(diffuseLoc, _diffuse);

			GLint specularLoc = shader->GetUniformLocation("material.specular");
			glUniform1f(specularLoc, _specular);

			GLint shininessLoc = shader->GetUniformLocation("material.shininess");
			glUniform1f(shininessLoc, _shininess);
		}

		InputID UniqueBits() const { return ID; }

		void SetMaterial(const unsigned int diffuse, const int specular = -1, float shininess = 32.0f) { 
			_diffuse = diffuse;
			if (specular == -1) {
				_specular = diffuse;
			}
			else {
				_specular = specular;
			}

			_shininess = shininess;
		}
	private:
		unsigned int _diffuse;
		unsigned int _specular;
		float _shininess;
	};
}

