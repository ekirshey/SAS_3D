/*
* sas_sdl.h
*
* Functions related to initializing the SDL subsystems. In the engines current state
* it uses SDL for window management, input and audio. If this ever changes in the future
* it will be reflected in this file.
*
*
* Author: Erik Kirshey
* Date: 10/3/17
*/
#pragma once
#include <string>
#include <memory>
#include <SDL.h>

namespace SAS_3D {
	/*
	* Simple wrapper around the SDL Window.
	* Makes certain values(height, width) more readily available
	* and hides the various SDL functions
	*/
	class SASWindow {
	public:
		SASWindow(std::string title, int width, int height, int window_flags);
		~SASWindow();

		void SwapWindow() { SDL_GL_SwapWindow(_window); }
		void Clear(float r, float g, float b);
		void TurnOnWireframe();
		int GetScreenWidth() { return _screenwidth; }
		int GetScreenHeight() { return _screenheight; }
		void SwitchContext() { 
			SDL_GL_MakeCurrent(_window, _glcontext); 
			std::cout << SDL_GetError() << std::endl;
		}
	private:
		SDL_Window* _window;
		SDL_GLContext _glcontext;
		int _screenwidth;
		int _screenheight;
	};

	using uptrSASWindow = std::unique_ptr<SASWindow>;
	/*
	* Takes in an ini file with parameters used for initializing SDL and OpenL
	* ...and then initializes opengl and sdl
	* ec will hold possible error information, set to NO_ERROR if good
	*
	*/
	uptrSASWindow InitializeVideo(std::string windowtitle, int width, int height);
}