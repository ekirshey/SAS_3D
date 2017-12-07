#include <iostream>
#include <thread>
#include "game_state_machine/game.h"
#include "game_state_machine/main_menu_state.h"
#include "game_state_machine/game_running_state.h"
#include "network/client.h"
#include "asio.hpp"

using namespace SAS_3D;
using namespace asio::ip;
int main(int argc, char* argv[]) {
    asio::io_service io_service;
	tcp::resolver resolver(io_service);
	auto endpoint_iterator = resolver.resolve({ "127.0.0.1", "25977" });
	Network::Client c(io_service, endpoint_iterator);

    std::thread t([&io_service](){ io_service.run(); });
	GSM::Game game("F:/github/SAS_3D/bin/Debug/config.ini");

	game.AddState<GSM::MainMenuState>(true,0);
	game.AddState<GSM::GameRunningState>(false,1,game.Config());

	game.Run();

	return 0;
}