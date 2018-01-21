#include <glm/gtc/matrix_transform.hpp>
#include "render_system.h"
#include "subsystems/subsystem_controller.h"
#include "ecs/framework/entity_manager.h"
#include "entities/camera.h"

// Relevant Component includes
#include "ecs/components/physical_component.h"
#include "ecs/components/anim_component.h"
#include "ecs/components/render_component.h"

namespace SAS_3D {
	RenderSystem::RenderSystem(std::string systemname, SystemID uuid, GameConfig config) 
		: System(systemname, uuid)
		, _config(config)
	{
	}

	RenderSystem::~RenderSystem() {}

	void RenderSystem::UpdateViewTransform(const Camera& camera) {
		_view = camera.GetViewMatrix();
		_zoom = camera.Zoom();
	}

	void RenderSystem::BeforeEntityProcessing(int elapsedtime, SubsystemController* subsystems) {
		_projection = glm::perspective(_zoom, (float)_config.screenwidth / (float)_config.screenheight, 0.1f, 1000.0f);
	}

	void RenderSystem::ProcessEntity(SubsystemController* subsystems, EntityManager* em, EntityID entity) {
		// For each entity build up a render event
		auto physical = GetEntityComponent<PhysicalComponent*>(em, entity, PHYSICAL_COMPONENT);
		auto animation = GetEntityComponent<AnimationComponent*>(em, entity, ANIMATION_COMPONENT);
		auto render = GetEntityComponent<RenderComponent*>(em, entity, RENDER_COMPONENT);

		RenderEvent re;
		re.id = entity;
		re.pv = _projection * _view;
		re.m = physical->modeltransform;
		re.modelidx = render->modelidx;
		if (animation != nullptr) {
			re.bones = std::move(animation->animationstate.bones);
		}
		
		_renderevents.push_back(std::move(re));
	}

	void RenderSystem::AfterEntityProcessing(SubsystemController* subsystems) {
		auto renderer = subsystems->GetRenderEngine();
		renderer->RegisterEvents(std::move(_renderevents));
	}
}
