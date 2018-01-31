#include <iostream>
#include <GL/glew.h>
#include "assimp/Importer.hpp"
#include "assets/textures.h"
#include "FreeImage/FreeImage.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace SAS_3D {

	FIBITMAP* _loadTexture(std::string path) {
		//image format
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//pointer to the image, once loaded
		FIBITMAP *dib(0);

		fif = FreeImage_GetFileType(path.c_str(), 0);
		if (fif == FIF_UNKNOWN) {
			fif = FreeImage_GetFIFFromFilename(path.c_str());
		}

		//if still unkown, return failure
		if (fif == FIF_UNKNOWN) {
			std::cout << "Texture failed to load. Unknown File type: " << path << std::endl;
			return 0;
		}

		//check that the plugin has reading capabilities and load the file
		if (FreeImage_FIFSupportsReading(fif)) {
			dib = FreeImage_Load(fif, path.c_str());
		}

		//if the image failed to load, return failure
		if (!dib) {
			std::cout << "Texture failed to load. " << path << std::endl;
			return 0;
		}

		FreeImage_FlipVertical(dib);

		//retrieve the image data
		auto bpp = FreeImage_GetBPP(dib);
		if (bpp != 32) {
			FIBITMAP* hOldImage = dib;
			dib = FreeImage_ConvertTo32Bits(hOldImage);
			FreeImage_Unload(hOldImage);
		}

		return dib;
	}

	GLuint TextureContainer::LoadTextureFromFile(std::string path, std::string type_name, GLint format) {
		bool loaded = true;
		Texture texture;

		auto it = _textures.begin();
		while (it != _textures.end()) {
			if (it->second.path == path) {
				return it->first;
			}
			it++;
		}

		texture.path = path;
		texture.type = type_name;

		FIBITMAP* dib = _loadTexture(path);
		BYTE* bits = FreeImage_GetBits(dib);

		if (bits == NULL) {
			std::cout << "Bit data not valid for " << path << std::endl;
			return -1;
		}

		//get the image width and height
		unsigned int width = FreeImage_GetWidth(dib);
		unsigned int height = FreeImage_GetHeight(dib);
		unsigned pitch = FreeImage_GetPitch(dib);
		//if this somehow one of these failed (they shouldn't), return failure
		if ((bits == 0) || (width == 0) || (height == 0)) {
			std::cout << "Texture failed to load." << path << std::endl;
			return -1;
		}

		GLenum err;
		GLuint id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, format, GL_UNSIGNED_BYTE, bits);
		err = glGetError();
		if (err != 0) {
			std::cerr << "Error when loading texture: " << err << std::endl;
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		FreeImage_Unload(dib);

		_textures.insert({ id, texture });
		return id;
	}

	GLuint TextureContainer::LoadCubeMap(std::string path, GLint format) {
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		std::vector<std::string> faces =
		{
			path + "right.jpg",
			path + "left.jpg",
			path + "top.jpg",
			path + "bottom.jpg",
			path + "front.jpg",
			path + "back.jpg"
		};

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				auto err = glGetError();
				if (err != 0) {
					std::cerr << "Error when loading texture: " << err << std::endl;
				}
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;

#ifdef FOO
		int width, height;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			FIBITMAP* dib = _loadTexture(faces[i]);
			BYTE* data = FreeImage_GetBits(dib);
			unsigned int width = FreeImage_GetWidth(dib);
			unsigned int height = FreeImage_GetHeight(dib);
			unsigned pitch = FreeImage_GetPitch(dib);

			if (data) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				err = glGetError();
				if (err != 0) {
					std::cerr << "Error when loading texture: " << err << std::endl;
				}
				FreeImage_Unload(dib);
			}
			else {
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				FreeImage_Unload(dib);
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;
#endif
	}
}