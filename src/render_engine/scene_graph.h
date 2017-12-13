#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace SAS_3D {
	class SceneGraph {
	public:
		SceneGraph() {}
		struct SceneNode {
			int modelid;
			glm::mat4 mvp;
		};

		void AddNode(const SceneNode& node) {
			_data.push_back(node);
		}
		// Temp
		std::vector<SceneNode> _data;
	};
}
