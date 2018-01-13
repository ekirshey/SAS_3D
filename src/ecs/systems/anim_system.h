#pragma once
#include "ecs/framework/system.h"

namespace SAS_3D {
	class AnimationSystem : public System
	{
	public:
		static const EntityID COMPONENTIDS = 1;// PositionComponentID | RenderComponentID;

		AnimationSystem(std::string systemname);
		~AnimationSystem();

		EntityID ComponentBits() { return AnimationSystem::COMPONENTIDS; }    // TODO: Is this necessary or is just accessing the variable directly better?

		void BeforeEntityProcessing(SubsystemController* subsystems);
		void ProcessEntity(int elapsedtime, SubsystemController* subsystems, EntityManager* em, EntityID entity);
		void AfterEntityProcessing(SubsystemController* subsystems);

	private:
	};

}