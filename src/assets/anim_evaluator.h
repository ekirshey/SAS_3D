#pragma once
#include <vector>
#include <memory>
#include <tuple>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "anim_node.h"

namespace SAS_3D {
	// helper https://stackoverflow.com/questions/29184311/how-to-rotate-a-skinned-models-bones-in-c-using-assimp
	glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from);

	template<typename T>
	struct Key {
		double time;
		T val;

		Key(double t, T v) 
			: time(t)
			, val(v)
		{

		}
	};

	struct Channel {
		std::vector<Key<glm::vec3>> positions;
		std::vector<Key<glm::quat>> rotations;
		std::vector<Key<glm::vec3>> scalings;

		std::tuple<int, int, int> lastindices;
		glm::mat4 currenttransform;
	};

	class AnimEvaluator {
	public:
		AnimEvaluator(int id, AnimNode* rootnode, const aiAnimation* animation);
		void Calculate(double time);
		glm::mat4 GetTransform(int channelid) { return _channels[channelid].currenttransform; }
	private:
		std::vector<Channel> _channels;
		double _tickspersecond;

	};
}
