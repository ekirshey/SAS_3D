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
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/scene.h>           // Output data structure
#include "assets/textures.h"
#include "render_engine/shaders/shader_program.h"

namespace SAS_3D {
	struct Vertex {
		Vertex() {
			for (int i = 0; i < 4; i++) {
				bones[i] = 0;
				weights[i] = 0.0f;
			}
		}

		// Position
		glm::vec3 Position;
		// Normal
		glm::vec3 Normal;
		// TexCoords
		glm::vec2 TexCoords;
		// Bones
		GLuint bones[4];
		// Weights
		GLfloat weights[4];
	};

	struct Joint {
		std::string name;
		glm::mat4 offsetmatrix;
	};

	struct JointPose {
		JointPose() {
			rotation = glm::quat();
			translation = glm::vec3(1.0,1.0,1.0);
			scale = glm::vec3(1.0,1.0,1.0);
		}
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 scale;
	};

	struct SkeletonPose {
		std::vector<JointPose> local_pose;
		std::vector<glm::mat4> global_pose;
	};

	struct Animation {
		double tickspersecond;
		double duration;
	};

	struct Node {
		std::string name;
		glm::mat4 transformation;
		int parent;	// reference into node vector
	};

	class Mesh {
	public:
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<GLuint> textures;
		std::vector<Joint> skeleton;
		glm::mat4 globalmeshtransform;
		Node rootnode;
		std::unordered_map<std::string, int> bonemap;
		SkeletonPose pose;
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		void LoadIntoGPU();
		void UnloadFromGPU();

		Mesh(std::string modelpath, TextureContainer& c, const aiMesh* ai_m, const aiScene* scene);
		void BuildBoneMatrices(std::unordered_map<std::string, Animation>& animations, const aiScene* scene);
		void BuildGlobalTransforms(aiNode* node);
		int FindRoot(aiNode* node);
	private:
		void _loadMaterialTextures(std::string modelpath, TextureContainer& c, aiMaterial *mat, aiTextureType type, std::string type_name);
	};

	class Model {
	public:
		Model(std::string path, TextureContainer& c, const aiScene* scene);
		void LoadIntoGPU();
		void Draw(ShaderProgram& shader, glm::mat4& m, glm::mat4& v, glm::mat4& p);
		void Draw(); //Debug draw, just loads indices into opengl
		void DrawSkeleton(glm::mat4& m, glm::mat4& v, glm::mat4& p, Model& primitive, ShaderProgram& shader);
		std::string Path() { return _path; }
	private:
		void BuildMeshes(const aiScene* scene, TextureContainer& c, aiNode* node, std::string path);
		std::vector<Node> nodes;
		std::string _path;
		std::vector<Mesh> _meshes;
		std::unordered_map<std::string, Animation> _animations;
		bool _loaded;
	};
}