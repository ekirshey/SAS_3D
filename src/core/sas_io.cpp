#include <SDL.h>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include "sas_io.h"
#include "error_codes.h"

namespace SAS_3D {
	void UpdateInput(bool& quit, InputState& input) {
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type) {
			case SDL_QUIT:
				/* Quit */
				quit = true;
				break;
			case SDL_KEYDOWN:
				input.keyarray[event.key.keysym.scancode] = KeyState::PRESSED;
				break;
			case SDL_KEYUP:
				input.keyarray[event.key.keysym.scancode] = KeyState::RELEASED;
				break;
			case SDL_MOUSEWHEEL:
				input.m_wheel = event.wheel.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					input.m_left = KeyState::PRESSED;
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) {
					input.m_right = KeyState::PRESSED;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					input.m_left = KeyState::RELEASED;
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) {
					input.m_right = KeyState::RELEASED;
				}
				break;
			default:
				break;
			}
		}
		SDL_GetMouseState(&input.m_x, &input.m_y);
	}

	std::string ReadFile(std::string file) {
		SetError(ErrorCode::NO_ERROR);

		// Open file
		std::ifstream ifstrm(file);
		if (!ifstrm.is_open()) {
			SetError(ErrorCode::FAILED_TO_OPEN_FILE);
			return "";
		}

		// Read file into buffer
		std::stringstream buffer;
		buffer << ifstrm.rdbuf();

		// Make a std::string and fill it with the contents of buffer
		std::string fileContent = buffer.str();

		return fileContent;
	}
}