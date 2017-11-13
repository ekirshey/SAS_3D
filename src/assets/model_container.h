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
#include <assimp/Importer.hpp>      // C++ importer interface
#include "assets/model.h"
#include "shaders/texture_shader.h"

namespace SAS_3D {
	namespace Assets {
		#define MAXMODELS 100
		using ModelIdx = int;
		using LoadedModelIdx = int;
		class ModelContainer {
			public:
				ModelContainer(std::string modelpath, std::string texturepath);
				~ModelContainer();

				ModelIdx LoadModelFromFile(std::string path, unsigned int flags);
				void LoadModelIntoGPU(ModelIdx idx);
				void Draw(ModelIdx idx, const Shaders::TextureShader& textureshader);
			private:
				std::string _modelpath;
				std::string _texturepath;
				std::vector<Model> _models;
		};
	}
}