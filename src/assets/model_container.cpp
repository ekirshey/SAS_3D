#include <iostream>
#include "core/error_codes.h"
#include "model_container.h"

namespace SAS_3D {
	ModelContainer::ModelContainer(std::string modelpath)
		: _modelpath(modelpath)
	{

	}

	ModelContainer::~ModelContainer() {

	}

	ModelIdx ModelContainer::LoadModelFromFile(std::string path, unsigned int flags) {
		std::string fullpath = _modelpath + path;
		SetError(ErrorCode::NO_ERROR);

		if (_models.size() + 1 > MAXMODELS) {
			SetError(ErrorCode::MODEL_CONTAINER_FILLED);
			return -1;
		}

		// Check if model has already been loaded
		ModelIdx idx;
		for (auto& m : _models) {
			if (m.Path() == path) {
				return idx;
			}
			idx++;
		}

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(fullpath, flags);
		// Check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) { // if is Not Zero
			std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			SetError(ErrorCode::BAD_MODEL_FILE);
			return -1;
		}

		_models.push_back(Model(fullpath, _tc, scene));

		return static_cast<ModelIdx>(_models.size() - 1);
	}

	void ModelContainer::LoadModelIntoGPU(ModelIdx idx) {
		_models[idx].LoadIntoGPU();
	}

	void ModelContainer::Draw(ModelIdx idx, ShaderProgram& shader) {
		_models[idx].Draw(shader);
	}
}