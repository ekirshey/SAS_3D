#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include "game_state_machine/game.h"
#include "assets/assimp_loader.h"

const int SCREEN_FPS = 1000;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

namespace SAS_3D {
	Game::Game(std::string config)
		: _config(LoadConfig(config))
		, _gamestates{MAX_STATES}
		, _activestate(0)
		, _subsystems(_config)
		, _quitgame(false)
	{
	}

	Game::~Game() {

	}

	void Game::RemoveStateAtIndex(int idx) {

	}

	void Game::Run() {
		double fps = 0;
		double samples = 0;
		InputState inputstate;

		// If everything initialized ok then enter main game loop
		std::cout << "Kicking off render thread" << std::endl;

		//using namespace std::chrono_literals;
		//std::this_thread::sleep_for(5s);

		std::cout << "Finished Game Init" << std::endl;
		std::cout << "Entering Main Loop" << std::endl << std::endl;

		auto current = std::chrono::high_resolution_clock::now();
		std::chrono::microseconds targetticks_us(16670);

		// Spin up all subsystems and threads
		_subsystems.Bootstrap();

		while (!_quitgame)
		{
			auto previous = current;
			current = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(current - previous).count();
			// Read the event queue so the state has the input
			UpdateInput(_quitgame, inputstate);
			Update(elapsed, inputstate);

			auto end = std::chrono::high_resolution_clock::now();

			auto t = std::chrono::duration_cast<std::chrono::microseconds>(end - current);
			std::this_thread::sleep_for(targetticks_us - t);
			//std::cout << (targetticks_us - t).count() << std::endl;
		}   // End Main Loop

	}

	void Game::Update(long long elapsedtime, const InputState& inputstate) {
		_activestate = _gamestates[_activestate]->FiniteStateMachine(elapsedtime, &_subsystems, inputstate);
		// Invalid next state
		if (_activestate >= _gamestates.size() || _activestate < 0) {
			_quitgame = false;
		}
	}
}
