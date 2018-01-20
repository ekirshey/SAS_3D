#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
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

	AnimEvaluator::AnimEvaluator(int id, AnimNode* rootnode, const aiAnimation* animation) 
		: _lasttime(0)
	{
		std::cout << "Processing new animation: " << animation->mName.data << std::endl;
		if (animation->mNumChannels <= 0) {
			std::cout << "Error: Animation has no channels" << std::endl;
		}
		_tickspersecond = animation->mTicksPerSecond != 0.0 ? animation->mTicksPerSecond : 5.0;
		_mspertick = 1000 / _tickspersecond;
		_duration = animation->mDuration;
		_animlength = _duration / _tickspersecond;

		// Process channels/nodes
		for (int i = 0; i < animation->mNumChannels; i++) {
			auto ai_c = animation->mChannels[i];
			_channels.push_back(Channel());
			auto c = &_channels.back();
			auto node = rootnode->FindNode(ai_c->mNodeName.data);
			if (node != nullptr) {
				node->_channelids[id] = _channels.size() - 1;
				// Build up the position, rotation and scaling keys
				for (int p = 0; p < ai_c->mNumPositionKeys; p++) {
					auto t = ai_c->mPositionKeys[p].mTime;
					auto ai_v = &ai_c->mPositionKeys[p].mValue;
					glm::vec3 v(ai_v->x, ai_v->y, ai_v->z);;
					c->positions.push_back(Key<glm::vec3>(t, v));
				}

				for (int r = 0; r < ai_c->mNumRotationKeys; r++) {
					auto t = ai_c->mRotationKeys[r].mTime;
					auto ai_q = &ai_c->mRotationKeys[r].mValue;
					glm::quat q(ai_q->w, ai_q->x, ai_q->y, ai_q->z);
					c->rotations.push_back(Key<glm::quat>(t, q));
				}

				for (int s = 0; s < ai_c->mNumScalingKeys; s++) {
					auto t = ai_c->mScalingKeys[s].mTime;
					auto ai_v = &ai_c->mScalingKeys[s].mValue;
					glm::vec3 v(ai_v->x, ai_v->y, ai_v->z);;
					c->scalings.push_back(Key<glm::vec3>(t, v));
				}
			}
			else {
				std::cout << "Error: Channel " << ai_c->mNodeName.data << " has no corresponding node" << std::endl;
			}
		}
		std::cout << "done.." << std::endl;
	}

	void AnimEvaluator::Calculate(double rawtime, FrameIndices& lastindices) {

		// _duration is specified in ticks
		// 55 ticks is duration
		// 25 ticks per second
		// duration lasts 2.2 seconds
		// tick every 0.04 seconds
		if (lastindices.size() != _channels.size()) {
			lastindices.resize(_channels.size());
		}

		double seconds = rawtime / 1000;
		seconds *= _tickspersecond;
		double time = fmod(seconds, _duration);
		//std::cout << "S " << rawtime << " " << time << std::endl;
		for (int i = 0; i < _channels.size(); i++) {
			Channel* c = &_channels[i];
			glm::mat4 translation;
			glm::mat4 rotation;
			glm::mat4 scaling;

			// Handle Position
			if (c->positions.size() > 0) {
				int frame = std::get<0>(lastindices[i]);
				if (frame >= c->positions.size()) { frame = 0; }

				auto initialtime = c->positions[frame].time;
				while (frame < c->positions.size()-1) {
					if (time < c->positions[frame+1].time) {
						break;
					}
					frame++;
				}
				int nextframe = (frame + 1) % c->positions.size();
				auto key = &c->positions[frame];
				auto nextkey = &c->positions[nextframe];
				double difftime = nextkey->time - key->time;
				glm::vec3 position;
				if (difftime < 0.0)
					difftime += _duration;
				if (difftime > 0) {
					float factor = float((time - key->time) / difftime);
					position = key->val + (nextkey->val - key->val) * factor;
				}
				else {
					position = key->val;
				}

				translation = glm::translate(translation, position);

				std::get<0>(lastindices[i]) = frame;
			}

			// Handle Rotations
			if (c->rotations.size() > 0) {
				int frame = std::get<1>(lastindices[i]);
				if (frame >= c->rotations.size()) { frame = 0; }

				auto initialtime = c->rotations[frame].time;
				while (frame < c->rotations.size() - 1) {
					if (time < c->rotations[frame + 1].time) {
						break;
					}
					frame++;
				}

				int nextframe = (frame + 1) % c->rotations.size();
				auto key = &c->rotations[frame];
				auto nextkey = &c->rotations[nextframe];
				double difftime = nextkey->time - key->time;
				//std::cout << "T " << difftime << " " << nextkey->time << " " << key->time << " " << time << " " << frame << std::endl;
				glm::quat quaternion;
				if (difftime < 0.0)
					difftime += _duration;
				if (difftime > 0) {
					float factor = float((time - key->time) / difftime);
					quaternion = glm::lerp(key->val, nextkey->val, factor);
				}
				else {
					quaternion = key->val;
				}

				rotation = glm::toMat4(quaternion);

				std::get<1>(lastindices[i]) = frame;
			}

			// Handle Scaling
			if (c->scalings.size() > 0) {
				int frame = std::get<2>(lastindices[i]);
				if (frame >= c->scalings.size()) { frame = 0; }

				auto initialtime = c->scalings[frame].time;
				while (frame < c->scalings.size() - 1) {
					if (time < c->scalings[frame + 1].time) {
						break;
					}
					frame++;
				}
				int nextframe = (frame + 1) % c->scalings.size();
				auto key = &c->scalings[frame];
				auto nextkey = &c->scalings[nextframe];
				double difftime = nextkey->time - key->time;
				glm::vec3 scale;
				if (difftime < 0.0)
					difftime += _duration;
				if (difftime > 0) {
					float factor = float((time - key->time) / difftime);
					scale = key->val + (nextkey->val - key->val) * factor;
				}
				else {
					scale = key->val;
				}

				scaling = glm::scale(scaling, scale);

				std::get<2>(lastindices[i]) = frame;
			}

			c->currenttransform = translation * rotation * scaling;
		}
		//_lasttime = time;
	}
}