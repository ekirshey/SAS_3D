#include <iostream>
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
		, _shaders(4)
		, _scenes(MAX_SCENES, Scene(_config.screenwidth, _config.screenheight))
	{

	}

	RenderImpl::~RenderImpl() {

	}

	void RenderImpl::Initialize(ModelContainer&& mc, TextureContainer&& tc) {
		_window->SwitchContext();

		_mc = std::move(mc);
		_tc = std::move(tc);

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

		_shaders[3].Load("F:/github/opengl_experiments/shaders/skybox.vs", "F:/github/opengl_experiments/shaders/skybox.fs");

		_skybox.Load("F:/github/SAS_3D/media/skybox/", tc);
	}

	void RenderImpl::Run() {
		// Render loop 
		if (!_config.vsync) {
			_window->DisableVSYNC();
		}

		std::cout << "Entering main render loop" << std::endl;
		_running = true;
		_window->SwapWindow();

		while (_running)
		{
			// Do we add a light component to the render event or do we handle the light sources in a separate queue?
			_window->Clear(0.1f, 0.1f, 0.1f);
			size_t size = 1;

			while (size != 0) {
				Scene newscene(_config.screenwidth, _config.screenheight);
				size = _event_queue->dequeue(newscene);
				if (size != 0) {
					// Manage the scene index
					_scenes[0] = std::move(newscene);
				}
			}

			for (const auto &s : _scenes) {
				s.DrawScene(_shaders, _mc, _skybox);
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

	void RenderEngine::Start(ModelContainer&& mc, TextureContainer&& tc) {
		SDL_GL_MakeCurrent(NULL, NULL);
		_renderthread = std::thread([&]() {
			// Separate initialize due to needing to switch context first
			_impl.Initialize(std::move(mc), std::move(tc));
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