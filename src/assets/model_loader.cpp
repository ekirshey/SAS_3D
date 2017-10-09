/*
* model_loader.h
*
* Set of functions for handling the loading of models into the game engine
* Utliizes the assimp library to load in the various 3d file types
*
* Author: Erik Kirshey
* Date: 10/1/17
*/
#include <iostream>
#include "model_loader.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure

namespace SAS_3D {
	namespace Assets {
		namespace ModelLoader {
			ModelIdx LoadModelFromFile(ModelContainer& mc, std::string path, unsigned int flags) {
				Core::SetError(Core::ErrorCode::NO_ERROR);

				Assimp::Importer importer;

				const aiScene* scene = importer.ReadFile(path, flags);
				// Check for errors
				if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) { // if is Not Zero
					std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
					Core::SetError(Core::ErrorCode::BAD_MODEL_FILE);
					return -1;
				}

				auto idx = mc.AddModel(scene);

				return idx;
			}

		}
	}
}