#include <iostream>
#include "model_container.h"

namespace SAS_3D {
	namespace Assets {
		ModelContainer::ModelContainer() {

		}

		ModelContainer::~ModelContainer() {

		}


		ModelIdx ModelContainer::AddModel(const aiScene* scene) {
			Core::SetError(Core::ErrorCode::NO_ERROR);
			if (_models.size() > MAXMODELS) {
				Core::SetError(Core::ErrorCode::MODEL_CONTAINER_FILLED);
				return -1;
			}

			_models.push_back(Model(scene));

			return (ModelIdx)(_models.size()-1);
		}

		void ModelContainer::LoadModelIntoGPU(ModelIdx idx) {
			// Might move this somewhere else
			for (int i = 0; i < _models[idx].meshes.size(); i++) {
				_models[idx].meshes[i].LoadIntoGPU();
			}
		}

		void ModelContainer::Draw(ModelIdx idx) 
		{
			auto model = &_models[idx];
			if (!model->loaded) {
				std::cout << "Trying to draw unloaded model: " << idx << std::endl;
			}

			for (int i = 0; i < _models[idx].meshes.size(); i++) {
				auto m = &model->meshes[i];
				glBindVertexArray(m->VAO);
				glDrawElements(GL_TRIANGLES,m->indices.size(), GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
	}
}