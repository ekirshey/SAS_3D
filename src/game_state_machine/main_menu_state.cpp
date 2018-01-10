#include "game_state_machine/main_menu_state.h"
#include "subsystems/render_engine/render_engine.h"

namespace SAS_3D {
	MainMenuState::MainMenuState() {

	}

	MainMenuState::~MainMenuState() {

	}

	FSMStates MainMenuState::InitializeState(SubsystemController* subsystems, const InputState& input) {
		return FSMStates::TRANSITIONIN;
	}

	FSMStates MainMenuState::UpdateState(int elapsedtime, SubsystemController* subsystems, const InputState& input) {
		_nextstate = 1;
		return FSMStates::TRANSITIONOUT;
	}
}