#pragma once
#include "ecs/framework/system.h"
#include "subsystems/render_engine/render_engine.h"
#include "ecs/ecs_defines.h"

namespace SAS_3D {
	class Camera;
	// High level system for interacting with the render engine
	class RenderSystem : public System
	{
	public:
		static const EntityID COMPONENTIDS = 0;// RENDER_COMPONENT | LIGHT_COMPONENT;// PositionComponentID | RenderComponentID;

		RenderSystem(std::string systemname, SystemID uuid, GameConfig config);
		~RenderSystem();

		EntityID ComponentBits() { return RenderSystem::COMPONENTIDS; }    // TODO: Is this necessary or is just accessing the variable directly better?

		// Not sure how I want the final camera->system interface to look, but this will due 
		// for now. I'm thinking some observer type model/signal<-> slot/what have you
		void UpdateViewTransform(const Camera& camera);
		
		void BeforeEntityProcessing(SubsystemController* subsystems);
		void ProcessEntity(SubsystemController* subsystems, EntityManager* em, EntityID entity);
		void AfterEntityProcessing(SubsystemController* subsystems);

	private:
		GameConfig _config;
		Scene _scene;
		glm::mat4 _projection;
		glm::vec3 _camerapos;
		glm::mat4 _viewmatrix;
		float _camerazoom;
	};
}