#pragma once
#include "subsystems/render_engine/render_engine.h"
#include "ecs/ecs_defines.h"
#include "ecs/framework/message.h"

namespace SAS_3D {
	class EntityManager;
	class SubsystemController;

	class Camera;
	// High level system for interacting with the render engine
	class RenderSystem
	{
	public:
		RenderSystem(GameConfig config);
		~RenderSystem();
		
		void Update(long long elapsedtime, const std::vector<EntityID>& entities, EntityManager* em, SubsystemController* subsystems);

		void HandleCallback(Message& m);

	private:
		GameConfig _config;
		Scene _scene;
		glm::mat4 _projection;
		glm::vec3 _camerapos;
		glm::mat4 _viewmatrix;
		float _camerazoom;
	};
}