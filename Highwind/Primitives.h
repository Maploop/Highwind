#pragma once

#include <vector>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include "Vertex.h"

class Primitive {
public:
	Primitive() {

	}
	virtual ~Primitive() {}

	void set(const Vertex* vertices, const unsigned nrOfVertices, const GLuint* indices, const unsigned nrOfIndices) {
		for (size_t i = 0; i < nrOfVertices; i++)
		{
			m_vertices.push_back(vertices[i]);
		}

		for (size_t i = 0; i < nrOfIndices; i++)
		{
			m_indices.push_back(indices[i]);
		}
	}

	inline Vertex* get_vertices() { return m_vertices.data(); }
	inline GLuint* get_indices() { return m_indices.data(); }
	inline const unsigned get_nr_of_vertices() { return m_vertices.size(); }
	inline const unsigned get_nr_of_indices() { return m_indices.size(); }
private:
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
};

class Quad : public Primitive {
public:
	Quad() : Primitive() {
		Vertex vertices[] = {
			// positins						// colors						// texcoords			// normals
			glm::vec3(-0.5f, 0.5f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f),	glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(-0.5f, -0.5f, 0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.5f, -0.5f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.5f, 0.5f, 0.0f),	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 1.0f),	glm::vec3(0.0f, 0.0f, 1.0f)
		};

		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] = {
			0, 1, 2, // tri #1
			0, 2, 3, // tri #2
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};

class Triangle : public Primitive {
public:
	Triangle() : Primitive() {
		Vertex vertices[] = {
			// positins						// colors						// texcoords			// normals
			glm::vec3(-0.5f, 0.5f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f),	glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(-0.5f, -0.5f, 0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.5f, -0.5f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.5f, 0.5f, 0.0f),	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 1.0f),	glm::vec3(0.0f, 0.0f, 1.0f)
		};

		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] = {
			0, 1, 2, // tri #1
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};

class Pyramid : public Primitive {
public:
	Pyramid() : Primitive() {
		Vertex vertices[] = {
			// positins						// colors						// texcoords			// normals
			glm::vec3(0.0f, 0.5f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.5f, 1.0f),	glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.5f, -0.5f, 0.5f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),

			glm::vec3(0.0f, 0.5f, 0.0f),	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(0.5f, 1.0f),	glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f),	glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 0.0f),	glm::vec3(-1.0f, 0.0f, 0.0f),

			glm::vec3(0.0f, 0.5f, 0.0f),	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(0.5f, 1.0f),	glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f),	glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, -1.0f),

			glm::vec3(0.0f, 0.5f, 0.0f),	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(0.5f, 1.0f),	glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.5f, -0.5f, 0.5f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f)
		};

		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		this->set(vertices, nrOfVertices, nullptr, 0);
	}
};