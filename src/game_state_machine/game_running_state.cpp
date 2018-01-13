#include <thread>
#include "game_state_machine/game_running_state.h"
#include "ecs/systems/anim_system.h"
#include "ecs/systems/render_system.h"

// components
#include "ecs/components/physical_component.h"
#include "ecs/components/anim_component.h"
#include "ecs/components/render_component.h"

namespace SAS_3D {
	GameRunningState::GameRunningState(const GameConfig& config)
		: _config(config)
		, _camera()//config.screenwidth, config.screenheight)
	{

	}

	GameRunningState::~GameRunningState() {

	}

	FSMStates GameRunningState::InitializeState(SubsystemController* subsystems, const InputState& input) {
		int priority = 0;
		_ecs.AddSystem<AnimationSystem>("AnimationSystem",priority++);
		_rendersystem = _ecs.AddSystem<RenderSystem>("RenderSystem", priority++, _config);

		_player = _ecs.CreateEntity();
		_ecs.AddComponentToEntity<PhysicalComponent>(_player);
		_ecs.AddComponentToEntity<AnimationComponent>(_player);
		_ecs.AddComponentToEntity<RenderComponent>(_player);

		return FSMStates::TRANSITIONIN;
	}

	FSMStates GameRunningState::UpdateState(int elapsedtime, SubsystemController* subsystems, const InputState& input) {
		bool sendevent = false;
		auto prevcam = _camera.GetViewMatrix();
		_camera.Update(input, elapsedtime / 1000.0f);

		// HIDEOUS
		auto rendersystem = static_cast<RenderSystem*>(_ecs.GetSystem(_rendersystem));
		rendersystem->UpdateViewTransform(_camera);

		_ecs.Update(elapsedtime, subsystems);
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

		return FSMStates::UPDATE;
	}
}