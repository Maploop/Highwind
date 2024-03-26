#pragma once

#include "logger.hpp"
#include "stb_image.h"
#include <glad/glad.h>

class Texture {
public:
	Texture(const char* path) {
		load_texture(path);
	}

	~Texture() {
		glDeleteTextures(GL_TEXTURE_2D, &texture_index);
	}

	GLuint get_texture_index() {
		return texture_index;
	}

	void bind() {
		glBindTexture(GL_TEXTURE_2D, texture_index);
	}
private:
	GLuint texture_index;

	void load_texture(const char* path) {
		glGenTextures(1, &texture_index);
		glBindTexture(GL_TEXTURE_2D, texture_index);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, number_of_channels;
		unsigned char* data = stbi_load(path, &width, &height, &number_of_channels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			HERROR("Texture.hpp: 'load_texture' Failed to execute correctly: DATA_PTR_UNDEFINED");
		}

		stbi_image_free(data);
	}
};