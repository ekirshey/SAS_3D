#pragma once
#include <thread>
#include "assets/model_container.h"
#include "subsystems/render_engine/shaders/shader_program.h"
#include "subsystems/render_engine/scene_graph.h"
#include "game_state_machine/game_config.h"
#include "utility/locking_queue.h"
#include "core/sas_video.h"

namespace SAS_3D {
	using RenderID = unsigned long long;
	struct RenderEvent {
		RenderID id;
		glm::mat4 pvm;
		int modelidx;
	};
	using RenderQueue = LockingQueue<std::vector<RenderEvent>>;

	class RenderImpl {
	public:
		RenderImpl(const GameConfig& config, 
					 SASWindow* window, 
					 RenderQueue* queue);

		~RenderImpl();
		// Called after opengl context switches to the thread renderengine runs on
		void Initialize(ModelContainer&& mc);
		void Run();
		void Stop() { _running = false; }
		bool isRunning() { return _running; }
	private:
		GameConfig _config;
		SASWindow* _window;
		ShaderProgram _shader;
		ShaderProgram _debugshader;
		ModelContainer _mc;
		// Need to figure out the best way to replace this unordered_map
		std::unordered_map<RenderID, RenderEvent> _entities;
		RenderQueue* _event_queue;
		bool _running;
	};

	class RenderEngine {
	public:
		RenderEngine(const GameConfig& config, SASWindow* window);
		~RenderEngine();
		void Start(ModelContainer&& mc);
		bool isRunning();
		void RegisterEvents(std::vector<RenderEvent>& events);
	private:
		RenderImpl _impl;
		RenderQueue _eventqueue;
		std::thread _renderthread;
	};
}