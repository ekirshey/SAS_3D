#pragma once
#include "game_state_machine/game_state.h"

namespace SAS_3D {
	class MainMenuState : public GameStateImpl
	{
	public:
		MainMenuState();
		~MainMenuState();

		FSMStates InitializeState(SubsystemController* subsystems, const InputState& input);
		FSMStates UpdateState(long long elapsedtime, SubsystemController* subsystems, const InputState& input);
		int NextState() { return _nextstate; }

	private:
		int _nextstate;
		bool _exit;
	};
}
