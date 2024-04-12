#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <vector>
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

		generate_depth_cubemap();
	}

	~Material() { }

	void send_to_shader(Shader& shader) {
		shader.set_vec3f("material.ambient", this->ambient);
		shader.set_vec3f("material.diffuse", this->diffuse);
		shader.set_vec3f("material.specular", this->specular);
		shader.set_1i("material.diffuseTex", this->diffuseTex);
		shader.set_1i("material.specularTex", this->specularTex);
	}

	//void startDepthFrame(Shader& depth, std::vector<PointLight*> lights)
	//{
	//	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, 0.0f, 25.0f);
	//	std::vector<glm::mat4> shadowTransforms;
	//	for (auto& pointLight : lights)
	//	{
	//		// glm::vec3 lightPos = pointLight.position;
	//		//shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	//		//shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	//		//shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	//		//shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	//		//shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	//		//shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	//	}
	//	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	//	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//	glClear(GL_DEPTH_BUFFER_BIT);
	//	depth.use();
	//	for (unsigned int i = 0; i < 6; ++i)
	//	{
	//		std::string s = "shadowMatrices[" + std::to_string(i) + "]";
	//		depth.setMat4fv(s.c_str(), shadowTransforms[i]);
	//	}
	//		
	//	depth.set1f("far_plane", 25.0f);
	//	depth.setVec3f("lightPos", lights[0]->position);
	//}

	void end()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLint get_diffuse_texture_unit() {
		return diffuseTex;
	}

	GLint get_specular_texture_unit() {
		return specularTex;
	}
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	GLint diffuseTex;
	GLint specularTex;
	GLuint depthMapFBO;
	GLuint depthCubemap;

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	void generate_depth_cubemap()
	{
		glGenFramebuffers(1, &depthMapFBO);

		glGenTextures(1, &depthCubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};