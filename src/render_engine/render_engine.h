#pragma once
#include <thread>
#include "assets/model_container.h"
#include "render_engine/shaders/shader_program.h"
#include "render_engine/scene_graph.h"
#include "game_state_machine/game_config.h"
#include "utility/locking_queue.h"

namespace SAS_3D {
	class RenderEngine {
	public:
		RenderEngine(const GameConfig& config, LockingQueue<SceneGraph>& queue);
		~RenderEngine();
		void Run();
		void Stop() { _running = false; }
	private:
		GameConfig _config;
		ShaderProgram _shader;
		ModelContainer _mc;
		int _crowidx;
		LockingQueue<SceneGraph>& _scene_queue;
		bool _running;
	};
}