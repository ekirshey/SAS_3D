#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "core/sas_io.h"
#include "ecs/framework/subject.h"

namespace SAS_3D {
	// Default camera values
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 15.5f;
	const float SENSITIVTY = 0.6f;
	const float ZOOM = 45.0f;

	// Subset of information given to listeners and the scene
	struct CameraInfo {
		CameraInfo();
		CameraInfo(glm::vec3 pos, glm::vec3 front, float zoom);
		glm::mat4 GetViewMatrix() const;

		glm::vec3	m_position;
		glm::vec3	m_up;
		glm::vec3	m_front;
		glm::vec3	m_right;
		float		m_zoom;

	};

	// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
	class Camera : public Subject
	{
	public:
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

		glm::mat4 GetViewMatrix() const;
		glm::vec3 GetPosition() const { return _info.m_position; }
		glm::vec3 GetFront() const { return _info.m_front; }
		float Zoom() const { return _info.m_zoom; }
		void Update(const InputState& input, float deltatime);

	private:
		void _process_keyboard(const InputState& input, float deltaTime);
		void _process_mouse_movement(const InputState& input, GLboolean constrainPitch = true);
		void _process_mouse_scroll(float yoffset);
		void _update_camera_vectors();

		// Camera Attributes
		CameraInfo _info;	// Specific info separated out for easily passing around

		// Eular Angles
		float _yaw;
		float _pitch;
		glm::vec3 _worldup;

		// Camera options
		float _movement_speed;
		float _mouse_sensitivity;
		float _lastx;
		float _lasty;
		bool _firstmouse;
	};
}