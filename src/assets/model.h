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

#include "core/error_codes.h"

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

		struct Mesh {
			std::vector<Vertex> vertices;
			std::vector<GLuint> indices;
			GLuint VAO;
			GLuint VBO;
			GLuint EBO;
			void LoadIntoGPU();
			void UnloadFromGPU();
			Mesh(const aiMesh* ai_m, const aiScene* scene);
		};

		struct Model {
			bool loaded;
			std::vector<Mesh> meshes;
			Model(const aiScene* scene);
		};
	}
}