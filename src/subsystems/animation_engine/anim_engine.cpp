#include "anim_engine.h"

namespace SAS_3D {

	AnimationImpl::AnimationImpl()
		: _running(false)
	{
	}

	void AnimationImpl::Run() {
		_running = true;
		while (_running) {
			/*
				Listen for animation updates
				Check for tick
				if time to update
					for each animationstate
						Get new update
						Write update to secondary buffer
						swap buffer ptr
			*/

		}
	}


	/// Engine wrapper
	AnimationEngine::~AnimationEngine() {
		_impl.Stop();
		_animthread.join();
	}

	void AnimationEngine::Start(AnimationContainer&& ac) {
		_impl.Initialize(std::move(ac));
		_animthread = std::thread([&]() {
			_impl.Run();
		});
	}
}