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
#include "subsystems/render_engine/shaders/shader_program.h"

namespace SAS_3D {
	#define MAXMODELS 100
	using ModelIdx = unsigned int;
	class ModelContainer
	{
	public:
		ModelContainer();
		
		void AddModel(const SceneInfo& sceneinfo, TextureContainer& tc);
		void LoadModelIntoGPU(ModelIdx idx);
		void Draw(ModelIdx idx, ShaderProgram& shader, const glm::mat4& pv, const glm::mat4& m, const BoneMatrix* bones = nullptr);
		// Debug draw
		void DrawSkeleton(ModelIdx idx, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, ShaderProgram& shader) const;
		bool HasTextures(ModelIdx idx) const { return _models[idx].HasTextures(); }
	private:
		std::string _modelpath;
		std::vector<Model> _models;
	};
}