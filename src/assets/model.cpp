#include "model.h"
#include "render_engine/shaders/input_modules/texture_module.h"
#include "render_engine/shaders/input_modules/animation_module.h"
#include <iostream>
#include <queue>
#include <unordered_set>

namespace SAS_3D {
	// helper https://stackoverflow.com/questions/29184311/how-to-rotate-a-skinned-models-bones-in-c-using-assimp
	inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from) {
		glm::mat4 to;

		to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
		to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
		to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
		to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;

		return to;
	}

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

		// Now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
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

		// process bones
		for (unsigned int i = 0; i < ai_m->mNumBones; i++) {
			Bone b;
			aiBone* ai_b = ai_m->mBones[i];
			b.name = ai_b->mName.C_Str();
			b.offsetmatrix = aiMatrix4x4ToGlm(ai_b->mOffsetMatrix);
			for (unsigned int j = 0; j < ai_b->mNumWeights; j++) {
				auto vertid = ai_b->mWeights[j].mVertexId;
				auto weight = ai_b->mWeights[j].mWeight;
				b.weights[vertid].push_back(weight);
				vertices[vertid].bones.push_back(i);
				vertices[vertid].weights.push_back(weight);
			}
			bones.push_back(b);
		}
	}

	void Mesh::BuildBoneMatrices(std::unordered_map<std::string, Animation>& animations) {
		if (animations.size() > 0) {
			for (auto& f : animations.begin()->second.frames) {
				bonematrices.insert({ f.first, std::vector<glm::mat4>{100} });
				for (int i = 0; i < bones.size(); i++) {
					auto bonename = bones[i].name;
					// needs to recurse up
					bonematrices[f.first].at(i) = f.second.at(bonename) * bones[i].offsetmatrix;
				}
			}
		}
		std::cout << "done building matrices" << std::endl;
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
		// Vertex Texture Coords
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_INT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bones));
		// Vertex Texture Coords
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, weights));

		glBindVertexArray(0);
	}

	void Mesh::UnloadFromGPU() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	Model::Model(std::string path, TextureContainer& c, const aiScene* scene)
		: _path(path)
		, _loaded(false)
	{
		// Build Node list
		std::queue<aiNode*> to_visit;
		std::unordered_set<aiNode*> visited;
		to_visit.push(scene->mRootNode);

		while (!to_visit.empty()) {
			auto n = to_visit.front();
			to_visit.pop();
			visited.insert(n);
			// Build SAS Node
			auto node_name = n->mName.C_Str();
			_nodes.insert({ node_name, Node() });
			auto active_node = &_nodes[node_name];
			if (n->mParent != nullptr) {
				active_node->parent = n->mParent->mName.C_Str();
			}
			for (unsigned int i = 0; i <n->mNumChildren; i++) {
				if (visited.find(n->mChildren[i]) == visited.end()) {
					auto child = n->mChildren[i];
					active_node->children.push_back(child->mName.C_Str());
					to_visit.push(child);
				}
			}
		}

		// Build animation frames
		if (scene->HasAnimations()) {
			for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
				auto ai_a = scene->mAnimations[i];
				std::string ai_name = ai_a->mName.C_Str();
				_animations[ai_name] = Animation();
				auto ani = &_animations[ai_name];
				ani->tickspersecond = ai_a->mTicksPerSecond;
				ani->duration = ai_a->mDuration;
				for (unsigned int j = 0; j < ai_a->mNumChannels; j++) {
					auto nodeanim = ai_a->mChannels[j];
					auto nodename = nodeanim->mNodeName.C_Str();
					// Translate 
					for (int q = 0; q < nodeanim->mNumPositionKeys; q++) {
						auto time = nodeanim->mPositionKeys[q].mTime;
						auto aipos = nodeanim->mPositionKeys[q].mValue;
						glm::vec3 pos(aipos.x, aipos.y, aipos.z);
						if (ani->CreateNewFrame(time, _nodes)) {
							ani->TranslateNode(time, nodename, pos);
						}
					}
					//Rotate
					for (int r = 0; r < nodeanim->mNumRotationKeys; r++) {
						auto time = nodeanim->mRotationKeys[r].mTime;
						auto airot = nodeanim->mRotationKeys[r].mValue;
						glm::quat rot(airot.w, airot.x, airot.y, airot.z);
						if (ani->CreateNewFrame(time, _nodes)) {
							ani->RotateNode(time, nodename, rot);
						}
					}
					//Scale
					for (int k = 0; k < nodeanim->mNumScalingKeys; k++) {
						auto time = nodeanim->mScalingKeys[k].mTime;
						auto aiscale = nodeanim->mScalingKeys[k].mValue;
						glm::vec3 scale(aiscale.x, aiscale.y, aiscale.z);
						if (ani->CreateNewFrame(time, _nodes)) {
							ani->ScaleNode(time, nodename, scale);
						}
					}
				}
			}
		}

		// Build meshes
		if (scene->HasMeshes()) {
			auto lastslash = path.rfind('/') + 1;
			for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
				_meshes.push_back(Mesh(path.substr(0, lastslash),c,scene->mMeshes[i], scene));
				_meshes.back().BuildBoneMatrices(_animations);
			}
		}

		std::cout << "Done model processing..." << std::endl;
	}

	void Model::LoadIntoGPU() {
		if (!_loaded) {
			_loaded = true;
			for (int i = 0; i < _meshes.size(); i++) {
				_meshes[i].LoadIntoGPU();
			}
		}
	}

	void Model::Draw(ShaderProgram& shader) {
		if (!_loaded) {
			// Load the model into memory
			LoadIntoGPU();
		}
		auto texturemodule = shader.GetInputModule<TextureModule*>(TextureModule::ID);
		auto animationmodule = shader.GetInputModule<AnimationModule*>(AnimationModule::ID);
		for (auto& m : _meshes) {
			unsigned int tct = 0;
			for (auto& t : m.textures) {
				glActiveTexture(GL_TEXTURE0 + tct);
				texturemodule->SetTexture(tct);
				glBindTexture(GL_TEXTURE_2D, t);
				tct++;
			}
			if (m.bonematrices.size() > 0) {
				auto it = m.bonematrices.begin()++;
				animationmodule->SetBones(&it->second);
			}
			shader.ApplyModules();

			glBindVertexArray(m.VAO);
			glDrawElements(GL_TRIANGLES, m.indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glActiveTexture(GL_TEXTURE0);
		}
	}
}