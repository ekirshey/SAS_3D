#include "sas_input.h"
#include <SDL.h>

namespace SAS_3D {
	namespace Core {
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

	}
}