#pragma once

#include "defines.h"

// base
class Light {
protected:
	float intensity;
	glm::vec3 color;
	Model* model = 0;

public:
	Light(float intensity, glm::vec3 color) : intensity(intensity), color(color) {

	}

	~Light() {

	}

	void render(Shader* shader) {
		if (model) {
			model->render(shader);
		}
	}

	void setLightModel(Model* m) {
		this->model = m;
		onLightModelSet();
	}

	virtual void sendToShader(Shader& program) = 0;
	virtual void onLightModelSet() = 0;
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
		if (model) {
			model->setPosition(pos);
		}
	}

	void onLightModelSet() {
		this->model->setPosition(this->position);
	}

	void sendToShader(Shader& program) {
		program.setVec3f("pointLight[0].position", position);
		program.set1f("pointLight[0].intensity", intensity);
		program.setVec3f("pointLight[0].color", color);
		program.set1f("pointLight[0].constant", constant);
		program.set1f("pointLight[0].linear", linear);
		program.set1f("pointLight[0].quadratic", quadratic);
	}
};