#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <thread>
#include "game_state_machine/game_running_state.h"


#include "glm/ext.hpp"

namespace SAS_3D {
	GameRunningState::GameRunningState(const GameConfig& config)
		: _config(config)
		, _camera()//config.screenwidth, config.screenheight)
	{

	}

	GameRunningState::~GameRunningState() {

	}

	FSMStates GameRunningState::InitializeState(const InputState& input, RenderQueue* event_queue) {

		auto view = _camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(_camera.Zoom(), (float)_config.screenwidth / (float)_config.screenheight, 0.1f, 100.0f);

		std::vector<RenderEvent> events;
		for (int i = 0; i < 1; i++) {
			_mobs.push_back(RigidBody(0.1f, glm::vec3(i*30+0.0f, 0.0, -5.0)));
			RenderEvent ev;
			ev.id = i;
			ev.modelidx = 0;
			ev.model = _mobs.back().ModelMatrix();
			ev.view = view;
			ev.projection = projection;
			events.push_back(ev);
		}

		event_queue->enqueue(events);
		return FSMStates::TRANSITIONIN;
	}

	FSMStates GameRunningState::UpdateState(int elapsedtime, RenderQueue* event_queue, const InputState& input) {
		bool sendevent = false;
		auto prevcam = _camera.GetViewMatrix();
		_camera.Update(input, elapsedtime / 1000.0f);

		glm::mat4 view = _camera.GetViewMatrix();
		if (prevcam != view) {
			sendevent = true;
		}

		glm::mat4 projection = glm::perspective(_camera.Zoom(), (float)_config.screenwidth / (float)_config.screenheight, 0.1f, 100.0f);
/*
		glm::vec3 direction;
		if (input.keyarray[SDL_SCANCODE_LEFT] == KeyState::PRESSED)
			direction.x = 1.0f;
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
		
		if (direction != glm::vec3()) {
			sendevent = true;
		}

		_player.Move(elapsedtime, direction);
*/

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

		std::vector<RenderEvent> events;
		if (sendevent) {
			for (int i = 0; i < 1; i++) {
				RenderEvent ev;
				ev.id = i;
				ev.modelidx = 0;
				ev.model = _mobs[i].ModelMatrix();
				ev.view = view;
				ev.projection = projection;
				events.push_back(ev);
			}

			event_queue->enqueue(events);
		}
		
		return FSMStates::UPDATE;
	}
}