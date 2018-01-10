#pragma once
#include <string>

namespace SAS_3D {
	struct GameConfig {
		std::string windowtitle;
		int screenwidth;
		int screenheight;

		std::string shaderpath;
		std::string model_registry;

		std::string serverip;
		std::string port;
	};

	GameConfig LoadConfig(std::string ini);
}
