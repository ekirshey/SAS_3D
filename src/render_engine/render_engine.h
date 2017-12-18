#pragma once
#include <thread>
#include "assets/model_container.h"
#include "render_engine/shaders/shader_program.h"
#include "render_engine/scene_graph.h"
#include "game_state_machine/game_config.h"
#include "utility/locking_queue.h"
#include "core/sas_video.h"

namespace SAS_3D {
	struct RenderEvent {
		unsigned long long id;
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		int modelidx;
	};
	using RenderQueue = LockingQueue<std::vector<RenderEvent>>;

	class RenderEngine {
	public:
		RenderEngine(const GameConfig& config, SASWindow* window, RenderQueue* queue);
		~RenderEngine();
		void Run();
		void Stop() { _running = false; }
		void LoadModels(std::string model_registry);
	private:
		GameConfig _config;
		SASWindow* _window;
		ShaderProgram _shader;
		ShaderProgram _debugshader;
		ModelContainer _mc;
		std::unordered_map<unsigned long long, RenderEvent> _entities;
		RenderQueue* _event_queue;
		bool _running;
	};
}