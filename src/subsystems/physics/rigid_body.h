#pragma once
#include <glm/glm.hpp>

namespace SAS_3D {
	class RigidBody {
	public:
		RigidBody(float movementspeed, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f));
		void Move(float elapsedtime, glm::vec3 direction);
		glm::mat4 ModelMatrix();

	private:
		float _movementspeed;
		glm::vec3 _position;
		glm::vec3 _front;
		glm::vec3 _up;
		glm::vec3 _right;

	};
}