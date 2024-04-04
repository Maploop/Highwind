#pragma once

#include "defines.h"

// base
class Light {
protected:
	float intensity;
	glm::vec3 color;

public:
	Light(float intensity, glm::vec3 color) : intensity(intensity), color(color) {

	}

	~Light() {

	}

	virtual void sendToShader(Shader& program) = 0;
};

class PointLight : public Light {
protected:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;

public:
	PointLight(glm::vec3 position, float intensity = 1.0f, glm::vec3 color = glm::vec3(1.0f), float constant = 1.0f, float linear = 0.045f, float quadratic = 0.0075f)
		: Light(intensity, color), position(position), constant(constant), linear(linear), quadratic(quadratic) {

	}

	~PointLight() {

	}

	void setPosition(const glm::vec3 pos) {
		this->position = pos;
	}

	void sendToShader(Shader& program) {
		program.setVec3f("pointLight.position", position);
		program.set1f("pointLight.intensity", intensity);
		program.setVec3f("pointLight.color", color);
		program.set1f("pointLight.constant", constant);
		program.set1f("pointLight.linear", linear);
		program.set1f("pointLight.quadratic", quadratic);
	}
};