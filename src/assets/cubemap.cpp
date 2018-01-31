#include "cubemap.h"

namespace SAS_3D {
	namespace {

	}

	CubeMap::CubeMap(std::string texturepath, TextureContainer& tc) 
		: _textureid(tc.LoadCubeMap(texturepath))
	{
		glGenVertexArrays(1, &_VAO);
		glGenBuffers(1, &_VBO);

		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubemapvertices), &cubemapvertices, GL_STATIC_DRAW);

		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);

		glBindVertexArray(0);
	}

	void CubeMap::Load(std::string texturepath, TextureContainer& tc) {
		cubemapvertices = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};
		_textureid = tc.LoadCubeMap(texturepath);
		glGenVertexArrays(1, &_VAO);
		glGenBuffers(1, &_VBO);

		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, cubemapvertices.size() * sizeof(float), &cubemapvertices[0], GL_STATIC_DRAW);

		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);

		//glBindVertexArray(0);
	}

	void CubeMap::Draw(ShaderProgram& shader, glm::mat4& p, glm::mat4& v) {
		// I think I'm going to refactor how I manage my shaders, so this is for now
		shader.SetInt("skybox", _textureid);
		shader.SetMat4("p", p);
		shader.SetMat4("v", v);

		glBindVertexArray(_VAO);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _textureid);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

	}
}