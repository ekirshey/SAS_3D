#include <iostream>
#include "core/error_codes.h"
#include "model_container.h"

namespace SAS_3D {
	namespace Assets {
		ModelContainer::ModelContainer(std::string modelpath, std::string texturepath) 
			: _modelpath(modelpath)
			, _texturepath(texturepath)
		{

		}

		ModelContainer::~ModelContainer() {

		}

		ModelIdx ModelContainer::LoadModelFromFile(std::string path, unsigned int flags) {
			Core::SetError(Core::ErrorCode::NO_ERROR);

			if (_models.size()+1 > MAXMODELS) {
				Core::SetError(Core::ErrorCode::MODEL_CONTAINER_FILLED);
				return -1;
			}

			Assimp::Importer importer;
			std::string fullpath = _modelpath + path;
			const aiScene* scene = importer.ReadFile(fullpath, flags);
			// Check for errors
			if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) { // if is Not Zero
				std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
				Core::SetError(Core::ErrorCode::BAD_MODEL_FILE);
				return -1;
			}

			_models.push_back(Model(fullpath, scene));

			return static_cast<ModelIdx>(_models.size()-1);
		}

		void ModelContainer::LoadModelIntoGPU(ModelIdx idx) {
			_models[idx].LoadIntoGPU();
		}

		void ModelContainer::Draw(ModelIdx idx, const Shaders::TextureShader& textureshader) {
			_models[idx].Draw(textureshader);
		}
	}
}