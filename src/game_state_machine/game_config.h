#pragma once
#include "core/sas_video.h"
#include <string>

namespace SAS_3D {
	struct GameConfig {
		std::string windowtitle;

		std::string shaderpath;
		std::string model_registry;

		std::string serverip;
		std::string port;

		VideoConfig videoconfig;
	};

	GameConfig LoadConfig(std::string ini);
}
