#include "game_state_machine/main_menu_state.h"
#include "render_engine/render_engine.h"

namespace SAS_3D {
	MainMenuState::MainMenuState() {

	}

	MainMenuState::~MainMenuState() {

	}

	FSMStates MainMenuState::InitializeState(const InputState& input, RenderQueue* event_queue) {
		return FSMStates::TRANSITIONIN;
	}

	FSMStates MainMenuState::UpdateState(int elapsedtime, RenderQueue* event_queue, const InputState& input) {
		_nextstate = 1;
		return FSMStates::TRANSITIONOUT;
	}
}