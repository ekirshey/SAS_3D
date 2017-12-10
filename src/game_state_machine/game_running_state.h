#pragma once
#include "game_state_machine/game_state.h"
#include "game_state_machine/game_config.h"
#include "assets/model_container.h"
#include "entities/camera.h"
#include "physics/rigid_body.h"
#include "render_engine/render_engine.h"
#include "render_engine/scene_graph.h"
#include "utility/locking_queue.h"

namespace SAS_3D {
	class GameRunningState : public GameStateImpl
	{
	public:
		GameRunningState(const GameConfig& config);
		~GameRunningState();

		FSMStates InitializeState(SASWindow* window, const InputState& input);
		FSMStates UpdateState(int elapsedtime, SASWindow* window, const InputState& input);
		int NextState() { return _nextstate; }

	private:
		GameConfig _config;
		Camera _camera;
		LockingQueue<SceneGraph> _scene_queue;
		int _nextstate;
		bool _exit;

		// TEST
		RigidBody _player;
	};
}
