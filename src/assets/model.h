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
		Vertex() 
			: bones(4, 0)	// 255 could be a bone...
			, weights(4, 0.0f)
		{}

		// Position
		glm::vec3 Position;
		// Normal
		glm::vec3 Normal;
		// TexCoords
		glm::vec2 TexCoords;
		// Bones
		std::vector<GLuint> bones;
		// Weights
		std::vector<GLfloat> weights;
	};

	struct Bone {
		std::string name;
		glm::mat4 offsetmatrix;
		std::unordered_map<unsigned int, std::vector<float>> weights;
	};

	struct Node {
		std::string parent;
		std::vector<std::string> children;
	};

	using Frame = std::unordered_map<std::string, glm::mat4>;

	struct Animation {
		double tickspersecond;
		double duration;
		std::map<double, Frame> frames;

		bool CreateNewFrame(double time, std::unordered_map<std::string, Node>& nodes) {
			if (time < 0)
				return false;
			if (frames.find(time) == frames.end()) {
				auto it = nodes.begin();
				while (it != nodes.end()) {
					frames[time].insert({ it->first, glm::mat4() });
					it++;
				}
			}
			return true;
		}

		void TranslateNode(double time, std::string node, glm::vec3 pos) {
			frames[time].at(node) = glm::translate(frames[time].at(node), pos);
		}

		void RotateNode(double time, std::string node, glm::quat rot) {
			glm::mat4 mat = frames[time].at(node);
			frames[time].at(node) =  mat * glm::toMat4(rot);
		}

		void ScaleNode(double time, std::string node, glm::vec3 scale) {
			frames[time].at(node) = glm::scale(frames[time].at(node), scale);
		}

	};

	class Mesh {
	public:
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<GLuint> textures;
		std::vector<Bone> bones;
		std::map<double, std::vector<glm::mat4>> bonematrices;
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		void LoadIntoGPU();
		void UnloadFromGPU();

		Mesh(std::string modelpath, TextureContainer& c, const aiMesh* ai_m, const aiScene* scene);
		void BuildBoneMatrices(std::unordered_map<std::string, Animation>& animations);
	private:
		void _loadMaterialTextures(std::string modelpath, TextureContainer& c, aiMaterial *mat, aiTextureType type, std::string type_name);
	};

	class Model {
	public:
		Model(std::string path, TextureContainer& c, const aiScene* scene);
		void LoadIntoGPU();
		void Draw(ShaderProgram& shader);
		std::string Path() { return _path; }
	private:
		std::string _path;
		std::vector<Mesh> _meshes;
		std::unordered_map<std::string, Node> _nodes;
		std::unordered_map<std::string, Animation> _animations;
		bool _loaded;
	};
}