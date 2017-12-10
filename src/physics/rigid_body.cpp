#include <glm/gtc/matrix_transform.hpp>
#include "rigid_body.h"

namespace SAS_3D {
	RigidBody::RigidBody(float movementspeed, glm::vec3 position, glm::vec3 up, glm::vec3 front)
		: _movementspeed(movementspeed)
		, _position(position)
		, _up(up)
		, _front(front)
		, _right(glm::cross(_front, _up))
	{

	}

	void RigidBody::Move(float elapsedtime, glm::vec3 direction) {
		float velocity = elapsedtime * _movementspeed;
		_position += direction.z * _front * velocity;
		_position += direction.x * _right * velocity;
		_position += direction.y * _up * velocity;
	}

	glm::mat4 RigidBody::ModelMatrix() {
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::translate(model, _position);
		// TEMP
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		return model;
	}
}