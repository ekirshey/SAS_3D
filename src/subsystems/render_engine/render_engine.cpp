#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "render_engine.h"
#include "shaders/input_modules/pvm_module.h"
#include "shaders/input_modules/texture_module.h"
#include "shaders/input_modules/animation_module.h"

namespace SAS_3D {
	//////// IMPL DETAILS
	RenderImpl::RenderImpl(const GameConfig& config, 
							   SASWindow* window,
							   RenderQueue* queue)
		: _config(config)
		, _window(window)	
		, _event_queue(queue)
		, _running(false)
	{
	}

	RenderImpl::~RenderImpl() {

	}

	void RenderImpl::Initialize(ModelContainer&& mc) {
		_window->SwitchContext();

		_mc = std::move(mc);
		_shader.Load(_config.shaderpath + "animation.vert", _config.shaderpath + "animation.frag");
		_debugshader.Load(_config.shaderpath + "simple.vert", _config.shaderpath + "simple.frag");

		_shader.AddInputModule<PVMModule>();
		_shader.AddInputModule<TextureModule>();
		//_shader.AddInputModule<AnimationModule>();

		_debugshader.AddInputModule<PVMModule>();
	}

	void RenderImpl::Run() {
		// Render loop 
		// DISABLES VSYNC
		if (SDL_GL_SetSwapInterval(0) != 0) {
			printf("Error: %s\n", SDL_GetError());
		}

		std::cout << "Entering main render loop" << std::endl;
		_running = true;
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
				_mc.Draw(e.second.modelidx, _shader, e.second.pvm);

				//_debugshader.UseProgram();
				//_window->TurnOnWireframe();
				//_mc.DrawSkeleton(e.second.modelidx, e.second.model, e.second.view, e.second.projection, _debugshader);
			}

			_window->SwapWindow();

		}   // End Main Loop
	}

	// Engine wrapper 
	RenderEngine::RenderEngine(const GameConfig& config, SASWindow* window)
		: _impl(config, window, &_eventqueue)
	{

	}

	RenderEngine::~RenderEngine() {
		_impl.Stop();
		_renderthread.join();
	}

	void RenderEngine::Start(ModelContainer&& mc) {
		SDL_GL_MakeCurrent(NULL, NULL);
		_renderthread = std::thread([&]() {
			ModelContainer tmc = std::move(mc);
			// Separate initialize due to needing to switch context first
			_impl.Initialize(std::move(tmc));
			_impl.Run();
		});
	}

	bool RenderEngine::isRunning() {
		return _impl.isRunning();
	}

	void RenderEngine::RegisterEvents(std::vector<RenderEvent>& events) {
		_eventqueue.enqueue(events);
	}
}