/*
* model_loader.h
*
* Set of functions for handling the loading of models into the game engine
* Utliizes the assimp library to load in the various 3d file types
*
* Author: Erik Kirshey
* Date: 10/1/17
*/

#pragma once
#include <string>
#include <assimp/postprocess.h>     // Post processing flags
#include "core/error_codes.h"
#include "model_container.h"

namespace SAS_3D {
	namespace Assets {
		namespace ModelLoader {
			ModelIdx LoadModelFromFile(ModelContainer& mc, std::string path, unsigned int flags);
		};
	}
}