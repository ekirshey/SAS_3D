#include <iostream>
#include <GL/glew.h>
#include "assimp/Importer.hpp"
#include "assets/textures.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace SAS_3D {
	namespace Assets {
		Texture LoadTextureFromFile(const char* path, std::string type_name) {
			bool loaded = true;
			Texture texture;

			int width, height, nrComponents;
			unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
			if (data) {
				GLenum iformat;
				if (nrComponents == 1)
					iformat = GL_RED;
				else if (nrComponents == 3)
					iformat = GL_RGB;
				else if (nrComponents == 4)
					iformat = GL_RGBA;

				// Generate OpenGL texture
				glGenTextures(1, &texture.id);
				glBindTexture(GL_TEXTURE_2D, texture.id);
				glTexImage2D(GL_TEXTURE_2D, 0, iformat, width, height, 0, iformat, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

			stbi_image_free(data);

			return texture;
		}
	}
}