#pragma once
#include <glm/glm.hpp>

namespace SAS_3D {
	struct AnimNode {
		AnimNode* _parent;
		std::vector<std::unique_ptr<AnimNode>> _children;

		std::string _name;

		glm::mat4 _local_transform;
		glm::mat4 _global_transform;

		// Corresponding channel id in each animation state
		std::vector<int> _channelids;

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

		AnimNode(std::string name, AnimNode* parent, int animations)
			: _name(name)
			, _parent(parent)
			, _channelids(animations, -1)
		{

		}
	};
}
