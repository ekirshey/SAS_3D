/*
* sas_input.h
*
* Defines functions and structures needed for keeping track of mouse and keyboard input.
* Input is currently handled by SDL
*
* Author: Erik Kirshey
* Date: 10/3/17
*/
#pragma once
#include <unordered_map>

namespace SAS_3D {
#define MAXKEYS 1024
	enum class KeyState { RELEASED, PRESSED };
	using KeyArray = KeyState[MAXKEYS];

	struct InputState {
		KeyArray keyarray;
		KeyState m_left;	// mouse left button
		KeyState m_right;	// mouse right button
		int m_x;			// mouse x position
		int m_y;			// mouse y position
		int m_wheel;		// mouse wheel position

		InputState()
			: keyarray{ KeyState::RELEASED }
			, m_left(KeyState::RELEASED)
			, m_right(KeyState::RELEASED)
			, m_x(0)
			, m_y(0)
			, m_wheel(0) {}
	};

	void UpdateInput(bool& quit, InputState& input);
}