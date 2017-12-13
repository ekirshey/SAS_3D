#include <iostream>
#include "render_engine.h"
#include "shaders/input_modules/mvp_module.h"
#include "shaders/input_modules/texture_module.h"

namespace SAS_3D {
	RenderEngine::RenderEngine(const GameConfig& config, SASWindow* window, RenderQueue* queue)
		: _config(config)
		, _window(window)
		, _shader(config.shaderpath + "texture.vert", config.shaderpath + "texture.frag")
		, _mc(config.modelpath)
		, _event_queue(queue)
		, _running(true)
	{
		_shader.AddInputModule<MVPModule>();
		_shader.AddInputModule<TextureModule>();

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

			_shader.UseProgram();
			for (auto &e : _entities){
				auto mvpmodule = _shader.GetInputModule<MVPModule*>(MVPModule::ID);
				mvpmodule->SetMVP(e.second.mvp);
				_mc.Draw(e.second.modelidx, _shader);
			}

			_window->SwapWindow();

		}   // End Main Loop
	}

}