#include "model.h"
#include "render_engine/shaders/input_modules/texture_module.h"
#include "render_engine/shaders/input_modules/animation_module.h"
#include "render_engine/shaders/input_modules/mvp_module.h"
#include <iostream>
#include <queue>
#include <unordered_set>

namespace SAS_3D {
	// helper https://stackoverflow.com/questions/29184311/how-to-rotate-a-skinned-models-bones-in-c-using-assimp
	inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from) {
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}

	Mesh::Mesh(std::string modelpath, TextureContainer& c, const aiMesh* ai_m, const aiScene* scene)
		: vertices{ ai_m->mNumVertices }
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
		std::vector<int> bonetracker(vertices.size(),0);
		std::vector<int> weighttracker(vertices.size(),0);
		for (unsigned int i = 0; i < ai_m->mNumBones; i++) {
			Joint b;
			aiBone* ai_b = ai_m->mBones[i];
			b.name = ai_b->mName.C_Str();
			b.offsetmatrix = aiMatrix4x4ToGlm(ai_b->mOffsetMatrix);
			for (unsigned int j = 0; j < ai_b->mNumWeights; j++) {
				auto vertid = ai_b->mWeights[j].mVertexId;
				auto weight = ai_b->mWeights[j].mWeight;
				vertices[vertid].bones[bonetracker[vertid]++] = i;
				vertices[vertid].weights[weighttracker[vertid]++] = weight;
			}
			skeleton.push_back(b);
			bonemap.insert({ b.name, static_cast<int>(skeleton.size() - 1) });	// map bone index to name
		}

		auto node = scene->mRootNode;
		auto rootid = FindRoot(node);
		if (rootid != -1) {
			auto tmp = node->FindNode(skeleton[rootid].name.c_str());
			aiNode* root;
			if (tmp != NULL) {
				root = tmp->mParent;
				rootnode.name = root->mName.C_Str();
				rootnode.transformation = aiMatrix4x4ToGlm(root->mTransformation);
			}
		}

	}

	int Mesh::FindRoot(aiNode* node) {
		std::queue<aiNode*> to_visit;
		std::unordered_set<aiNode*> visited;
		to_visit.push(node);
		visited.insert(node);
		int found;
		while (!to_visit.empty()) {
			auto node = to_visit.front();
			to_visit.pop();
			if (bonemap.find(node->mName.C_Str()) != bonemap.end()) {
				return bonemap[node->mName.C_Str()];
			}
			for (int i = 0; i < node->mNumChildren; i++) {
				auto child = node->mChildren[i];
				if (visited.find(child) == visited.end()) {
					visited.insert(child);
					to_visit.push(child);
				}
			}
		}

		return -1;
	}

	void Mesh::BuildGlobalTransforms(aiNode* node) {
		auto nodename = node->mName.C_Str();
		glm::mat4 localtransform;
		if (bonemap.find(nodename) != bonemap.end()) {
			int idx = bonemap[nodename];
			glm::mat4 parenttransform;
			if (node->mParent != NULL) {
				int pidx = bonemap[node->mParent->mName.C_Str()];
				parenttransform = pose.global_pose[pidx];
			}
		}
		//pose.global_pose[idx] = parenttransform*localtransform;// *aiMatrix4x4ToGlm(node->mTransformation);// ;
		/*
		pose.global_pose[idx] = parenttransform * glm::translate(glm::mat4(), pose.local_pose[idx].translation)
		* glm::toMat4(pose.local_pose[idx].rotation)
			* glm::scale(glm::mat4(), pose.local_pose[idx].scale)
			* skeleton[idx].offsetmatrix;
			*/
		for (int i = 0; i < node->mNumChildren; i++) {
			BuildGlobalTransforms(node->mChildren[i]);
		}
	}


	void Mesh::BuildBoneMatrices(std::unordered_map<std::string, Animation>& animations, const aiScene* scene) {
#ifdef FOO
		if (animations.size() > 0) {
			for (auto& f : animations.begin()->second.frames) {
				bonematrices.insert({ f.first, std::vector<glm::mat4>{100} });
				for (int i = 0; i < bones.size(); i++) {
					auto bonename = bones[i].name;
					// needs to recurse up
					// p0 * p1 * p2 * p2_offset
					glm::mat4 transform = f.second.at(bonename);// *bones[i].offsetmatrix;
					auto current_node = root->FindNode(bonename.c_str())->mParent;
					while (current_node != NULL) {
						std::string nodename = current_node->mName.C_Str();
						transform = f.second.at(nodename) * transform;
						current_node = current_node->mParent;
					}
					bonematrices[f.first].at(i) = transform;
				}
			}
		}
		std::cout << "done building matrices" << std::endl;
#endif
		if (scene->HasAnimations()) {
			pose.local_pose.resize(skeleton.size(), JointPose());
			pose.global_pose.resize(skeleton.size());
			auto animation = scene->mAnimations[0];	
			// Channels are nodes that are animated
			for (int i = 0; i < animation->mNumChannels; i++) {
				auto channel = animation->mChannels[i];
				auto nodename = channel->mNodeName.C_Str();
				if (bonemap.find(nodename) != bonemap.end()) {
					int idx = bonemap[nodename];
					auto scale = channel->mScalingKeys[0].mValue;
					auto quat = channel->mRotationKeys[0].mValue;
					auto trans = channel->mPositionKeys[0].mValue;
					pose.local_pose[idx].scale = glm::vec3(scale.x, scale.y, scale.z);
					pose.local_pose[idx].rotation = glm::quat(quat.w, quat.x, quat.y, quat.z);
					pose.local_pose[idx].translation = glm::vec3(trans.x, trans.y, trans.z);
				}
			}
			auto root = scene->mRootNode;//->FindNode(rootnode.name.c_str());
			BuildGlobalTransforms(root);
			for (int i = 0; i < pose.global_pose.size(); i++) {
				pose.global_pose[i] = glm::inverse(rootnode.transformation) * pose.global_pose[i] * skeleton[i].offsetmatrix;
			}
		}
		/*
		auto node = root_node->FindNode(nodename)->mParent;
		glm::mat4 newtransform = glm::translate(glm::mat4(), pose.local_pose[idx].translation) * glm::toMat4(pose.local_pose[idx].rotation) * glm::scale(glm::mat4(),pose.local_pose[idx].scale);
		glm::mat4 transform = newtransform;
		while (node != NULL) {
		transform = aiMatrix4x4ToGlm(node->mTransformation) * transform;
		node = node->mParent;
		}
		pose.global_pose[idx] = transform;
		*/
		std::cout << "end matrix build" << std::endl;
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

	void Model::BuildMeshes(const aiScene* scene, TextureContainer& c, aiNode* node, std::string path) {
		for (int j = 0; j < node->mNumMeshes; j++) {
			_meshes.push_back(Mesh(path, c, scene->mMeshes[j], scene));
			_meshes.back().globalmeshtransform = aiMatrix4x4ToGlm(node->mTransformation);
			_meshes.back().BuildBoneMatrices(_animations, scene);
		}
		for (int i = 0; i < node->mNumChildren; i++) {
			BuildMeshes(scene, c, node->mChildren[i], path);
		}
	}

	Model::Model(std::string path, TextureContainer& c, const aiScene* scene)
		: _path(path)
		, _loaded(false)
	{
		// Build meshes
		if (scene->HasMeshes()) {
			auto lastslash = path.rfind('/') + 1;
			BuildMeshes(scene, c, scene->mRootNode, path.substr(0, lastslash));
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
	
	void Model::DrawSkeleton(glm::mat4& m, glm::mat4& v, glm::mat4& p, Model& primitive, ShaderProgram& shader) {
		if (!_loaded) {
			// Load the model into memory
			LoadIntoGPU();
		}

		auto mvpmodule = shader.GetInputModule<MVPModule*>(MVPModule::ID);

		for (auto& me : _meshes) {
			for (int i = 0; i < me.skeleton.size(); i++) {
				//auto mat = mvp * m.bones[i].offsetmatrix;
				// offset goes from mesh->bone, so to show it properly i need  to  invert it, so its not bone to mesh
				//auto x = _animations.begin()->second.frames.begin()->second.at(me.bones[i].name);
				//auto x = me.bonematrices.begin()->second.at(i);
				//auto mat = p * v * m * me.globalmeshtransform * glm::inverse(me.skeleton[i].offsetmatrix);// *root[i];// glm::inverse(me.skeleton[i].offsetmatrix);
				auto mat = p * v * m * me.globalmeshtransform * me.pose.global_pose[0] * me.skeleton[i].offsetmatrix;// *root[i];// glm::inverse(me.skeleton[i].offsetmatrix);
				mvpmodule->SetMVP(mat);

				shader.ApplyModules();

				primitive.Draw();

				glActiveTexture(GL_TEXTURE0);
			}
		}
	}

	void Model::Draw() {
		if (!_loaded) {
			// Load the model into memory
			LoadIntoGPU();
		}

		for (auto& m : _meshes) {
			glBindVertexArray(m.VAO);
			glDrawElements(GL_TRIANGLES, m.indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glActiveTexture(GL_TEXTURE0);
		}
	}

	void Model::Draw(ShaderProgram& shader, glm::mat4& m, glm::mat4& v, glm::mat4& p) {
		if (!_loaded) {
			// Load the model into memory
			LoadIntoGPU();
		}
		auto texturemodule = shader.GetInputModule<TextureModule*>(TextureModule::ID);
		auto animationmodule = shader.GetInputModule<AnimationModule*>(AnimationModule::ID);
		auto mvpmodule = shader.GetInputModule<MVPModule*>(MVPModule::ID);
		for (auto& me : _meshes) {
			unsigned int tct = 0;
			for (auto& t : me.textures) {
				glActiveTexture(GL_TEXTURE0 + tct);
				texturemodule->SetTexture(tct);
				glBindTexture(GL_TEXTURE_2D, t);
				tct++;
			}
			if (me.skeleton.size() > 0) {
				animationmodule->SetBones(&me.pose.global_pose);
			}
			mvpmodule->SetMVP(p*v*m);
			shader.ApplyModules();

			glBindVertexArray(me.VAO);
			glDrawElements(GL_TRIANGLES, me.indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glActiveTexture(GL_TEXTURE0);
		}
	}
}