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
#include "render_engine/shaders/shader_program.h"

namespace SAS_3D {
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
		std::vector<GLuint> textures;
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		void LoadIntoGPU();
		void UnloadFromGPU();

		Mesh(std::string modelpath, TextureContainer& c, const aiMesh* ai_m, const aiScene* scene);
	private:
		void _loadMaterialTextures(std::string modelpath, TextureContainer& c, aiMaterial *mat, aiTextureType type, std::string type_name);
	};

	class Model {
	public:
		Model(std::string path, TextureContainer& c, const aiScene* scene);
		void LoadIntoGPU();
		void Draw(ShaderProgram& shader);
		std::string Path() { return _path; }
	private:
		std::string _path;
		std::vector<Mesh> _meshes;
		bool _loaded;
	};
}