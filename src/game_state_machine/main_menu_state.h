#pragma once
#include "game_state_machine/game_state.h"

namespace SAS_3D {
	class MainMenuState : public GameStateImpl
	{
	public:
		MainMenuState();
		~MainMenuState();

		FSMStates InitializeState(const InputState& input, RenderQueue* event_queue);
		FSMStates UpdateState(int elapsedtime, RenderQueue* event_queue, const InputState& input);
		int NextState() { return _nextstate; }

	private:
		int _nextstate;
		bool _exit;
	};
}
