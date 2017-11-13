#pragma once
#include <string>

namespace SAS_3D {
	namespace GSM {
		struct GameConfig {
			std::string windowtitle;
			int screenwidth;
			int screenheight;

			std::string shaderpath;
			std::string modelpath;
			std::string texturepath;
		};

		GameConfig LoadConfig(std::string ini);
	}
}
