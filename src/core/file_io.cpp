#include <sstream>
#include <iostream>
#include <fstream>
#include "core/file_io.h"

namespace SAS_3D {
	namespace Core {
		std::string ReadFile(std::string file) {
			SetError(ErrorCode::NO_ERROR);

			// Open file
			std::ifstream ifstrm(file);
			if (!ifstrm.is_open()) {
				SetError(ErrorCode::FAILED_TO_OPEN_FILE);
				return "";
			}

			// Read file into buffer
			std::stringstream buffer;
			buffer << ifstrm.rdbuf();

			// Make a std::string and fill it with the contents of buffer
			std::string fileContent = buffer.str();

			return fileContent;
		}
	}
}