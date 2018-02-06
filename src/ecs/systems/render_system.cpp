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

	RenderSystem::RenderSystem(GameConfig config)
		: _config(config)
		, _scene(_config.videoconfig.width, _config.videoconfig.height)
	{
	}

	RenderSystem::~RenderSystem() {}

	// All this copying might be a bit too slow
	void RenderSystem::HandleCallback(Message& m) {
		// Create actual message types
		if (m.ID() == 0) {
			_scene.SetCamera(*m.GetData<CameraInfo>());
		}
	}

	void RenderSystem::Update(long long elapsedtime, const std::vector<EntityID>& entities, EntityManager* em, SubsystemController* subsystems) {
		// Update each entity
		for (const auto& entity : entities) {
			// For each entity build up a render event
			auto physical = em->GetEntityComponent<PhysicalComponent>(entity, PHYSICAL_COMPONENT);
			auto animation = em->GetEntityComponent<AnimationComponent>( entity, ANIMATION_COMPONENT);
			auto render = em->GetEntityComponent<RenderComponent>(entity, RENDER_COMPONENT);
			auto lighting = em->GetEntityComponent<LightComponent>(entity, LIGHT_COMPONENT);

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

		// Send final scene to the renderer
		auto renderer = subsystems->GetRenderEngine();
		renderer->RegisterScene(std::move(_scene));
	}
}
