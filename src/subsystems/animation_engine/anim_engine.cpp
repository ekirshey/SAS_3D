#include <thread>
#include <iostream>
#include "anim_engine.h"

namespace SAS_3D {
	/// Engine wrapper
	AnimationEngine::AnimationEngine() 
		: _asyncrunning(false)
	{
	}

	AnimationEngine::~AnimationEngine()
	{
	}

	bool AnimationEngine::_calculateBoneMatrices(double time) {
		for (int i = 0; i < _instances.size(); i++) {
			_animations[_instances[i].id].CalculateBoneMatrices(time, _instances[i]);
		}
		return true;
	}

	void AnimationEngine::Initialize(AnimationContainer&& ac) {
		_animations = std::move(ac);
	}

	int AnimationEngine::AddAnimationInstance(int id, int index, std::tuple<int, int, int> frameindices) {
		if (_asyncrunning) {
			std::cout << "Wait for animation processing to finish" << std::endl;
			return -1;
		}
		AnimationState animation;
		animation.id = id;
		animation.index = index;
		animation.frameindices = frameindices;
		if (animation.id < _animations.size()) {
			_instances.push_back(animation);
		}
		else {
			std::cout << "Invalid animation id: " << animation.id;
		}

		return _instances.size() - 1;
	}

	void AnimationEngine::StartAsyncBoneCalculations(double time) {
		_asyncrunning = true;
		_boneresults = std::async(std::launch::async, &AnimationEngine::_calculateBoneMatrices, this, time);
	}

	std::vector<AnimationState> AnimationEngine::CollectBoneCalculations() {
		if (_asyncrunning) {
			auto val = _boneresults.get();
			_asyncrunning = false;
		}
		return std::move(_instances);
	}
}