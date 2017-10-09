#include "game_state_machine/game_running_state.h"

namespace SAS_3D {
	namespace GSM {
		GameRunningState::GameRunningState(int screenwidth, int screenheight) 
			: _camera(screenwidth, screenheight)
		{

		}

		GameRunningState::~GameRunningState() {

		}

		FSMStates GameRunningState::InitializeState(Core::SASWindow* window, const Core::InputState& input) {
			_crowidx = SAS_3D::Assets::ModelLoader::LoadModelFromFile(
				_mc
				, "F:/game_assets/totalwar_warhammerII/models2_exports/crow.fbx"
				, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (_crowidx == -1) {
				std::cerr << "Error Loading Model." << std::endl;
				return FSMStates::TRANSITIONOUT;
			}

			_mc.LoadModelIntoGPU(_crowidx);

			return FSMStates::TRANSITIONIN;
		}

		FSMStates GameRunningState::UpdateState(int elapsedtime, Core::SASWindow* window, const Core::InputState& input) {
			_camera.Update(input);

			glm::mat4 model;
			glm::mat4 view = _camera.GetViewMatrix();
			glm::mat4 projection = glm::perspective(_camera.Zoom(), (float)window->GetScreenWidth() / (float)window->GetScreenHeight(), 0.1f, 100.0f);

			window->TurnOnWireframe();
			SAS_3D::Shaders::ApplyShader(_baseshader, model, view, projection);
			_mc.Draw(_crowidx);

			return FSMStates::UPDATE;
		}
	}
}