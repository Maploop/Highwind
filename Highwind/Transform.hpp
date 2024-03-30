#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transfrom {
public:
	Transfrom() {
		// Default transform values
		translate(glm::vec3(0, 0, 0));
		rotate(glm::vec3(0, 0, 0));
		scale(glm::vec3(1, 1, 1));
	}

	void translate(glm::vec3 pos) {
		transformMatrix = glm::translate(transformMatrix, pos);
	}

	void rotate(glm::vec3 rot) {
		transformMatrix = glm::rotate(transformMatrix, glm::radians(90.0f), rot);
	}

	void scale(glm::vec3 scale) {
		transformMatrix = glm::scale(transformMatrix, scale);
	}

	void scale(float scale) {
		this->scale(glm::vec3(scale, scale, scale));
	}

	glm::mat4 getMatrix() {
		return transformMatrix;
	}
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::mat4 transformMatrix;
};