#pragma once
#include <thread>
#include <future>
#include <atomic>
#include "anim_state.h"
#include "animator.h"
#include "game_state_machine/game_config.h"
#include "ecs/ecs_defines.h"

namespace SAS_3D {
	using AnimationContainer = std::vector<Animator>;
	class AnimationEngine {
	public:
		AnimationEngine();
		~AnimationEngine();
		void Initialize(AnimationContainer&& ac);
		int AddAnimationInstance(int id, int index, const FrameIndices& frameindices);
		void StartAsyncBoneCalculations(double time);
		std::vector<AnimationState> CollectBoneCalculations();
	private:
		bool _calculateBoneMatrices(double time);
		bool _asyncrunning;
		AnimationContainer _animations;
		std::vector<AnimationState> _instances;
		std::future<bool> _boneresults;
	};
}