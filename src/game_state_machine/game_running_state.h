#pragma once
#include "game_state_machine/game_state.h"
#include "game_state_machine/game_config.h"
#include "assets/model_container.h"
#include "shaders/texture_shader.h"
#include "entities/camera.h"

namespace SAS_3D {
	namespace GSM {
		class GameRunningState : public GameStateImpl
		{
			public:
				GameRunningState(const GameConfig& config);
				~GameRunningState();

				FSMStates InitializeState(Core::SASWindow* window, const Core::InputState& input);
				FSMStates UpdateState(int elapsedtime, Core::SASWindow* window, const Core::InputState& input);
				int NextState() { return _nextstate; }

			private:
				GameConfig _config;
				Entities::Camera _camera;
				Shaders::TextureShader _textureshader;
				Assets::ModelContainer _mc;
				Assets::ModelIdx _crowidx;
				int _nextstate;
				bool _exit;
		};
	}
}
