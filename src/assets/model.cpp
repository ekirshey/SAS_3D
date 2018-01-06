#include <iostream>
#include "model.h"
#include "render_engine/shaders/input_modules/texture_module.h"
#include "render_engine/shaders/input_modules/animation_module.h"
#include "render_engine/shaders/input_modules/pvm_module.h"

namespace SAS_3D {
	Model::Model(std::string path, TextureContainer& c, const aiScene* scene)
		: _path(path)
		, _loaded(false)
		, _anim(scene)
	{
		// Build meshes
		if (scene->HasMeshes()) {
			auto lastslash = path.rfind('/') + 1;
			for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
				_meshes.push_back(Mesh(path.substr(0, lastslash), c, scene->mMeshes[i], scene));
			}
		}

		std::cout << "Done model processing..." << std::endl;
	}

	void Model::LoadIntoGPU() {
		if (!_loaded) {
			_loaded = true;
			for (int i = 0; i < _meshes.size(); i++) {
				_meshes[i].LoadIntoGPU();
			}
		}
	}
	
	void Model::DrawSkeleton(glm::mat4& m, glm::mat4& v, glm::mat4& p, Model& primitive, ShaderProgram& shader) {
		if (!_loaded) {
			// Load the model into memory
			LoadIntoGPU();
		}

		auto pvmmodule = shader.GetInputModule<PVMModule*>(PVMModule::ID);

		for (auto& me : _meshes) {

		}
	}

	void Model::Draw() {
		if (!_loaded) {
			// Load the model into memory
			LoadIntoGPU();
		}

		for (auto& m : _meshes) {
			glBindVertexArray(m.VAO);
			glDrawElements(GL_TRIANGLES, m.indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glActiveTexture(GL_TEXTURE0);
		}
	}

	void Model::Draw(ShaderProgram& shader, glm::mat4& m, glm::mat4& v, glm::mat4& p) {
		if (!_loaded) {
			// Load the model into memory
			LoadIntoGPU();
		}
		auto texturemodule = shader.GetInputModule<TextureModule*>(TextureModule::ID);
		auto animationmodule = shader.GetInputModule<AnimationModule*>(AnimationModule::ID);
		auto pvmmodule = shader.GetInputModule<PVMModule*>(PVMModule::ID);

		for (int i = 0; i < _meshes.size(); i++) {
			auto me = &_meshes[i];
			unsigned int tct = 0;
			for (auto& t : me->textures) {
				glActiveTexture(GL_TEXTURE0 + tct);
				texturemodule->SetTexture(tct);
				glBindTexture(GL_TEXTURE_2D, t);
				tct++;
			}
			std::vector<glm::mat4> bones;
			_anim.GetBoneMatrices(bones, i);
			animationmodule->SetBones(&bones);

			pvmmodule->SetPVM(p*v*m);
			shader.ApplyModules();

			glBindVertexArray(me->VAO);
			glDrawElements(GL_TRIANGLES, me->indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glActiveTexture(GL_TEXTURE0);
		}
	}
}