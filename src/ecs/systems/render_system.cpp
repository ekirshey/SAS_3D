#include "render_system.h"
#include "subsystems/subsystem_controller.h"
#include "subsystems/render_engine/scene.h"
#include "ecs/framework/entity_manager.h"
#include "entities/camera.h"

// Relevant Component includes
#include "ecs/components/physical_component.h"
#include "ecs/components/anim_component.h"
#include "ecs/components/render_component.h"
#include "ecs/components/light_component.h"

// TEMP INCLUDE
#include <glm/gtc/matrix_transform.hpp>

namespace SAS_3D {
	RenderSystem::RenderSystem(std::string systemname, SystemID uuid, GameConfig config) 
		: System(systemname, uuid)
		, _config(config)
		, _scene(_config.screenwidth, _config.screenheight)
	{
	}

	RenderSystem::~RenderSystem() {}

	void RenderSystem::UpdateViewTransform(const Camera& camera) {
		_scene.SetCamera(camera);
	}

	void RenderSystem::BeforeEntityProcessing(SubsystemController* subsystems) {
	}

	void RenderSystem::ProcessEntity(SubsystemController* subsystems, EntityManager* em, EntityID entity) {
		// For each entity build up a render event
		auto physical = GetEntityComponent<PhysicalComponent>(em, entity, PHYSICAL_COMPONENT);
		auto animation = GetEntityComponent<AnimationComponent>(em, entity, ANIMATION_COMPONENT);
		auto render = GetEntityComponent<RenderComponent>(em, entity, RENDER_COMPONENT);
		auto lighting = GetEntityComponent<LightComponent>(em, entity, LIGHT_COMPONENT);

		if (render != nullptr) {
			RenderItem re;
			re.m_id = entity;
			re.m_model = physical->modeltransform;
			re.m_modelidx = render->modelidx;
			if (animation != nullptr) {
				re.m_bones = std::move(animation->animationstate.bones);
			}
			_scene.AddRenderItem(std::move(re));
		}

		if (lighting != nullptr) {	
			_scene.AddLight(lighting->m_light);
		}

	}

	void RenderSystem::AfterEntityProcessing(SubsystemController* subsystems) {
		auto renderer = subsystems->GetRenderEngine();
		renderer->RegisterScene(std::move(_scene));
	}
}
