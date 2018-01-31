#pragma once
#include <string>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/postprocess.h>     // Post processing flags
#include "assets/model_container.h"
#include "subsystems/animation_engine/animator.h"

namespace SAS_3D {
	struct SceneInfo {
		std::string filepath;
		const aiScene* scene;
		SceneInfo(std::string p, const aiScene* s)
			: filepath(p)
			, scene(s)
		{}
	};
	namespace AssimpLoader {
		void Load(std::string model_registry, std::vector<Animator>& ac, ModelContainer& mc, TextureContainer& tc, unsigned int flags);
	};
}
