#include <iostream>
#include "model.h"
#include "subsystems/render_engine/shaders/input_modules/texture_module.h"
#include "subsystems/render_engine/shaders/input_modules/animation_module.h"
#include "subsystems/render_engine/shaders/input_modules/pvm_module.h"
#include "assets/assimp_loader.h"

namespace SAS_3D {
	Model::Model(const SceneInfo* sceneinfo, TextureContainer& c)
		: _path(sceneinfo->filepath)
		, _loaded(false)
	{
		// Build meshes
		auto scene = sceneinfo->scene;
		auto root = scene->mRootNode;
		for (int i = 0; i < root->mNumChildren; i++) {
			auto c = root->mChildren[i];
			if (c->mNumMeshes > 0) {
				_transform = aiMatrix4x4ToGlm(c->mTransformation);
			}
		}

		if (scene->HasMeshes()) {
			auto lastslash = _path.rfind('/') + 1;
			for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
				_meshes.push_back(Mesh(_path.substr(0, lastslash), c, scene->mMeshes[i], scene));
			}
		}

		std::cout << "Done model processing..." << std::endl;
	}

	bool Model::HasTextures() {
		bool hastextures = false;
		for (auto& m : _meshes) {
			if (m.textures.size() > 0) {
				hastextures = true;
				break;
			}
		}
		return hastextures;
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

	void Model::Draw(ShaderProgram& shader, glm::mat4& pv, glm::mat4& m, BoneMatrix* bones) {
		if (!_loaded) {
			// Load the model into memory
			LoadIntoGPU();
		}
		auto materialmodule = shader.GetInputModule<MaterialModule*>(MaterialModule::ID);
		auto animationmodule = shader.GetInputModule<AnimationModule*>(AnimationModule::ID);
		auto pvmmodule = shader.GetInputModule<PVMModule*>(PVMModule::ID);

		for (int i = 0; i < _meshes.size(); i++) {
			auto me = &_meshes[i];
			unsigned int tct = 0;
			if (materialmodule != nullptr) {
				// Needs to change
				for (auto& t : me->textures) {
					glActiveTexture(GL_TEXTURE0 + tct);
					materialmodule->SetMaterial(tct);
					glBindTexture(GL_TEXTURE_2D, t);
					tct++;
				}
			}

			if (animationmodule != nullptr) {
				if (bones != nullptr && bones->size() >= (i + 1)) {
					animationmodule->SetBones(&bones->at(i));
				}
			}

			pvmmodule->SetPVM(pv, m*_transform);
			shader.ApplyModules();

			glBindVertexArray(me->VAO);
			glDrawElements(GL_TRIANGLES, me->indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glActiveTexture(GL_TEXTURE0);
		}
	}

}