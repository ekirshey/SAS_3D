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
		/* 
			Generate UUID for system, not related to priority
			I'm saving uuid of the system and that's how the system should be accessed.
			No guarantee on the lifetime of the system pointer returned from the get function
			You should use it in as limited scope as possible.
			Does it make sense to do some weird thing where I return a special system pointer
			object with an overloaded accessor, which will update the pointer to the internal ECS
		*/
		_renderuuid = _ecs.AddSystem<RenderSystem>("RenderSystem", priority++, _config);
		_camera.RegisterObserver([&](const Camera& c) {
			auto rendersystem = static_cast<RenderSystem*>(_ecs.GetSystem(_renderuuid));
			if (rendersystem != nullptr) {
				rendersystem->UpdateViewTransform(c);
			}
		});

		_player = _ecs.CreateEntity();
		auto x = RigidBody(0.1f, glm::vec3(30, 0.0, -5.0));
		_ecs.AddComponentToEntity<PhysicalComponent>(_player, x.ModelMatrix());
		_ecs.AddComponentToEntity<AnimationComponent>(_player);
		_ecs.AddComponentToEntity<RenderComponent>(_player);
		

		return FSMStates::TRANSITIONIN;
	}

	FSMStates GameRunningState::UpdateState(int elapsedtime, SubsystemController* subsystems, const InputState& input) {
		bool sendevent = false;
		auto prevcam = _camera.GetViewMatrix();
		_camera.Update(input, elapsedtime/1000000.0f); //microseconds

		_ecs.Update(elapsedtime, subsystems);

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