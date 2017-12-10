#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include "game_state_machine/game_running_state.h"

namespace SAS_3D {
	GameRunningState::GameRunningState(const GameConfig& config)
		: _camera()//config.screenwidth, config.screenheight)
		, _player(0.1f)
	{

	}

	GameRunningState::~GameRunningState() {

	}

	FSMStates GameRunningState::InitializeState(SASWindow* window, const InputState& input) {
/*
		_shader.AddInputModule<MVPModule>();
		_shader.AddInputModule<TextureModule>();

		_crowidx = _mc.LoadModelFromFile(
			"hagraven/hagraven_idle.fbx"
			, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (_crowidx == -1) {
			std::cerr << "Error Loading Model." << std::endl;
			return FSMStates::TRANSITIONOUT;
		}

		_mc.LoadModelIntoGPU(_crowidx);
		std::cout << "Done loading textures!" << std::endl;
		*/
		return FSMStates::TRANSITIONIN;
	}

	FSMStates GameRunningState::UpdateState(int elapsedtime, SASWindow* window, const InputState& input) {
		_camera.Update(input, elapsedtime / 1000.0f);

		glm::mat4 view = _camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(_camera.Zoom(), (float)_config.screenwidth / (float)_config.screenheight, 0.1f, 100.0f);

		glm::vec3 direction;
		if (input.keyarray[SDL_SCANCODE_LEFT] == KeyState::PRESSED) {
			std::cout << "Test" << std::endl;
			direction.x = 1.0f;
		}
		if (input.keyarray[SDL_SCANCODE_RIGHT] == KeyState::PRESSED)
			direction.x = -1.0f;
		if (input.keyarray[SDL_SCANCODE_DOWN] == KeyState::PRESSED)
			direction.z = 1.0f;
		if (input.keyarray[SDL_SCANCODE_UP] == KeyState::PRESSED)
			direction.z = -1.0f;
		if (input.keyarray[SDL_SCANCODE_8] == KeyState::PRESSED)
			direction.y = 1.0f;
		if (input.keyarray[SDL_SCANCODE_2] == KeyState::PRESSED)
			direction.y = -1.0f;

		_player.Move(elapsedtime, direction);

		/*
		In parallel the network thread is receiving messages, processing them and
		throwing them on the appropriate queue

		Handle player input
			check for network events for correction
			update based on movement
		Update camera
			Update based on player OR input
		Update ecs
			update monsters/npcs from network events
				separate queues for movement
			check for collisions
				Only player->terrain collisions for this game but if I place it here it allows for easier expansion and different games
			Send network events
				send players new position after client
			Send render events to render thread
		*/

		//window->TurnOnWireframe();
		//_shader.UseProgram();
		//auto mvpmodule = _shader.GetInputModule<MVPModule*>(MVPModule::ID);
		//mvpmodule->SetMVP(projection*view*_player.ModelMatrix());
		//_mc.Draw(_crowidx, _shader);

		return FSMStates::UPDATE;
	}
}