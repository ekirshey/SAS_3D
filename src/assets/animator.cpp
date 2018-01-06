#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "animator.h"

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

	Animator::Animator(const aiScene* scene) {
		// Build up custom Node structure
		_rootnode = _createNodeTree(scene->mRootNode, nullptr);

		// build the nodes-for-bones table
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
		{
			const aiMesh* mesh = scene->mMeshes[i];
			_bonemaps.push_back(BoneMap());
			for (unsigned int n = 0; n < mesh->mNumBones; ++n)
			{
				const aiBone* bone = mesh->mBones[n];
				auto node = _rootnode->FindNode(bone->mName.data);
				if (node == nullptr) {
					std::cout << "Error! Could not find node with name " << bone->mName.data << std::endl;
				}
				_bonemaps[i].insert({n, node});
				_meshskeletons[i].push_back(aiMatrix4x4ToGlm(bone->mOffsetMatrix));
			}
		}
	}

	void Animator::GetBoneMatrices(std::vector<glm::mat4>& matrices, int meshidx) {
		glm::mat4 inverse_mesh_transform = glm::inverse(_meshnode->_global_transform);

		matrices.resize(_bonemaps[meshidx].size(), glm::mat4());
		for (int i = 0; i < _bonemaps[meshidx].size(); i++) {
			const glm::mat4 current_global_transform = _bonemaps[meshidx].at(i)->_global_transform;
			matrices[i] = inverse_mesh_transform * current_global_transform *_meshskeletons[meshidx].at(i);
		}
	}

	Animator::pAnimNode Animator::_createNodeTree(aiNode* node, AnimNode* parent) {
		pAnimNode internalnode = std::make_unique<AnimNode>(node->mName.data, parent);

		internalnode->_local_transform = aiMatrix4x4ToGlm(node->mTransformation);
		if (parent != nullptr) {
			internalnode->_global_transform = parent->_global_transform * internalnode->_local_transform;
		}
		else {
			internalnode->_global_transform = internalnode->_local_transform;
		}

		for (int i = 0; i < node->mNumChildren; i++) {
			auto childnode = _createNodeTree(node->mChildren[i], internalnode.get());
			internalnode->_children.push_back(std::move(childnode));
		}

		/* This is the node that holds the meshes. Need to store it for
		   generating the bone matrices. I'm making an assumption that only
		   ONE node will contain the meshes. This seems correct from all the models
		   I've looked at.
		*/
		if (node->mNumMeshes > 0) {
			_meshnode = internalnode.get();
		}

		return std::move(internalnode);
	}

}