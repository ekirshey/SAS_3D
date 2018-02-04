/*
* texture.h
*
* Defines structures and functions for managing textures
* Separated from model.h right now but could hypothetically be merged in
*
* Author: Erik Kirshey
* Date: 10/4/17
*/
#pragma once
#include <string>
#include <unordered_map>

namespace SAS_3D {
	struct Texture {
		std::string type;
		std::string path;
	};

	class TextureContainer {
	public:
		TextureContainer() {}
		// FreeImage appears to load everything as BGR and NOT RGB. STBI uses RGB but does not load DDS so using freeimage
		GLuint LoadTextureFromFile(std::string path, std::string type_name, GLint format = GL_BGRA);
		GLuint LoadCubeMap(std::string path, GLint format = GL_BGR);
	private:
		std::unordered_map<GLuint, Texture> _textures;
	};

}
