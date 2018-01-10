#pragma once
#include "ecs/framework/system.h"

namespace SAS_3D {
	class AnimationSystem : public System
	{
	public:
		static const uint_fast64_t COMPONENTIDS = 1;// PositionComponentID | RenderComponentID;

		AnimationSystem(std::string systemname);
		~AnimationSystem();

		uint_fast64_t ComponentBits() { return AnimationSystem::COMPONENTIDS; }    // TODO: Is this necessary or is just accessing the variable directly better?

		void ProcessEntity(uint_fast64_t entity);

		void BeforeObjectProcessing();
		void AfterObjectProcessing();
	private:
	};

}