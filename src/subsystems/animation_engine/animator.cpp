#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "animator.h"
#include "assets/assimp_loader.h"

namespace SAS_3D {
	Animator::Animator(const SceneInfo* sceneinfo) 
	{
		auto scene = sceneinfo->scene;
		if (!scene->HasAnimations()) {
			std::cout << "No animations!" << std::endl;
		}
		_animidx = 0;
		_animct = scene->mNumAnimations;

		// Build up custom Node structure
		_rootnode = _createNodeTree(scene->mRootNode, nullptr);

		// build the nodes-for-bones table
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
		{
			const aiMesh* mesh = scene->mMeshes[i];
			_bonemaps.push_back(BoneVec{mesh->mNumBones});
			_meshskeletons.push_back(Skeleton{mesh->mNumBones});
			for (unsigned int n = 0; n < mesh->mNumBones; ++n)
			{
				const aiBone* bone = mesh->mBones[n];
				auto node = _rootnode->FindNode(bone->mName.data);
				if (node == nullptr) {
					std::cout << "Error! Could not find node with name " << bone->mName.data << std::endl;
				}
				_bonemaps[i][n] =node;
				_meshskeletons[i][n] = aiMatrix4x4ToGlm(bone->mOffsetMatrix);
			}
		}

		// Build up the transforms
		for (int i = 0; i < scene->mNumAnimations; i++) {
			_animations.push_back(AnimEvaluator(i, _rootnode.get(), scene->mAnimations[i]));
		}
	}

	void Animator::SetAnimationIndex(int i) {
		_animidx = i;
	}

	void Animator::_getGlobalTransform(AnimNode* node) {
		auto parent = node->_parent;
		if (parent != nullptr) {
			node->_global_transform = parent->_global_transform * node->_local_transform;
		}
		else {
			node->_global_transform = node->_local_transform;
		}
	}

	void Animator::_updateNodes(AnimNode* node) {
		auto channelid = node->_channelids[_animidx];
		if (channelid != -1) {
			node->_local_transform = _animations[_animidx].GetTransform(channelid);
		}

		_getGlobalTransform(node);

		for (int i = 0; i < node->_children.size(); i++) {
			_updateNodes(node->_children[i].get());
		}
	}

	void Animator::CalculateBoneMatrices(double time, AnimationState& animation) {
		_animidx != animation.index;
		_animations[_animidx].Calculate(time, animation.frameindices);
		_updateNodes(_rootnode.get());

		animation.bones.resize(_bonemaps.size());
		for (int i = 0; i < _bonemaps.size(); i++) {
			GetBoneMatrices(animation.bones[i], i);
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

	Animator::upAnimNode Animator::_createNodeTree(aiNode* node, AnimNode* parent) {
		upAnimNode internalnode = std::make_unique<AnimNode>(node->mName.data, parent, _animct);

		internalnode->_local_transform = aiMatrix4x4ToGlm(node->mTransformation);
		_getGlobalTransform(internalnode.get());

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