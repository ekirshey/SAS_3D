#include <iostream>
#include <GL/glew.h>
#include "assimp/Importer.hpp"
#include "assets/textures.h"
#include "FreeImage/FreeImage.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace SAS_3D {
	namespace Assets {
		Texture LoadTextureFromFile(const char* path, std::string type_name, GLint format) {
			bool loaded = true;
			Texture texture;

			//image format
			FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
			//pointer to the image, once loaded
			FIBITMAP *dib(0);
			//pointer to the image data
			BYTE* bits(0);
			unsigned int width;
			unsigned int height;

			texture.path = path;
			texture.type = type_name;

			fif = FreeImage_GetFileType(path, 0);
			if (fif == FIF_UNKNOWN) {
				fif = FreeImage_GetFIFFromFilename(path);
			}

			//if still unkown, return failure
			if (fif == FIF_UNKNOWN) {
				std::cout << "Texture failed to load. Unknown File type: " << path << std::endl;
				return texture;
			}

			//check that the plugin has reading capabilities and load the file
			if (FreeImage_FIFSupportsReading(fif)) {
				dib = FreeImage_Load(fif, path);
			}

			//if the image failed to load, return failure
			if (!dib) {
				std::cout << "Texture failed to load. " << path << std::endl;
				return texture;
			}

			FreeImage_FlipVertical(dib);

			//retrieve the image data
			bits = FreeImage_GetBits(dib);

			FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib);

			//get the image width and height
			width = FreeImage_GetWidth(dib);
			height = FreeImage_GetHeight(dib);
			unsigned pitch = FreeImage_GetPitch(dib);
			//if this somehow one of these failed (they shouldn't), return failure
			if ((bits == 0) || (width == 0) || (height == 0)) {
				std::cout << "Texture failed to load." << path << std::endl;
				return texture;
			}

			GLenum err;
			glGenTextures(1, &texture.id);
			glBindTexture(GL_TEXTURE_2D, texture.id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, bits);
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

			return texture;
		}
	}
}