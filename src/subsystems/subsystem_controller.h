#pragma once
#include <memory>
#include "game_state_machine/game_config.h"
#include "render_engine/render_engine.h"
#include "animation_engine/anim_engine.h"
#include "network/client.h"

namespace SAS_3D {
	class SASWindow;
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
		std::unique_ptr<SASWindow> _window;

		//Render Subsystem
		RenderEngine _renderengine;
		AnimationEngine _animengine;

		Client _client;
	};

}