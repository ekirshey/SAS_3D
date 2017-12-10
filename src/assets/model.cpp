#include "model.h"
#include "render_engine/shaders/input_modules/texture_module.h"
#include <iostream>

namespace SAS_3D {
	Mesh::Mesh(std::string modelpath, TextureContainer& c, const aiMesh* ai_m, const aiScene* scene)
		: vertices{ai_m->mNumVertices}
	{
		// Walk through each of the ai_m's vertices
		for (unsigned int i = 0; i < ai_m->mNumVertices; i++) {
			Vertex vertex;
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// Positions
			vector.x = ai_m->mVertices[i].x;
			vector.y = ai_m->mVertices[i].y;
			vector.z = ai_m->mVertices[i].z;
			vertex.Position = vector;
			// Normals
			vector.x = ai_m->mNormals[i].x;
			vector.y = ai_m->mNormals[i].y;
			vector.z = ai_m->mNormals[i].z;
			vertex.Normal = vector;
			// Texture Coordinates
			if (ai_m->mTextureCoords[0]) { // Does the mesh contain texture coordinates?
				glm::vec2 vec;
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = ai_m->mTextureCoords[0][i].x;
				vec.y = ai_m->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else {
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}
			vertices[i] = vertex;
		}

		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < ai_m->mNumFaces; i++) {
			aiFace face = ai_m->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		// process materials
		aiMaterial* material = scene->mMaterials[ai_m->mMaterialIndex];
		_loadMaterialTextures(modelpath, c, material, aiTextureType_DIFFUSE, "texture_diffuse");

	}

	void Mesh::_loadMaterialTextures(std::string modelpath, TextureContainer& c, aiMaterial *mat, aiTextureType type, std::string type_name) {
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);
			std::string texpath(str.C_Str());
			std::string path = modelpath + texpath;
			textures.push_back(c.LoadTextureFromFile(path, type_name));
		}
	}

	void Mesh::LoadIntoGPU() {
		// Create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointers
		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
		// Vertex Texture Coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}

	void Mesh::UnloadFromGPU() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	Model::Model(std::string path, TextureContainer& c, const aiScene* scene)
		: _path(path)
	{
		if (scene->HasMeshes()) {
			auto lastslash = path.rfind('/') + 1;
			for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
				_meshes.push_back(Mesh(path.substr(0, lastslash),c,scene->mMeshes[i], scene));
			}
		}
	}

	void Model::LoadIntoGPU() {
		_loaded = true;
		for (int i = 0; i < _meshes.size(); i++) {
			_meshes[i].LoadIntoGPU();
		}
	}

	void Model::Draw(ShaderProgram& shader) {
		if (!_loaded) {
			// Proper error handling? Exception?
			std::cout << "Trying to draw unloaded model"  << std::endl;
			return;
		}
		auto texturemodule = shader.GetInputModule<TextureModule*>(TextureModule::ID);
		for (auto& m : _meshes) {
			unsigned int tct = 0;
			for (auto& t : m.textures) {
				glActiveTexture(GL_TEXTURE0 + tct);
				texturemodule->SetTexture(tct);
				glBindTexture(GL_TEXTURE_2D, t);
				tct++;
			}
			
			shader.ApplyModules();

			glBindVertexArray(m.VAO);
			glDrawElements(GL_TRIANGLES, m.indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glActiveTexture(GL_TEXTURE0);
		}
	}
}