/*
* model_container.h
*
* Class for holding models and properly loading and freeing them 
* from gpu memory and properly selecting them when drawing
*
* Author: Erik Kirshey
* Date: 10/4/17
*/
#pragma once
#include <vector>
#include <GL/glew.h>
#include <assimp/scene.h>           // Output data structure
#include "assets/model.h"
#include "shaders/shader_program.h"

namespace SAS_3D {
	namespace Assets {
		#define MAXMODELS 100
		using ModelIdx = int;
		using LoadedModelIdx = int;
		class ModelContainer {
			public:
				ModelContainer();
				~ModelContainer();

				ModelIdx AddModel(const aiScene* scene);
				void LoadModelIntoGPU(ModelIdx idx);
				void Draw(ModelIdx idx);
			private:
				std::vector<Model> _models;
		};
	}
}