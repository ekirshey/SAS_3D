#include "game_state_machine/game.h"
#include <iostream>
#include <sstream>

// TODO
const int SCREEN_FPS = 1000;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

namespace SAS_3D {
	namespace GSM {
		Game::Game(std::string config)
			: _gamestates()
			, _activestate(0)
			, _config(LoadConfig(config))
			, _window(Core::InitializeVideo(_config.windowtitle, _config.screenwidth, _config.screenheight))
			, _gamerunning(true)
		{
		}

		Game::~Game()
		{
		}

		void Game::RemoveStateAtIndex(int idx)
		{

		}

		void Game::Run()
		{
			int currenttime = 0;
			int previoustime = 0;
			int framecounter = 0;
			double lowfps = 10000;
			double highfps = 0;
			double fps = 0;
			double samples = 0;
			Core::InputState inputstate;

			// If everything initialized ok then enter main game loop
			std::cout << "Finished Game Init" << std::endl;
			std::cout << "Entering Main Loop" << std::endl << std::endl;

			//currenttime = SAS_System::CurrentTicks();

			// Main Game Loop
			_window->SwapWindow();
			while (_gamerunning)
			{
				_window->Clear(0.2f, 0.3f, 0.3f);
				previoustime = currenttime;
				//currenttime = SAS_System::CurrentTicks();

				// Read the event queue so the state has the input
				Core::UpdateInput(_gamerunning, inputstate);
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
					auto t = 1000 / (currenttime - previoustime);
					if (t < lowfps && t != 0)
						lowfps = t;
					if (t > highfps)
						highfps = t;
					fps = t;
				}

				framecounter++;
			}   // End Main Loop

		}


		void Game::Update(int elapsedtime, const Core::InputState& inputstate) {
			_activestate = _gamestates[_activestate]->FiniteStateMachine(elapsedtime, _window.get(), inputstate);
			// Invalid next state
			if (_activestate >= _gamestates.size() || _activestate < 0) {
				_gamerunning = false;
			}
		}
	}
}
