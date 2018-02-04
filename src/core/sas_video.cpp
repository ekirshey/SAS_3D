#include <iostream>
#include "sas_video.h"
#include <GL/glew.h>
#include "core/error_codes.h"

namespace SAS_3D {
	void CheckSDLError(int line = -1) {
		std::string error = SDL_GetError();

		if (error != "") {
			std::cout << "SDL Error : " << error << std::endl;

			if (line != -1) {
				std::cout << "\nLine : " << line << std::endl;
			}

			SDL_ClearError();
		}
	}

	// I think im being confused between window size and resolution
	SASWindow::SASWindow(std::string title, int width, int height, int window_flags)
		: _window(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags))
		, _glcontext(SDL_GL_CreateContext(_window))
	{
		int w, h;
		SDL_GL_GetDrawableSize(_window, &w, &h);
		_screenwidth = w;
		_screenheight = h;

		glewExperimental = GL_TRUE;
		glewInit();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_FRAMEBUFFER_SRGB); // Enable gamma correction

		// Check that everything worked out okay
		if (!_window) {
			std::cout << "Unable to create window\n" << std::endl;;
			CheckSDLError(__LINE__);
			// throw here
		}
		glViewport(0, 0, _screenwidth, _screenheight);
	}

	SASWindow::~SASWindow() {
		// Clean up SDL and OpenGL context
		SDL_GL_DeleteContext(_glcontext);
		SDL_DestroyWindow(_window);
		SDL_Quit();
	}

	void SASWindow::Clear(float r, float g, float b) {
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0,0, _screenwidth, _screenheight);
	}

	void SASWindow::TurnOnWireframe() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	uptrSASWindow InitializeVideo(const VideoConfig& config) {
		SetError(ErrorCode::NO_ERROR);

		// Start up SDL
		// Make sure it hasn't already been initialized.
		// You could hypothetically create multiple windows thru this 
		// function while only initializing SDL once.
		auto subsystem_init = SDL_WasInit(SDL_INIT_EVERYTHING);
		if (!(subsystem_init & SDL_INIT_VIDEO)) {
			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
				printf("Error: %s\n", SDL_GetError());
				SetError(ErrorCode::SDL_ERROR);
				return nullptr;
			}

			// Setup window
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
			SDL_DisplayMode current;
			SDL_GetCurrentDisplayMode(0, &current);
		}

		float ddpi = -1;
		float hdpi = -1;
		float vdpi = -1;

		int ret = SDL_GetDisplayDPI(0, &ddpi, &hdpi, &vdpi);
		if (ret != 0) {
			std::cout << "Error getting screen dpi values " << ret<< std::endl;
		}

		int window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
		if (config.fullscreen) {
			window_flags |= SDL_WINDOW_FULLSCREEN;
		}

		// Construct the window
		auto win =  std::make_unique<SASWindow>(config.windowtitle, config.width, config.height, window_flags);

		// After window is created check for other settings like vsync
		win->SetVSYNC(config.vsync);

		return win;
	}

	// vsync true enables vsync, false turns it off
	void SASWindow::SetVSYNC(bool vsync) {
		if (!vsync) {
			if (SDL_GL_SetSwapInterval(0) != 0) {
				printf("Error: %s\n", SDL_GetError());
			}
		}
		else {
			if (SDL_GL_SetSwapInterval(1) != 0) {
				printf("Error: %s\n", SDL_GetError());
			}
		}
	}
}