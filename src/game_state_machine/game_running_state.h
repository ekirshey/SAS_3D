#pragma once
#include "game_state_machine/game_state.h"
#include "assets/model_loader.h"
#include "assets/model_container.h"
#include "shaders/base_shader.h"
#include "entities/camera.h"

namespace SAS_3D {
	namespace GSM {
		class GameRunningState : public GameStateImpl
		{
			public:
				GameRunningState(int screenwidth, int screenheight);
				~GameRunningState();

				FSMStates InitializeState(Core::SASWindow* window, const Core::InputState& input);
				FSMStates UpdateState(int elapsedtime, Core::SASWindow* window, const Core::InputState& input);
				int NextState() { return _nextstate; }

			private:
				Entities::Camera _camera;
				Shaders::BaseShader _baseshader;
				Assets::ModelContainer _mc;
				Assets::ModelIdx _crowidx;
				int _nextstate;
				bool _exit;
		};
	}
}
