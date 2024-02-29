#pragma once

#include <iostream>
#include <vector>

#include "Material.h"
#include "Vertex.h"
#include "Primitives.h"

class Mesh {
public:
	Mesh(
		const Vertex* vertices, const unsigned nrOfVertices, const GLuint* indices, const unsigned nrOfIndices,

		glm::vec3 postion = glm::vec3(0.0f),
		glm::vec3 rotation = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f)) {

		m_position = postion;
		m_rotation = rotation;
		m_scale = scale;

		this->m_nrOfVertices = nrOfVertices;
		this->m_nrOfIndices = nrOfIndices;
		this->vertexArray = new Vertex[this->m_nrOfVertices];
		for (size_t i = 0; i < m_nrOfVertices; i++) {
			this->vertexArray[i] = vertices[i];
		}
		this->indexArray = new GLuint[this->m_nrOfIndices];
		for (size_t i = 0; i < m_nrOfIndices; i++) {
			this->indexArray[i] = indices[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	}

	Mesh(
		Primitive* prim,

		glm::vec3 postion = glm::vec3(0.0f),
		glm::vec3 rotation = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f)) {

		m_position = postion;
		m_rotation = rotation;
		m_scale = scale;

		this->m_nrOfVertices = prim->get_nr_of_vertices();
		this->m_nrOfIndices = prim->get_nr_of_indices();
		this->vertexArray = new Vertex[this->m_nrOfVertices];
		for (size_t i = 0; i < m_nrOfVertices; i++) {
			this->vertexArray[i] = prim->get_vertices()[i];
		}
		this->indexArray = new GLuint[this->m_nrOfIndices];
		for (size_t i = 0; i < m_nrOfIndices; i++) {
			this->indexArray[i] = prim->get_indices()[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	}

	Mesh(const Mesh& obj) {
		m_position = obj.m_position;
		m_rotation = obj.m_rotation;
		m_scale = obj.m_scale;

		this->m_nrOfVertices = obj.m_nrOfVertices;
		this->m_nrOfIndices = obj.m_nrOfIndices;
		this->vertexArray = new Vertex[this->m_nrOfVertices];
		for (size_t i = 0; i < m_nrOfVertices; i++) {
			this->vertexArray[i] = obj.vertexArray[i];
		}
		this->indexArray = new GLuint[this->m_nrOfIndices];
		for (size_t i = 0; i < m_nrOfIndices; i++) {
			this->indexArray[i] = obj.indexArray[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	}

	~Mesh() {
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		if (m_nrOfIndices > 0) {
			glDeleteBuffers(1, &m_EBO);
		}

		delete[] this->vertexArray;
		delete[] this->indexArray;
	}

	void update() {

	}

	void render(Shader* shader) {
		// Update uniforms
		this->updateModelMatrix();
		this->update_uniforms(shader);
		shader->use();

		// Bind VAO
		glBindVertexArray(m_VAO);
		// Render
		if (m_nrOfIndices == 0)
			glDrawArrays(GL_TRIANGLES, 0, m_nrOfVertices);
		else
			glDrawElements(GL_TRIANGLES, m_nrOfIndices, GL_UNSIGNED_INT, 0);

		// cleanup
		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Functions
	void move(const glm::vec3 pos) {
		m_position += pos;
	}

	void rotate(const glm::vec3 rot) {
		m_rotation += rot;
	}

	void scale(const glm::vec3 scale) {
		m_scale += scale;
	}
	// Accessors

	// Modifiers
	void setPosition(const glm::vec3 pos) {
		m_position = pos;
	}

	void setRotation(const glm::vec3 rot) {
		m_rotation = rot;
	}

	void setScale(const glm::vec3 scale) {
		m_scale = scale;
	}
private:
	Vertex* vertexArray;
	unsigned m_nrOfVertices;
	GLuint* indexArray;
	unsigned m_nrOfIndices;
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	glm::mat4 m_modelMatrix;

	void initVAO() {
		// create vao
		glCreateVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_nrOfVertices * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW);

		if (m_nrOfIndices > 0) {
			glGenBuffers(1, &m_EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nrOfIndices * sizeof(GLuint), this->indexArray, GL_STATIC_DRAW);
		}

		// g_uint32 attribLoc = glGetAttribLocation(core_program, "vertex_position");
		// position attrib
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		// color attrib
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		// texcoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);
		// normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);
	}

	void updateModelMatrix() {
		m_modelMatrix = glm::mat4(1.0f);
		m_modelMatrix = glm::translate(m_modelMatrix, m_position);
		m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
	}

	void update_uniforms(Shader* shader) {
		shader->setMat4fv("modelMatrix", m_modelMatrix);
	}
};