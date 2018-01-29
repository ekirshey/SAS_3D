#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "render_engine.h"
#include "shaders/input_modules/pvm_module.h"
#include "shaders/input_modules/texture_module.h"
#include "shaders/input_modules/animation_module.h"
#include "shaders/input_modules/light_module.h"
#include "core/sas_video.h"

namespace SAS_3D {
	//////// IMPL DETAILS
	RenderImpl::RenderImpl(const GameConfig& config,
		SASWindow* window,
		RenderQueue* queue)
		: _config(config)
		, _window(window)
		, _event_queue(queue)
		, _running(false)
		, _shaders(3)
	{
	}

	RenderImpl::~RenderImpl() {

	}

	void RenderImpl::Initialize(ModelContainer&& mc) {
		_window->SwitchContext();

		_mc = std::move(mc);
		_shaders[0].Load(_config.shaderpath + "animation.vert", _config.shaderpath + "lighting.frag");
		_shaders[0].AddInputModule<PVMModule>();
		_shaders[0].AddInputModule<MaterialModule>();
		_shaders[0].AddInputModule<AnimationModule>();
		_shaders[0].AddInputModule<LightModule>();

		_shaders[1].Load(_config.shaderpath + "model.vert", _config.shaderpath + "lighting.frag");
		_shaders[1].AddInputModule<PVMModule>();
		_shaders[1].AddInputModule<MaterialModule>();
		_shaders[1].AddInputModule<LightModule>();

		_shaders[2].Load(_config.shaderpath + "simple.vert", _config.shaderpath + "simple.frag");
		_shaders[2].AddInputModule<PVMModule>();
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
			// Do we add a light component to the render event or do we handle the light sources in a separate queue?
			_window->Clear(0.2f, 0.3f, 0.3f);
			size_t size = 1;
			while (size != 0) {
				Scene newscene;
				size = _event_queue->dequeue(newscene);
				if (size != 0) {
					// Manage the scene index
					_scenes[0] = std::move(newscene);
				}
			}

			for (auto &s : _scenes) {
				auto cam = &s.m_camera;
				_projectionmatrix = glm::perspective(cam->m_zoom, (float)_window->GetScreenWidth() / (float)_window->GetScreenHeight(), 0.1f, 1000.0f);
				glm::mat4 pv = _projectionmatrix * cam->m_viewmatrix;
				for (auto &e : s.m_objects) {
					// Need to group draws by shader type
					if (e.m_bones.size() > 0) {
						_shaders[0].UseProgram();
						auto lightmodule = _shaders[0].GetInputModule<LightModule*>(LightModuleID);
						lightmodule->SetLightSettings(cam->m_position, &s.m_dirlights, &s.m_pointlights, &s.m_spotlights);
						_mc.Draw(e.m_modelidx, _shaders[0], pv, e.m_model, &e.m_bones);
					}
					else if (_mc.HasTextures(e.m_modelidx)) {
						_shaders[1].UseProgram();
						auto lightmodule = _shaders[1].GetInputModule<LightModule*>(LightModuleID);
						lightmodule->SetLightSettings(cam->m_position, &s.m_dirlights, &s.m_pointlights, &s.m_spotlights);
						_mc.Draw(e.m_modelidx, _shaders[1], pv, e.m_model);
					}
					else {
						_shaders[2].UseProgram();
						_mc.Draw(e.m_modelidx, _shaders[2], pv, e.m_model);
					}

					//_debugshader.UseProgram();
					//_window->TurnOnWireframe();
					//_mc.DrawSkeleton(e.second.modelidx, e.second.model, e.second.view, e.second.projection, _debugshader);
				}
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

	void RenderEngine::RegisterScene(Scene& scene) {
		_eventqueue.enqueue(scene);
	}
}