#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include "SDL.h"
#include "camera.h"

namespace SAS_3D {

	CameraInfo::CameraInfo() {

	}

	CameraInfo::CameraInfo(glm::vec3 pos, glm::vec3 front, float zoom)
		: m_position(pos)
		, m_front(front)
		, m_zoom(zoom)
	{

	}

	glm::mat4 CameraInfo::GetViewMatrix() const {
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}

	// Constructor with vectors
	Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
		: _info(position, glm::vec3(0.0f, 0.0f, -1.0f), ZOOM)
		, _yaw(yaw)
		, _pitch(pitch)
		, _worldup(up)
		, _movement_speed(SPEED)
		, _mouse_sensitivity(SENSITIVTY)
		, _lastx(0)
		, _lasty(0)
		, _firstmouse(true)
	{
		_update_camera_vectors();
	}

	// Constructor with scalar values
	Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: _info(glm::vec3(posX, posY, posZ),glm::vec3(0.0f, 0.0f, -1.0f), ZOOM)
		, _yaw(yaw)
		, _pitch(pitch)
		, _worldup(glm::vec3(upX, upY, upZ))
		, _movement_speed(SPEED)
		, _mouse_sensitivity(SENSITIVTY)
		, _lastx(0)
		, _lasty(0)
		, _firstmouse(true)
	{
		_update_camera_vectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 Camera::GetViewMatrix() const {
		return _info.GetViewMatrix();
	}

	void Camera::Update(const InputState& input, float deltatime) {
		_process_keyboard(input, deltatime);
		_process_mouse_movement(input);

		// Build a message containing revelant camera info to send to observers
		Message m(0, _info);
		SignalObservers(m);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void Camera::_process_keyboard(const InputState& input, float deltaTime) {
		float velocity = _movement_speed * deltaTime;
		if (input.keyarray[SDL_SCANCODE_W] == KeyState::PRESSED)
			_info.m_position += _info.m_front * velocity;
		if (input.keyarray[SDL_SCANCODE_S] == KeyState::PRESSED)
			_info.m_position -= _info.m_front * velocity;
		if (input.keyarray[SDL_SCANCODE_A] == KeyState::PRESSED)
			_info.m_position -= _info.m_right * velocity;
		if (input.keyarray[SDL_SCANCODE_D] == KeyState::PRESSED)
			_info.m_position += _info.m_right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void Camera::_process_mouse_movement(const InputState& input, GLboolean constrain_pitch)
	{
		if (_firstmouse) {
			_lastx = static_cast<float>(input.m_x);
			_lasty = static_cast<float>(input.m_y);
			_firstmouse = false;
		}
		auto xoffset = input.m_x - _lastx;
		auto yoffset = _lasty - input.m_y;
		_lastx = static_cast<float>(input.m_x);
		_lasty = static_cast<float>(input.m_y);

		xoffset *= _mouse_sensitivity;
		yoffset *= _mouse_sensitivity;

		_yaw += xoffset;
		_pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrain_pitch)
		{
			if (_pitch > 89.0f)
				_pitch = 89.0f;
			if (_pitch < -89.0f)
				_pitch = -89.0f;
		}

		// _update _front, _right and _up Vectors using the updated Eular angles
		_update_camera_vectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void Camera::_process_mouse_scroll(float yoffset)
	{
		if (_info.m_zoom >= 1.0f && _info.m_zoom <= 45.0f)
			_info.m_zoom -= yoffset;
		if (_info.m_zoom <= 1.0f)
			_info.m_zoom = 1.0f;
		if (_info.m_zoom >= 45.0f)
			_info.m_zoom = 45.0f;
	}

	// Calculates the front vector from the Camera's (updated) Eular Angles
	void Camera::_update_camera_vectors()
	{
		// Calculate the new _front vector
		glm::vec3 front;
		front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		front.y = sin(glm::radians(_pitch));
		front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		_info.m_front = glm::normalize(front);
		// Also re-calculate the Right and _up vector
		_info.m_right = glm::normalize(glm::cross(_info.m_front, _worldup));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		_info.m_up = glm::normalize(glm::cross(_info.m_right, _info.m_front));
	}
}