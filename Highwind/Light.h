#pragma once

#include "defines.h"

class Light : public Component
{
protected:
	float intensity;
	
public:
	glm::vec3 color;
	Light(float intensity, glm::vec3 color) : intensity(intensity), color(color) 
	{

	}

	~Light() {

	}

	void render(Shader* shader)
	{
		this->render_children(shader);
	}

	void set_color(glm::vec3 c) 
	{
		this->color = color;
	}

	virtual void send_to_shader(Shader& program) = 0;
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

	void set_position(const glm::vec3 pos) 
	{
		this->position = pos;
		for (auto& child : get_children())
		{
			child->set_position(pos);
		}
	}

	void on_child_added(Component* child)
	{
		child->set_position(this->position);
	}

	void set_blinn(bool blinn)
	{
		this->blinn = blinn;
	}

	void send_to_shader(Shader& program) 
	{
		for (auto& child : get_children())
		{
			child->set_position(position);
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