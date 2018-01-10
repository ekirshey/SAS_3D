#pragma once
#include "game_state_machine/game_state.h"
#include "game_state_machine/game_config.h"
#include "assets/model_container.h"
#include "entities/camera.h"
#include "subsystems/physics/rigid_body.h"
#include "subsystems/render_engine/render_engine.h"
#include "subsystems/render_engine/scene_graph.h"
#include "utility/locking_queue.h"

namespace SAS_3D {
	class GameRunningState : public GameStateImpl
	{
	public:
		GameRunningState(const GameConfig& config);
		~GameRunningState();

		FSMStates InitializeState(SubsystemController* subsystems, const InputState& input);
		FSMStates UpdateState(int elapsedtime, SubsystemController* subsystems, const InputState& input);
		int NextState() { return _nextstate; }

	private:
		GameConfig _config;
		Camera _camera;
		int _nextstate;
		bool _exit;

		// TEST
		std::vector<RigidBody> _mobs;
	};
}
