#pragma once
#include <string>

namespace SAS_3D {
	/*
	* Verious options for configuring the window.
	*/
	struct VideoConfig {
		std::string windowtitle;
		bool fullscreen;
		bool vsync;

		int width;
		int height;
	};
}