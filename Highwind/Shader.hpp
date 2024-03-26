#pragma once

#include <glad/glad.h>
#include "logger.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath) {
		load_from_files(vertexPath, fragmentPath);
		compile_shaders();
	}

	~Shader() {
		glDeleteProgram(program_id);
	}

	void use_program() {
		glUseProgram(program_id);
	}

	void set_bool(const char* name, bool value) const {
		glUniform1i(glGetUniformLocation(program_id, name), (int)value);
	}

	void set_int(const char* name, int value) const {
		glUniform1i(glGetUniformLocation(program_id, name), value);
	}

	void set_float(const char* name, float value) const {
		glUniform1f(glGetUniformLocation(program_id, name), value);
	}
public:
	GLuint program_id;

private:
	std::string vertex_code;
	std::string fragment_code;

	void compile_shaders() {
		GLuint vertex, fragment;
		int success;
		char infoLog[512];

		// Compile vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		auto tmp = vertex_code.c_str();
		glShaderSource(vertex, 1, &tmp, NULL);
		glCompileShader(vertex);

		// Check if we compiled correctly
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			HERROR("Shader.hpp: 'compile_shaders' (Vertex) Failed to execute correctly: %s", infoLog);
		}

		// Compile fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		tmp = fragment_code.c_str();
		glShaderSource(fragment, 1, &tmp, NULL);
		glCompileShader(fragment);

		// Check if we compiled correctly
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			HERROR("Shader.hpp: 'compile_shaders' (Fragment) Failed to execute correctly: %s", infoLog);
		}

		// Now we create link and set the program ID
		program_id = glCreateProgram();
		glAttachShader(program_id, vertex);
		glAttachShader(program_id, fragment);
		glLinkProgram(program_id);

		// Check if the program managed to link up correctly
		glGetProgramiv(program_id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program_id, 512, NULL, infoLog);
			HERROR("Shader.hpp: 'compile_shaders' (Program Linking) Failed to execute correctly: %s", infoLog);
		}

		// Now delete the shaders cause we don't need them
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void load_from_files(const char* vert_path, const char* frag_path) {
		vertex_code = load_from_file(vert_path).c_str();
		fragment_code = load_from_file(frag_path).c_str();
	}

	std::string load_from_file(const char* path) {
		std::ifstream path_stream;

		try {
			path_stream.open(path);
			std::stringstream str_stream;
			str_stream << path_stream.rdbuf();
			path_stream.close();

			return str_stream.str();
		}
		catch (std::ifstream::failure e) {
			HERROR("Shader.hpp: 'load_from_file' Failed to execute correctly.");
		}
	}
};