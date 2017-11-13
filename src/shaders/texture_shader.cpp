
#include "shaders/texture_shader.h"

namespace SAS_3D {
	namespace Shaders {
		TextureShader::TextureShader(std::string shader_root_path)
			: ShaderProgram(shader_root_path + "texture.vert",
				shader_root_path + "texture.frag")
		{
		}

		TextureShader::~TextureShader() {

		}

		void TextureShader::ApplyTexture(unsigned int texture_unit) const {
			GLint sampleloc = GetUniformLocation("texture_diffuse1");
			glUniform1f(sampleloc, texture_unit);
		}

	}
}