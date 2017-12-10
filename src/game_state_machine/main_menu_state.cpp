#include "game_state_machine/main_menu_state.h"

namespace SAS_3D {
	MainMenuState::MainMenuState() {

	}

	MainMenuState::~MainMenuState() {

	}

	FSMStates MainMenuState::InitializeState(SASWindow* window, const InputState& input) {
		return FSMStates::TRANSITIONIN;
	}

	FSMStates MainMenuState::UpdateState(int elapsedtime, SASWindow* window, const InputState& input) {
		_nextstate = 1;
		return FSMStates::TRANSITIONOUT;
	}
}