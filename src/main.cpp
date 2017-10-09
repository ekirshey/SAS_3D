#include <iostream>
#include "game_state_machine/game.h"
#include "game_state_machine/main_menu_state.h"
#include "game_state_machine/game_running_state.h"

using namespace SAS_3D;
int main(int argc, char* argv[]) {
	GSM::Game game("F:/github/SAS_3D/bin/Debug/config.ini");

	game.AddState<GSM::MainMenuState>(true,0);
	game.AddState<GSM::GameRunningState>(false,1,game.ViewportWidth(), game.ViewportHeight());

	game.Run();

	return 0;
}