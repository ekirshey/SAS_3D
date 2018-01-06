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
#include "assets/animator.h"
#include "assets/mesh.h"
#include "render_engine/shaders/shader_program.h"

namespace SAS_3D {

	class Model {
	public:
		Model(std::string path, TextureContainer& c, const aiScene* scene);
		void LoadIntoGPU();
		void Draw(ShaderProgram& shader, glm::mat4& m, glm::mat4& v, glm::mat4& p);
		void Draw(); //Debug draw, just loads indices into opengl
		void DrawSkeleton(glm::mat4& m, glm::mat4& v, glm::mat4& p, Model& primitive, ShaderProgram& shader);
		std::string Path() { return _path; }
	private:
		Animator _anim;
		std::string _path;
		std::vector<Mesh> _meshes;
		bool _loaded;
	};
}