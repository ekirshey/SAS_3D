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
	{
	}

	RenderSystem::~RenderSystem() {}

	void RenderSystem::UpdateViewTransform(const Camera& camera) {
		auto cam = &_scene.m_camera;
		cam->m_position = camera.GetPosition();
		cam->m_viewmatrix = camera.GetViewMatrix();
		cam->m_zoom = camera.Zoom();
	}

	void RenderSystem::BeforeEntityProcessing(int elapsedtime, SubsystemController* subsystems) {
		_elapsedtime = elapsedtime;
	}

	void RenderSystem::ProcessEntity(SubsystemController* subsystems, EntityManager* em, EntityID entity) {
		// For each entity build up a render event
		auto physical = GetEntityComponent<PhysicalComponent*>(em, entity, PHYSICAL_COMPONENT);
		auto animation = GetEntityComponent<AnimationComponent*>(em, entity, ANIMATION_COMPONENT);
		auto render = GetEntityComponent<RenderComponent*>(em, entity, RENDER_COMPONENT);
		auto lighting = GetEntityComponent<LightComponent*>(em, entity, LIGHT_COMPONENT);

		if (render != nullptr) {
			if (lighting != nullptr) {
				float forward = _elapsedtime / 1000000 * 3.0f;
				physical->modeltransform = glm::translate(physical->modeltransform, glm::vec3(0.0, 0.0, forward));
			}
			RenderItem re;
			re.m_id = entity;
			re.m_model = physical->modeltransform;
			re.m_modelidx = render->modelidx;
			if (animation != nullptr) {
				re.m_bones = std::move(animation->animationstate.bones);
			}
			_scene.m_objects.push_back(std::move(re));
		}

		if (lighting != nullptr) {	
			// Testing lighting
			if (physical != nullptr) {
				lighting->m_light.m_position = physical->modeltransform * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			}
			_scene.AddLight(lighting->m_light);
		}


	}

	void RenderSystem::AfterEntityProcessing(SubsystemController* subsystems) {
		auto renderer = subsystems->GetRenderEngine();
		renderer->RegisterScene(std::move(_scene));
	}
}
