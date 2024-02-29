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
#include "Shader.h"

class Material {
public:
	Material(glm::vec3 ambient,
	glm::vec3 diffuse,
	glm::vec3 specular,

	GLint diffuseTex,
	GLint specularTex) {
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->diffuseTex = diffuseTex;
		this->specularTex = specularTex;
	}

	~Material() { }

	void send_to_shader(Shader& shader) {
		shader.setVec3f("material.ambient", this->ambient);
		shader.setVec3f("material.diffuse", this->diffuse);
		shader.setVec3f("material.specular", this->specular);
		shader.set1i("material.diffuseTex", this->diffuseTex);
		shader.set1i("material.specularTex", this->specularTex);
	}
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	GLint diffuseTex;
	GLint specularTex;
};