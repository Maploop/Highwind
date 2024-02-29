#pragma once

#include <iostream>
#include <string>
#include <SOIL2.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logger.hpp"

class Texture {
public:
	Texture(const char* path, GLenum type) {
		m_type = type;
		FINFO("Loading texture %s", path);
		unsigned char* image = SOIL_load_image(path, &m_width, &m_height, NULL, SOIL_LOAD_RGBA);
		glGenTextures(1, &m_id);
		glBindTexture(type, m_id);

		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (image) {
			glTexImage2D(type, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(type);
		}
		else {
			FERROR("Error while loading texture %s", path);
		}

		glActiveTexture(0);
		glBindTexture(type, 0);
		SOIL_free_image_data(image);
	}
	~Texture() {
		glDeleteTextures(1, &m_id);
	}

	inline GLuint get_id() const { return m_id; }
	
	void bind(const GLint textureUnit) {
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(m_type, m_id);
	}

	void unbind() {
		glActiveTexture(0);
		glBindTexture(m_type, 0);
	}
private:
	GLuint m_id;
	unsigned int m_type;
	int m_width;
	int m_height;

	void load_from_file(const char* path) {
		if (m_id) {
			glDeleteTextures(1, &m_id);
		}

		FINFO("Loading texture %s", path);
		unsigned char* image = SOIL_load_image(path, &m_width, &m_height, NULL, SOIL_LOAD_RGBA);
		glGenTextures(1, &m_id);
		glBindTexture(m_type, m_id);

		glTexParameteri(m_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(m_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (image) {
			glTexImage2D(m_type, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(m_type);
		}
		else {
			FERROR("Error while loading texture %s", path);
		}

		glActiveTexture(0);
		glBindTexture(m_type, 0);
		SOIL_free_image_data(image);
	}
};