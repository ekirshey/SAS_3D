#include <iostream>
#include <assimp/scene.h>           // Output data structure
#include "json.hpp"
#include "core/sas_io.h"
#include "assimp_loader.h"

namespace SAS_3D{

	namespace AssimpLoader {
		void Load(std::string model_registry, AnimationContainer& ac, ModelContainer& mc, unsigned int flags) {
			using json = nlohmann::json;
			json j = json::parse(ReadFile(model_registry));
			auto registry = j.at("registry");
			std::string rootmodelpath = j.at("root_model_path");

			std::vector<std::string> loadedscenes;
			for (int i = 0; i < registry.size(); i++) {
				std::string model_path = registry[i].at("model_path");
				std::string fullpath = rootmodelpath + model_path;

				// Check if scene has already been loaded
				bool existingscene = false;
				for (auto p : loadedscenes) {
					if (p == fullpath) {
						existingscene = true;
						break;
					}
				}
				if (existingscene) {
					continue;
				}

				Assimp::Importer importer;
				const aiScene* scene = importer.ReadFile(fullpath, flags);
				// Check for errors
				if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) { // if is Not Zero
					std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
				}
				else {
					SceneInfo sceneinfo(fullpath, scene);
					ac.AddAnimation(sceneinfo);
					mc.AddModel(sceneinfo);
				}
			}
		}
	}
}