#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/gtx/quaternion.hpp>
#include <assimp/scene.h>           // Output data structure
#include "assets/textures.h"

namespace SAS_3D {
	struct Vertex {
		// Position
		glm::vec3 Position;
		// Normal
		glm::vec3 Normal;
		// TexCoords
		glm::vec2 TexCoords;
		// Bones
		glm::uvec4 bones;
		// Weights
		glm::vec4 weights;
	};

	class Mesh {
	public:
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<GLuint> textures;
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		void LoadIntoGPU();
		void UnloadFromGPU();

		Mesh(std::string modelpath, TextureContainer& tc, const aiMesh* ai_m, const aiScene* scene);
	private:
		void _loadMaterialTextures(std::string modelpath, TextureContainer& c, aiMaterial *mat, aiTextureType type, std::string type_name);
	};
}
