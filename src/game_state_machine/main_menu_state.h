#pragma once
#include "game_state_machine/game_state.h"

namespace SAS_3D {
	namespace GSM {
		class MainMenuState : public GameStateImpl
		{
			public:
				MainMenuState();
				~MainMenuState();

				FSMStates InitializeState(Core::SASWindow* window, const Core::InputState& input);
				FSMStates UpdateState(int elapsedtime, Core::SASWindow* window, const Core::InputState& input);
				int NextState() { return _nextstate; }

			private:
				int _nextstate;
				bool _exit;
		};
	}
}
