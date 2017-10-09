#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include "shaders/base_shader.h"

namespace SAS_3D {
	namespace Shaders {
		BaseShader::BaseShader()
			: ShaderProgram("F:/github/SAS_3D/shaders/simple.vert", 
							"F:/github/SAS_3D/shaders/simple.frag")
		{
		}

		BaseShader::~BaseShader() {

		}

		void ApplyShader(  BaseShader& bs 
						 , const glm::mat4& model
						 , const glm::mat4& view
					     , const glm::mat4& projection) 
		{
			bs.UseProgram();

			GLint modelLoc = bs.GetUniformLocation("model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			GLint viewLoc = bs.GetUniformLocation("view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

			GLint projectionLoc = bs.GetUniformLocation("projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		}
	}
}