#pragma once

#include "defines.h"
#include <SOIL2.h>
#include "glm/glm.hpp"

class SkyBoxManager {
public:
	SkyBoxManager(const char* vertexShader, const char* fragmentShader, std::vector<std::string> facesCubemap) 
		: skyboxShader(4, 5, vertexShader, fragmentShader), facesCubemap(facesCubemap) {
		skyboxShader.use();
		skyboxShader.set_1i("skybox", 0);
	}

	void initialize() noexcept {
		float skyboxVertices[] =
		{
			//   Coordinates
			-1.0f, -1.0f,  1.0f,//        7--------6
			 1.0f, -1.0f,  1.0f,//       /|       /|
			 1.0f, -1.0f, -1.0f,//      4--------5 |
			-1.0f, -1.0f, -1.0f,//      | |      | |
			-1.0f,  1.0f,  1.0f,//      | 3------|-2
			 1.0f,  1.0f,  1.0f,//      |/       |/
			 1.0f,  1.0f, -1.0f,//      0--------1
			-1.0f,  1.0f, -1.0f
		};

		unsigned int skyboxIndices[] =
		{
			// Right
			1, 2, 6,
			6, 5, 1,
			// Left
			0, 4, 7,
			7, 3, 0,
			// Top
			4, 5, 6,
			6, 7, 4,
			// Bottom
			0, 3, 2,
			2, 1, 0,
			// Back
			0, 1, 5,
			5, 4, 0,
			// Front
			3, 7, 6,
			6, 2, 3
		};

		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glGenBuffers(1, &skyboxEBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenTextures(1, &cubemapTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// These are very important to prevent seams
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		// This might help with seams on some systems
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		for (unsigned int i = 0; i < 6; i++) {
			int width, height, nrChannels;
			unsigned char* data = SOIL_load_image(facesCubemap[i].c_str(), &width, &height, &nrChannels, SOIL_LOAD_RGBA);
			if (data) {
				
				glTexImage2D
				(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0,
					GL_RGB,
					width,
					height,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					data
				);
				SOIL_free_image_data(data);
			}
			else {
				std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
				SOIL_free_image_data(data);
			}
		}
	}
	void render(Camera& camera, int width, int height, float fov, float near, float far) noexcept {
		glDepthFunc(GL_LEQUAL);

		skyboxShader.use();
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 projection = glm::mat4(1.0);
		view = glm::mat4(glm::mat3(glm::lookAt(camera.getPosition(), camera.getPosition() + camera.get_front(), camera.get_up())));
		projection = glm::perspective(glm::radians(fov), (float)width / height, near, far);
		skyboxShader.set_mat4fv("view", view);
		skyboxShader.set_mat4fv("projection", projection);

		// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
		// where an object is present (a depth of 1.0f will always fail against any object's depth value)
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}
	void free() noexcept {
		
	}

private:
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	unsigned int cubemapTexture;

	std::vector<std::string> facesCubemap;

	Shader skyboxShader;
};