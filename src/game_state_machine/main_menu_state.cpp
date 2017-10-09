#include "game_state_machine/main_menu_state.h"

namespace SAS_3D {
	namespace GSM {
		MainMenuState::MainMenuState() {

		}

		MainMenuState::~MainMenuState() {

		}

		FSMStates MainMenuState::InitializeState(Core::SASWindow* window, const Core::InputState& input) {
			return FSMStates::TRANSITIONIN;
		}

		FSMStates MainMenuState::UpdateState(int elapsedtime, Core::SASWindow* window, const Core::InputState& input) {
			_nextstate = 1;
			return FSMStates::TRANSITIONOUT;
		}
	}
}