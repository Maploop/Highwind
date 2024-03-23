#pragma once

#include "defines.h"
#include "Camera.h"
#include "OBJLoader.h"

class Environment {
private:
    char* codename;
    std::vector<Model*>* models;

public:
    void self_initialize();
    void initShaders(std::vector<Shader*>* shaders);
	void initTextures(std::vector<Texture*>* textures);
	void initMaterials(std::vector<Material*>* materials);
	void initOBJModels();
	void initModels(std::vector<Model*>* models);
	void initLights(std::vector<glm::vec3*>* positions);

	void initUniforms();
	void updateUniforms();
};