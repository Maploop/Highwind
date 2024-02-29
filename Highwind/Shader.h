#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <SOIL2.h>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"

class Shader {
public:
	Shader(const int versionMajor, const int versionMinor, const char* vertexFile, const char* fragmentFile, const char* geometryFile = "")
	: m_versionMajor(versionMajor), m_versionMinor(versionMinor) {

		GLuint vertexShader = 0;
		GLuint geometryShader = 0;
		GLuint fragmentShader = 0;

		vertexShader = load_shader(GL_VERTEX_SHADER, vertexFile);
		if (geometryFile != "")
			geometryShader = load_shader(GL_GEOMETRY_SHADER, geometryFile);
		fragmentShader = load_shader(GL_FRAGMENT_SHADER, fragmentFile);

		this->link_program(vertexShader, geometryShader, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);
	}

	~Shader() {
		glDeleteProgram(this->m_id);
	}

	void use() {
		glUseProgram(this->m_id);
	}

	void unuse() {
		glUseProgram(0);
	}

	void set1i(const char* varName, GLint value) {
		this->use();
		glUniform1i(glGetUniformLocation(this->m_id, varName), value);
		this->unuse();
	}

	void setMat3fv(const char* varName, glm::mat3 value, GLboolean transpose = GL_FALSE) {
		this->use();
		glUniformMatrix3fv(glGetUniformLocation(this->m_id, varName), 1, transpose, glm::value_ptr(value));
		this->unuse();
	}

	void setMat4fv(const char* varName, glm::mat4 value, GLboolean transpose = GL_FALSE) {
		this->use();
		glUniformMatrix4fv(glGetUniformLocation(this->m_id, varName), 1, transpose, glm::value_ptr(value));
		this->unuse();
	}

	void setVec4f(const char* varName, glm::fvec4 value) {
		this->use();
		glUniform4fv(glGetUniformLocation(this->m_id, varName), 1, glm::value_ptr(value));
		this->unuse();
	}

	void setVec3f(const char* varName, glm::fvec3 value) {
		this->use();
		glUniform3fv(glGetUniformLocation(this->m_id, varName), 1, glm::value_ptr(value));
		this->unuse();
	}

	void setVec2f(const char* varName, glm::fvec2 value) {
		this->use();
		glUniform2fv(glGetUniformLocation(this->m_id, varName), 1, glm::value_ptr(value));
		this->unuse();
	}

	void set1f(const char* varName, GLfloat value) {
		this->use();
		glUniform1f(glGetUniformLocation(this->m_id, varName), value);
		this->unuse();
	}
private:
	GLuint m_id;
	const int m_versionMajor;
	const int m_versionMinor;

	std::string load_shader_source(const char* filePath) {
		std::string temp = "";
		std::string src = "";

		std::ifstream in_file;
		in_file.open(filePath);
		if (in_file.is_open()) {
			while (std::getline(in_file, temp))
				src += temp + "\n";
		}
		else {
			FERROR("Shader Error: Could not open file: %s", filePath);
		}

		in_file.close();

		std::string versionNr = std::to_string(m_versionMajor) + std::to_string(m_versionMinor) + "0";
		src.replace(src.find("#version"), 12, ("#version " + versionNr));

		FINFO("Loaded shader source %s", filePath);
		return src;
	}

	GLuint load_shader(GLenum type, const char* filePath) {
		GLint success;
		char infoLog[512];
		std::string str_src = this->load_shader_source(filePath);

		GLuint shader = glCreateShader(type);
		const GLchar* src = str_src.c_str();

		glShaderSource(shader, 1, &src, NULL);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			FERROR("Shader Error: Could not compile shader: %s", filePath);
			FERROR(infoLog);
		}
		else
			FINFO("Successfully compiled %s shader", filePath);

		return shader;
	}

	void link_program(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader) {
		char infoLog[512];
		GLint success;

		this->m_id = glCreateProgram();

		glAttachShader(this->m_id, vertexShader);
		if (geometryShader)
			glAttachShader(this->m_id, geometryShader);
		glAttachShader(this->m_id, fragmentShader);

		glLinkProgram(this->m_id);

		glGetProgramiv(this->m_id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->m_id, 512, NULL, infoLog);
			FERROR("Failed to link shader program! Stacktrace:");
			FERROR(infoLog);
		}
		else
			FINFO("Successfully linked shader programs!");

		glUseProgram(0);
	}
};