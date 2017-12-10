#include "game_state_machine/game.h"
#include <iostream>
#include <sstream>

const int SCREEN_FPS = 1000;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

namespace SAS_3D {
	Game::Game(std::string config)
		: _gamestates()
		, _activestate(0)
		, _config(LoadConfig(config))
		, _window(InitializeVideo(_config.windowtitle, _config.screenwidth, _config.screenheight))
		, _gamerunning(true)
		, _client(_config.serverip, _config.port)
	{
	}

	Game::~Game() {
	}

	void Game::RemoveStateAtIndex(int idx) {

	}

	void Game::Run() {
		int currenttime = 0;
		int previoustime = 0;
		int framecounter = 0;
		double lowfps = 10000;
		double highfps = 0;
		double fps = 0;
		double samples = 0;
		InputState inputstate;

		// If everything initialized ok then enter main game loop
		std::cout << "Finished Game Init" << std::endl;
		std::cout << "Entering Main Loop" << std::endl << std::endl;

		currenttime = SDL_GetTicks();

		// DISABLES VSYNC
		if (SDL_GL_SetSwapInterval(0) != 0) {
			printf("Error: %s\n", SDL_GetError());
		}

		// Main Game Loop
		_window->SwapWindow();
		while (_gamerunning)
		{
			_window->Clear(0.2f, 0.3f, 0.3f);
			previoustime = currenttime;
			currenttime = SDL_GetTicks();

			// Read the event queue so the state has the input
			UpdateInput(_gamerunning, inputstate);
			if (_gamerunning) {
				Update(currenttime - previoustime, inputstate);

				/*
				if (fps < 50) {
					std::cout << "LOW FPS " << fps << std::endl;
				}
				*/
				_window->SwapWindow();
			}

			if ((currenttime - previoustime) > 0) {
				++samples;
				auto t = 1000.0 / (currenttime - previoustime);
				if (t < lowfps && t != 0)
					lowfps = t;
				if (t > highfps)
					highfps = t;
				fps = t;
			}

			framecounter++;
		}   // End Main Loop

	}

	void Game::Update(int elapsedtime, const InputState& inputstate) {
		_activestate = _gamestates[_activestate]->FiniteStateMachine(elapsedtime, _window.get(), inputstate);
		// Invalid next state
		if (_activestate >= _gamestates.size() || _activestate < 0) {
			_gamerunning = false;
		}
	}
}
