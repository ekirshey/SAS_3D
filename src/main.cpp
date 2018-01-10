#include <iostream>
#include "game_state_machine/game.h"
#include "game_state_machine/main_menu_state.h"
#include "game_state_machine/game_running_state.h"
#include "subsystems/network/client.h"

using namespace SAS_3D;
using namespace asio::ip;
int main(int argc, char* argv[]) {
	Game game("../../config.ini");

	game.AddState<MainMenuState>(true,0);
	game.AddState<GameRunningState>(false,1,game.Config());

	game.Run();
	return 0;
}