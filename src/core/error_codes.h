/*
* Defined error exceptions for the game engine
*
*/
#pragma once
#include <stdexcept>
#include <string>

namespace SAS_3D {

	class BadShaderException : public std::runtime_error {
	public:
		explicit BadShaderException(const std::string& message)
			: std::runtime_error(message)
		{}
	};

	enum class ErrorCode {
		NO_ERROR = 0,
		FILE_NOT_FOUND = 1,
		FAILED_TO_OPEN_FILE = 2,
		SDL_ERROR = 3,
		BAD_MODEL_FILE = 4,
		MODEL_CONTAINER_FILLED = 5
	};

	void SetError(ErrorCode ec);
	ErrorCode GetError();

}