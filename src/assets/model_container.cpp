#include <iostream>
#include <GL/glew.h>
#include <assimp/scene.h>           // Output data structure
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/postprocess.h>     // Post processing flags
#include "core/sas_io.h"
#include "json.hpp"
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
		for (int i = 0; i < _models.size(); i++) {
			if (_models[i].Path() == path) {
				return i;
			}
		}

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(fullpath, flags);
		// Check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) { // if is Not Zero
			std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
//			SetError(ErrorCode::BAD_MODEL_FILE);
			return -1;
		}

		_models.push_back(Model(fullpath, _tc, scene));

		return static_cast<ModelIdx>(_models.size() - 1);
	}

	void ModelContainer::LoadModelIntoGPU(ModelIdx idx) {
		_models[idx].LoadIntoGPU();
	}

	void ModelContainer::Draw(ModelIdx idx, ShaderProgram& shader) {
		// Make sure the model exists/loaded
		if (idx < _models.size()) {
			_models[idx].Draw(shader);
		}
	}

	namespace ModelLoader {
		void LoadModels(std::string model_registry, ModelContainer& mc) {
			using json = nlohmann::json;
			json j = json::parse(ReadFile(model_registry));
			auto registry = j.at("registry");

			for (int i = 0; i < registry.size(); i++) {
				auto temp = mc.LoadModelFromFile(
					registry[i].at("model_path")
					, aiProcess_Triangulate | aiProcess_FlipUVs);

				if (temp == -1) {
					std::cerr << "Error Loading Model." << std::endl;
				}
			}
		}
	}
}