#include <thread>
#include "game_state_machine/game_running_state.h"
#include "ecs/systems/anim_system.h"
#include "ecs/systems/render_system.h"

// components
#include "ecs/components/physical_component.h"
#include "ecs/components/anim_component.h"
#include "ecs/components/render_component.h"
#include "ecs/components/light_component.h"

namespace SAS_3D {
	GameRunningState::GameRunningState(const GameConfig& config)
		: _config(config)
		, _camera(glm::vec3(-40.0,100.0,0.0))//config.screenwidth, config.screenheight)
	{

	}

	GameRunningState::~GameRunningState() {

	}

	FSMStates GameRunningState::InitializeState(SubsystemController* subsystems, const InputState& input) {
		int priority = 0;

		_ecs.AddSystem<AnimationSystem>("AnimationSystem", ANIMATION_COMPONENT);
		/* 
			Internally a UUID is generated for the system and returned. That can 
			be used to access a pointer to the system.
		*/
		_renderuuid = _ecs.AddSystem<RenderSystem>("RenderSystem", RENDER_COMPONENT | LIGHT_COMPONENT, _config);

		/*
			_ecs.AddCallback(&camera, systemname, )
		*/
		_camera.RegisterObserver([&](Message& m) {
			_ecs.CallSystemCallback(_renderuuid, m);
		});

		_player = _ecs.CreateEntity();
		auto x = RigidBody(0.1f, glm::vec3(0, 55.0, 0.0));
		_ecs.AddComponentToEntity<PhysicalComponent>(_player, x.ModelMatrix());
		_ecs.AddComponentToEntity<AnimationComponent>(_player);
		_ecs.AddComponentToEntity<RenderComponent>(_player,0);		

		auto landscape = _ecs.CreateEntity();
		auto l = RigidBody(0.1f, glm::vec3(0, 0.0, 0.0));
		_ecs.AddComponentToEntity<PhysicalComponent>(landscape, l.ModelMatrix());
		_ecs.AddComponentToEntity<RenderComponent>(landscape, 1);

		// Directional light with no physical representation
		auto sun = _ecs.CreateEntity();
		Light dirlight;
		dirlight.m_type = LightType::Directional;
		dirlight.m_direction = glm::vec3(0.0f, -1.0f, 0.0f);
		dirlight.m_ambient = glm::vec3(0.05f, 0.05f, 0.05f);
		dirlight.m_diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
		dirlight.m_specular = glm::vec3(0.5f, 0.5f, 0.5f);
		_ecs.AddComponentToEntity<LightComponent>(sun, dirlight);

		// Point lights
		for (int i = 0; i < 2; i++) {
			auto light = _ecs.CreateEntity();
			auto lightbody = RigidBody(0.1f, glm::vec3(0.0, 100.0 - 40*i, 20.0 + -75*i));
			_ecs.AddComponentToEntity<PhysicalComponent>(light, lightbody.ModelMatrix());
			_ecs.AddComponentToEntity<RenderComponent>(light, 2);
			Light pointlight;
			pointlight.m_type = LightType::Point;
			glm::vec4 lightpos = lightbody.ModelMatrix() * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			pointlight.m_position = glm::vec3(lightpos.x, lightpos.y, lightpos.z);
			pointlight.m_ambient = glm::vec3(0.05f, 0.05f, 0.05f);
			pointlight.m_diffuse = glm::vec3(15.0f, 15.0f, 15.0f);
			pointlight.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
			pointlight.m_constant = 1.0f;
			pointlight.m_linear = 0.09f;
			pointlight.m_quadratic = 0.032f;
			_ecs.AddComponentToEntity<LightComponent>(light, pointlight);
		}

		// Spotlight
		_flashlight = _ecs.CreateEntity();
		auto lightbody = RigidBody(0.1f, glm::vec3(0.0, 100.0, -50.0));
		Light spotlight;
		spotlight.m_type = LightType::Spotlight;
		spotlight.m_ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		spotlight.m_diffuse = glm::vec3(10.0f, 10.0f, 10.0f);
		spotlight.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
		spotlight.m_cutoff = static_cast<float>(glm::cos(glm::radians(12.5)));
		spotlight.m_outercutoff = glm::cos(glm::radians(15.0f));
		spotlight.m_constant = 1.0f;
		spotlight.m_linear = 0.009f;
		spotlight.m_quadratic = 0.0032f;
		_ecs.AddComponentToEntity<LightComponent>(_flashlight, spotlight);
		_lighttoggle = false;

		return FSMStates::TRANSITIONIN;
	}

	FSMStates GameRunningState::UpdateState(long long elapsedtime, SubsystemController* subsystems, const InputState& input) {
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