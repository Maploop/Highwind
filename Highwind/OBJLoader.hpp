#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Vertex.h"
#include "logger.hpp"

static std::vector<Vertex> loadObjFile(const char* file_name) {
	// portions
	std::vector<glm::fvec3> positions;
	std::vector<glm::fvec2> texcoords;
	std::vector<glm::fvec3> normals;

	// face
	std::vector<GLint> position_indices;
	std::vector<GLint> texcoord_indices;
	std::vector<GLint> normal_indices;

	std::vector<Vertex> vertices;
	std::stringstream ss;
	std::ifstream in_file(file_name);
	std::string line = "";
	std::string prefix = "";
	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLint temp_glint = 0;

	if (!in_file.is_open()) {
		FERROR("Error: OBJLoader -> Could not open file '%s'", file_name);
	}

	while (std::getline(in_file, line)) {
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "v") { // Vertex Pos
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			positions.push_back(temp_vec3);
		}
		else if (prefix == "vt") { // Vertex Texcoord
			ss >> temp_vec2.x >> temp_vec2.y;
			texcoords.push_back(temp_vec2);
		}
		else if (prefix == "vn") { // Vertex Normals
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			normals.push_back(temp_vec3);
		}
		else if (prefix == "f") {
			int counter = 0;
			while (ss >> temp_glint) {
				// adding indices
				if (counter == 0)
					position_indices.push_back(temp_glint);
				else if (counter == 1)
					texcoord_indices.push_back(temp_glint);
				else if (counter == 2)
					normal_indices.push_back(temp_glint);

				if (ss.peek() == '/') {
					counter++;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ') {
					counter++;
					ss.ignore(1, ' ');
				}

				if (counter > 2)
					counter = 0;
			}
		}
		else if (prefix == "#") { // Comment

		}
		else if (prefix == "o") {

		}
		else if (prefix == "s") {

		}
		else if (prefix == "use_mtl") {

		}
		else { }
	}

	vertices.resize(position_indices.size(), Vertex());
	for (size_t i = 0; i < position_indices.size(); i++) {
		vertices[i].position = positions[position_indices[i] - 1];
		vertices[i].texcoord = texcoords[texcoord_indices[i] - 1];
		vertices[i].normal = normals[normal_indices[i] - 1];
		vertices[i].color = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	FDEBUG("Loading a file with %i vertices.", vertices.size());
	FINFO("Successfully loaded model '%s'", file_name);

	return vertices;
}