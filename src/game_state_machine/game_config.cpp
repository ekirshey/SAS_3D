#include <iostream>
#include <stdexcept>
#include "game_config.h"
#include "third_party/INIReader.h"
#include "core/error_codes.h"

namespace SAS_3D {
	void sanitizePath(std::string& path) {
		if (path.back() != '/') {
			path += '/';
		}
	}
	GameConfig LoadConfig(std::string ini) {
		GameConfig config;
		// Load values from configfile ini
		INIReader reader(ini);

		if (reader.ParseError() < 0) {
			std::cerr << "Could not load file " << ini << std::endl;
			throw std::runtime_error("Could not load file " + ini);
		}

		config.windowtitle = reader.Get("video", "windowtitle", "default");
		config.videoconfig.width = reader.GetInteger("video", "width", 800);
		config.videoconfig.height = reader.GetInteger("video", "height", 600);
		config.videoconfig.vsync = reader.GetBoolean("video", "VSYNC", true);
		config.videoconfig.fullscreen = reader.GetBoolean("video", "fullscreen", true);

		config.shaderpath = reader.Get("paths", "shader_path", "");
		sanitizePath(config.shaderpath);

		config.model_registry = reader.Get("paths", "model_registry", "");

		config.serverip = reader.Get("network", "server_ip", "127.0.0.1");
		config.port = reader.Get("network", "port", "25977");


		return config;
	}
}