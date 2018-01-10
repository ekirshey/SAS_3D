#pragma once
#include <thread>
#include <atomic>
#include "anim_container.h"
#include "game_state_machine/game_config.h"

namespace SAS_3D {

	#define MAXBUFFERS 2
	using BoneMatrix = std::vector<std::vector<glm::mat4*>>;
	using BoneData = std::atomic<BoneMatrix*>;
	struct AnimationState {
		int animationidx;
		std::tuple<int, int, int> frameindices;
		BoneMatrix buffers[MAXBUFFERS];
		BoneData dataptr;
	};

	class AnimationImpl {
	public:
		AnimationImpl();
		void Initialize(AnimationContainer&& ac) { _animations = std::move(ac); }
		void Run();
		void Stop() { _running = false; }
		bool isRunning() { return _running; }
	private:
		bool _running;
		AnimationContainer _animations;
		std::vector<AnimationState> _instances;
	};

	class AnimationEngine {
	public:
		AnimationEngine() {}
		~AnimationEngine();
		void Start(AnimationContainer&& ac);
		bool isRunning() { return _impl.isRunning(); }
	private:
		AnimationImpl _impl;
		std::thread _animthread;
	};
}
