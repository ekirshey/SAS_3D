#pragma once
#include <thread>
#include "assets/model_container.h"
#include "subsystems/render_engine/shaders/shader_program.h"
#include "subsystems/render_engine/scene.h"
#include "game_state_machine/game_config.h"
#include "utility/locking_queue.h"
#include "assets/cubemap.h"

namespace SAS_3D {
	class SASWindow;

	using RenderQueue = LockingQueue<Scene>;
	#define MAX_SCENES 1
	class RenderImpl {
	public:
		RenderImpl(const GameConfig& config, 
					 SASWindow* window, 
					 RenderQueue* queue);

		~RenderImpl();
		// Called after opengl context switches to the thread renderengine runs on
		void Initialize(ModelContainer&& mc, TextureContainer&& tc);
		void Run();
		void Stop() { _running = false; }
		bool isRunning() { return _running; }
	private:
		GameConfig _config;
		SASWindow* _window;
		RenderQueue* _event_queue;
		bool _running;
		std::vector<ShaderProgram> _shaders;
		std::vector<ShaderProgram> _deferredshaders;

		// Camera information used in calculated ProjectionView and lights
		glm::mat4 _projectionmatrix;

		// Active scene being rendered
		Scene _scene;

		CubeMap _skybox;
		ModelContainer _mc;
		TextureContainer _tc;
	};

	class RenderEngine {
	public:
		RenderEngine(const GameConfig& config, SASWindow* window);
		~RenderEngine();
		void Start(ModelContainer&& mc, TextureContainer&& tc);
		bool isRunning();
		void RegisterScene(Scene& scene);
	private:
		RenderImpl _impl;
		RenderQueue _eventqueue;
		std::thread _renderthread;
	};
}