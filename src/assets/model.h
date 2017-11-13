/*
* model.h
*
* Defines the engine specific format for models
*
* Author: Erik Kirshey
* Date: 10/4/17
*/
#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/scene.h>           // Output data structure
#include "assets/textures.h"
#include "shaders/texture_shader.h"

namespace SAS_3D {
	namespace Assets {
		struct Vertex {
			// Position
			glm::vec3 Position;
			// Normal
			glm::vec3 Normal;
			// TexCoords
			glm::vec2 TexCoords;
		};

		class Mesh {
			public:
				std::vector<Vertex> vertices;
				std::vector<GLuint> indices;
				std::vector<Texture> textures;
				GLuint VAO;
				GLuint VBO;
				GLuint EBO;
				void LoadIntoGPU();
				void UnloadFromGPU();

				Mesh(std::string rootpath, const aiMesh* ai_m, const aiScene* scene);
			private:
				void _loadMaterialTextures(std::string rootpath, aiMaterial *mat, aiTextureType type, std::string type_name);
		};

		class Model {
			public:
				Model(std::string path, const aiScene* scene);
				void LoadIntoGPU();
				void Draw(const Shaders::TextureShader& textureshader);
			private:
				std::vector<Mesh> meshes;
				bool _loaded;
		};
	}
}