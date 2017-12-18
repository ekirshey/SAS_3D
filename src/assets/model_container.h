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
#include "assets/model.h"
#include "assets/textures.h"
#include "render_engine/shaders/shader_program.h"

namespace SAS_3D {
	#define MAXMODELS 100
	using ModelIdx = unsigned int;
	class ModelContainer
	{
	public:
		ModelContainer(std::string modelpath);
		~ModelContainer();

		ModelIdx LoadModelFromFile(std::string path, unsigned int flags);
		void LoadModelIntoGPU(ModelIdx idx);
		void Draw(ModelIdx idx, ShaderProgram& shader);
		// Debug draw
		void DrawSkeleton(ModelIdx idx, glm::mat4& m, glm::mat4& v, glm::mat4 p, ShaderProgram& shader);
	private:
		std::string _modelpath;
		TextureContainer _tc;
		std::vector<Model> _models;
	};

	namespace ModelLoader {
		void LoadModels(std::string model_registry, ModelContainer& mc);
	}
}