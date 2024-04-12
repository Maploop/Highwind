#pragma once

#include "defines.h"

class Light : public Component
{
protected:
	float intensity;
	glm::vec3 color;

public:
	Light(float intensity, glm::vec3 color) : intensity(intensity), color(color) 
	{

	}

	~Light() {

	}

	void render(Shader* shader)
	{
		this->renderAllChildren(shader);
	}

	virtual void sendToShader(Shader& program) = 0;
};

class PointLight : public Light 
{
protected:

public:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
	bool blinn;

	PointLight(glm::vec3 position, bool blinn = false, float intensity = 1.0f, glm::vec3 color = glm::vec3(1.0f), float constant = 1.0f, float linear = 0.045f, float quadratic = 0.0075f)
		: blinn(blinn), Light(intensity, color), position(position), constant(constant), linear(linear), quadratic(quadratic) 
	{

	}

	~PointLight() 
	{

	}

	void setPosition(const glm::vec3 pos) 
	{
		this->position = pos;
		for (auto& child : getChildren())
		{
			child->setPosition(pos);
		}
	}

	void onChildAdded(Component* child)
	{
		child->setPosition(this->position);
	}

	void setBlinn(bool blinn)
	{
		this->blinn = blinn;
	}

	void sendToShader(Shader& program) 
	{
		for (auto& child : getChildren())
		{
			child->setPosition(position);
		}

		program.set_vec3f("pointLight[0].position", position);
		program.set_1f("pointLight[0].intensity", intensity);
		program.set_vec3f("pointLight[0].color", color);
		program.set_1f("pointLight[0].constant", constant);
		program.set_1f("pointLight[0].linear", linear);
		program.set_1f("pointLight[0].quadratic", quadratic);
		program.set_1f("pointLight[0].blinn", blinn);
	}
};