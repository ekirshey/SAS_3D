#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <assimp/scene.h>
#include "anim_evaluator.h"
#include "anim_node.h"

/*
	Each mesh has a set of bones and are all bones mapped to a single bonemap?
	How do I specify the bones needed for each individual mesh?
	
*/
namespace SAS_3D {
	class Animator {
	public:
		Animator(const aiScene* scene);
		void SetAnimationIndex(int i);
		void Calculate(double time);
		void GetBoneMatrices(std::vector<glm::mat4>& matrices, int meshidx);
	private:
		using upAnimNode = std::unique_ptr<AnimNode>;

		int _animidx;
		int _animct;
		std::vector<AnimEvaluator> _animations;

		// Root node tree.
		std::unique_ptr<AnimNode> _rootnode;
		// Node containing mesh transform
		AnimNode* _meshnode;

		// Map bone indices to nodes in the tree
		// Each BoneVec corresponds to a mesh. Numbering for bones
		// starts at 0 for each mesh
		using BoneVec = std::vector<AnimNode*>;
		std::vector<BoneVec> _bonemaps;

		using Skeleton = std::vector<glm::mat4>;
		using MeshSkeletons = std::vector<Skeleton>;
		MeshSkeletons _meshskeletons;

		void _getGlobalTransform(AnimNode* node);
		upAnimNode _createNodeTree(aiNode* node, AnimNode* parent);
		void _updateNodes(AnimNode* node);
	};
}
