#include <iostream>
#include <GL/glew.h>
#include <assimp/scene.h>           // Output data structure
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/postprocess.h>     // Post processing flags
#include "core/sas_io.h"
#include "json.hpp"
#include "core/error_codes.h"
#include "model_container.h"
#include "subsystems/animation_engine/anim_state.h"

namespace SAS_3D {
	ModelContainer::ModelContainer() {

	}

	void ModelContainer::AddModel(const SceneInfo& sceneinfo, TextureContainer& tc) {
		_models.push_back(Model(&sceneinfo, tc));
	}

	void ModelContainer::LoadModelIntoGPU(ModelIdx idx) {
		_models[idx].LoadIntoGPU();
	}

	void ModelContainer::Draw(ModelIdx idx, ShaderProgram& shader, glm::mat4& pv, glm::mat4& m, BoneMatrix* bones) {
		// Make sure the model exists/loaded
		if (idx < _models.size()) {
			_models[idx].Draw(shader, pv, m, bones);
		}
	}

	void ModelContainer::DrawSkeleton(ModelIdx idx, glm::mat4& m, glm::mat4& v, glm::mat4& p, ShaderProgram& shader) {
		// Make sure the model exists/loaded
		if (idx < _models.size()) {
			_models[idx].DrawSkeleton(m,v,p, _models[1], shader);
		}
	}

}