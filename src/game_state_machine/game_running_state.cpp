#include <assimp/postprocess.h>     // Post processing flags
#include "game_state_machine/game_running_state.h"

namespace SAS_3D {
	namespace GSM {
		GameRunningState::GameRunningState(const GameConfig& config)
			: _camera()//config.screenwidth, config.screenheight)
			, _textureshader(config.shaderpath)
			, _mc(config.modelpath, config.texturepath)
		{

		}

		GameRunningState::~GameRunningState() {

		}

		FSMStates GameRunningState::InitializeState(Core::SASWindow* window, const Core::InputState& input) {
			_crowidx = _mc.LoadModelFromFile(
				"hagraven/hagraven_idle.fbx"
				, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (_crowidx == -1) {
				std::cerr << "Error Loading Model." << std::endl;
				return FSMStates::TRANSITIONOUT;
			}

			_mc.LoadModelIntoGPU(_crowidx);
			std::cout << "Done loading textures!" << std::endl;

			return FSMStates::TRANSITIONIN;
		}

		FSMStates GameRunningState::UpdateState(int elapsedtime, Core::SASWindow* window, const Core::InputState& input) {
			_camera.Update(input, elapsedtime/1000.0f);

			glm::mat4 model;
			// I seem to be looking down the x axis and im unsure why
			//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
			model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
			//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 view = _camera.GetViewMatrix();
			glm::mat4 projection = glm::perspective(_camera.Zoom, (float)window->GetScreenWidth() / (float)window->GetScreenHeight(), 0.1f, 100.0f);

			//window->TurnOnWireframe();
			_textureshader.ApplyMVP( model, view, projection);
			_mc.Draw(_crowidx, _textureshader);

			return FSMStates::UPDATE;
		}
	}
}