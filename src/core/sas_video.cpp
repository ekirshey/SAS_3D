#include <iostream>
#include "sas_video.h"
#include <GL/glew.h>
#include "third_party/INIReader.h"

namespace SAS_3D {
	namespace Core {
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

		SASWindow::SASWindow(std::string title, int width, int height, int window_flags) 
			: _window(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE))
			, _glcontext(SDL_GL_CreateContext(_window))
			, _screenwidth(width)
			, _screenheight(height)
		{
			glewExperimental = GL_TRUE;
			glewInit();

			glEnable(GL_DEPTH_TEST);

			// Check that everything worked out okay
			if (!_window) {
				std::cout << "Unable to create window\n" << std::endl;;
				CheckSDLError(__LINE__);
				// throw here
			}
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
		}

		void SASWindow::TurnOnWireframe() { 
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		
		uptrSASWindow InitializeVideo(std::string cfg) {
			SetError(ErrorCode::NO_ERROR);
			// Load values from configfile ini
			INIReader reader(cfg);

			if (reader.ParseError() < 0) {
				std::cerr << "Could not load file " << cfg << std::endl;
				SetError(ErrorCode::FILE_NOT_FOUND);
				return nullptr;
			}

			auto windowtitle = reader.Get("video", "windowtitle", "default");
			auto screenwidth = reader.GetInteger("video", "screenwidth", 800);
			auto screenheight = reader.GetInteger("video", "screenheight", 600);

			// Start up SDL
			// Make sure it hasn't already been initialized.
			// You could hypothetically create multiple windows thru this 
			// function while only initializing SDL once.
			auto subsystem_init = SDL_WasInit(SDL_INIT_EVERYTHING);
			if (!(subsystem_init & SDL_INIT_VIDEO)) {
				if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
				{
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

			// Construct and return the window
			return std::make_unique<SASWindow>(windowtitle, screenwidth, screenheight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		}
	}
}