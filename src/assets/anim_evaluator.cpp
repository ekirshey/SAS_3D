#include <iostream>
#include "anim_evaluator.h"

namespace SAS_3D {
	glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from) {
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}

	AnimEvaluator::AnimEvaluator(int id, AnimNode* rootnode, const aiAnimation* animation) {
		std::cout << "Processing new animation: " << animation->mName.data << std::endl;
		if (animation->mNumChannels <= 0) {
			std::cout << "Error: Animation has no channels" << std::endl;
		}
		double _ticksPerSecond = animation->mTicksPerSecond != 0.0 ? animation->mTicksPerSecond : 25.0;

		// Process channels/nodes
		for (int i = 0; i < animation->mNumChannels; i++) {
			auto ai_c = animation->mChannels[i];
			_channels.push_back(Channel());
			auto c = _channels.back();
			auto node = rootnode->FindNode(ai_c->mNodeName.data);
			if (node != nullptr) {
				node->_channelids[id] = _channels.size() - 1;
				// Build up the position, rotation and scaling keys
				for (int p = 0; p < ai_c->mNumPositionKeys; p++) {
					auto t = ai_c->mPositionKeys[p].mTime;
					auto ai_v = &ai_c->mPositionKeys[p].mValue;
					glm::vec3 v(ai_v->x, ai_v->y, ai_v->z);;
					c.positions.push_back(Key<glm::vec3>(t, v));
				}

				for (int r = 0; r < ai_c->mNumRotationKeys; r++) {
					auto t = ai_c->mRotationKeys[r].mTime;
					auto ai_q = &ai_c->mRotationKeys[r].mValue;
					glm::quat q(ai_q->w, ai_q->x, ai_q->y, ai_q->z);
					c.rotations.push_back(Key<glm::quat>(t, q));
				}

				for (int s = 0; s < ai_c->mNumScalingKeys; s++) {
					auto t = ai_c->mScalingKeys[s].mTime;
					auto ai_v = &ai_c->mScalingKeys[s].mValue;
					glm::vec3 v(ai_v->x, ai_v->y, ai_v->z);;
					c.scalings.push_back(Key<glm::vec3>(t, v));
				}

				// Set all indices to 0
				std::get<0>(c.lastindices) = 0;
				std::get<1>(c.lastindices) = 0;
				std::get<2>(c.lastindices) = 0;
			}
			else {
				std::cout << "Error: Channel " << ai_c->mNodeName.data << " has no corresponding node" << std::endl;
			}
		}
	}

	void AnimEvaluator::Calculate(double time) {

		for (int i = 0; i < _channels.size(); i++) {

		}
	}
}