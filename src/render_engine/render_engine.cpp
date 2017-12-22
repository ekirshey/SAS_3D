#include <iostream>
#include "render_engine.h"
#include "shaders/input_modules/mvp_module.h"
#include "shaders/input_modules/texture_module.h"
#include "shaders/input_modules/animation_module.h"

namespace SAS_3D {
	RenderEngine::RenderEngine(const GameConfig& config, SASWindow* window, RenderQueue* queue)
		: _config(config)
		, _window(window)
		, _shader(config.shaderpath + "animation.vert", config.shaderpath + "animation.frag")
		, _debugshader(config.shaderpath + "simple.vert", config.shaderpath + "simple.frag")
		, _mc(config.modelpath)
		, _event_queue(queue)
		, _running(true)
	{
		_shader.AddInputModule<MVPModule>();
		_shader.AddInputModule<TextureModule>();
		_shader.AddInputModule<AnimationModule>();

		_debugshader.AddInputModule<MVPModule>();

		LoadModels(_config.model_registry);

		Run();
	}


	void RenderEngine::LoadModels(std::string model_registry) {
		ModelLoader::LoadModels(model_registry, _mc);
	}

	RenderEngine::~RenderEngine() {

	}

	void RenderEngine::Run() {
		// Main Game Loop
		// DISABLES VSYNC
		if (SDL_GL_SetSwapInterval(0) != 0) {
			printf("Error: %s\n", SDL_GetError());
		}

		std::cout << "Entering main render loop" << std::endl;
		_window->SwapWindow();
		while (_running)
		{
			_window->Clear(0.2f, 0.3f, 0.3f);
			size_t size = 1;
			while (size != 0) {
				std::vector<RenderEvent> events;
				size = _event_queue->dequeue(events);
				if (size != 0) {
					for (auto& e : events) {
						_entities[e.id] = e;
					}
				}
			}

			for (auto &e : _entities){
				_shader.UseProgram();
				_mc.Draw(e.second.modelidx, e.second.model, e.second.view, e.second.projection, _shader);

				_debugshader.UseProgram();
				//_window->TurnOnWireframe();
				//_mc.DrawSkeleton(e.second.modelidx, e.second.model, e.second.view, e.second.projection, _debugshader);
			}

			_window->SwapWindow();

		}   // End Main Loop
	}

}