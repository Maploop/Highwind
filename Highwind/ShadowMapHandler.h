#pragma once

#include "Shader.h"
#include "glm/glm.hpp"
#include <vector>
#include "Model.h"
#include "Camera.h"

class ShadowMapHandler {
public:
	ShadowMapHandler(const char* vertexShader, const char* fragmentShader, const char* geometryShader);
	ShadowMapHandler() = default;
	~ShadowMapHandler();

	void initialize(glm::vec3 lightPosition);
	
	void pre_render() noexcept;
	void render(std::vector<Model*> modeList, Camera camera);

private:
	// Variables
	glm::mat4 lightSpaceMatrix;
	unsigned int shadowMap;
	unsigned int depthCubemap;

	glm::vec3 lightPosition;

	Shader shadowMapShader;

	unsigned int shadowMapFBO;
	unsigned int shadowMapWidth = 2048, shadowMapHeight = 2048;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
};

