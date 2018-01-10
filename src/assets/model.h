/*
* model.h
*
* Defines the engine specific format for models
*
* Author: Erik Kirshey
* Date: 10/4/17
*/
#pragma once
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>           // Output data structure
#include "assets/mesh.h"
#include "subsystems/render_engine/shaders/shader_program.h"

namespace SAS_3D {
	struct SceneInfo;
	class Model {
	public:
		Model(const SceneInfo* sceneinfo, TextureContainer& c);
		void LoadIntoGPU();
		void Draw(ShaderProgram& shader, glm::mat4& pvm);
		void Draw(); //Debug draw, just loads indices into opengl
		void DrawSkeleton(glm::mat4& m, glm::mat4& v, glm::mat4& p, Model& primitive, ShaderProgram& shader);
		std::string Path() { return _path; }
	private:
		std::string _path;
		std::vector<Mesh> _meshes;
		bool _loaded;
	};
}