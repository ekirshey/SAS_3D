#pragma once
#include <memory>
#include <thread>
#include "utility/locking_queue.h"
#include "game_state_machine/game_config.h"
#include "render_engine/render_engine.h"
#include "animation_engine/anim_engine.h"
#include "network/client.h"

namespace SAS_3D {
	class SubsystemController
	{
	public:
		SubsystemController(const GameConfig& config);
		~SubsystemController();

		bool Bootstrap();

		// Getters
		RenderEngine* GetRenderEngine() { return &_renderengine; }
		AnimationEngine* GetAnimationEngine() { return &_animengine; }

	private:
		GameConfig _config;
		uptrSASWindow _window;

		//Render Subsystem
		RenderEngine _renderengine;
		AnimationEngine _animengine;

		Client _client;
	};

}