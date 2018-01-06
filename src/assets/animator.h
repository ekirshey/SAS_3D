#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <assimp/scene.h>
#include <glm/glm.hpp>

/*
	Each mesh has a set of bones and are all bones mapped to a single bonemap?
	How do I specify the bones needed for each individual mesh?
	
*/

namespace SAS_3D {
	struct AnimNode {
		AnimNode* _parent;
		std::vector<std::unique_ptr<AnimNode>> _children;

		std::string _name;

		glm::mat4 _local_transform;
		glm::mat4 _global_transform;

		AnimNode* FindNode(std::string name) {
			if (_name == name) {
				return this;
			}
			for (int i = 0; i < _children.size(); i++) {
				auto n = _children[i]->FindNode(name);
				if (n != nullptr) {
					return n;
				}
			}

			return nullptr;
		}

		AnimNode(std::string name, AnimNode* parent) 
			: _name(name)
			, _parent(parent)
		{

		}
	};

	class Animator {
	public:
		Animator(const aiScene* scene);
		void GetBoneMatrices(std::vector<glm::mat4>& matrices, int meshidx);
	private:
		using pAnimNode = std::unique_ptr<AnimNode>;

		pAnimNode _createNodeTree(aiNode* node, AnimNode* parent);

		std::unique_ptr<AnimNode> _rootnode;
		AnimNode* _meshnode;

		// Map bone indices to nodes in the tree
		using BoneMap = std::unordered_map<int, AnimNode*>;
		// Each BoneMap corresponds to a mesh. Numbering for bones
		// starts at 0 for each mesh
		std::vector<BoneMap> _bonemaps;

		using Skeleton = std::vector<glm::mat4>;
		using MeshSkeletons = std::unordered_map<int, Skeleton>;
		MeshSkeletons _meshskeletons;
	};
}
