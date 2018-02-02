#pragma once
#include <string>
#include <glm/glm.hpp>
#include "subsystems/render_engine/shaders/shader_program.h"
#include "textures.h"

namespace SAS_3D {

	class CubeMap {
	public:
		CubeMap() {}
		CubeMap(std::string texturepath, TextureContainer& tc);

		void Load(std::string texturepath, TextureContainer& tc);
		void Draw(ShaderProgram& shader, glm::mat4& p, glm::mat4& v);
	private:
		GLuint _VAO;
		GLuint _VBO;
		unsigned int _textureid;

	};

}