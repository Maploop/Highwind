#pragma once

#include "Mesh.h"

class Model {
private:
	Material* material;
	Texture* overrideTextureDiffuse;
	Texture* overrideTextureSpecular;
	std::vector<Mesh*> meshes;

	glm::vec3 position;

	void updateUniforms() {

	}
public:
	Model(glm::vec3 position, Material* material, Texture* overrideTexDif, Texture* overrideTexSpec, std::vector<Mesh*> meshes) {
		this->position = position;
		this->material = material;
		this->overrideTextureDiffuse = overrideTexDif;
		this->overrideTextureSpecular = overrideTexSpec;

		for (auto& i : meshes)
			this->meshes.push_back(new Mesh(*i));
	}
	~Model() {
		for (auto*& i : this->meshes)
			delete i;
	}

	// functions
	void rotate(glm::vec3 rotation) {
		for (auto& i : this->meshes)
			i->rotate(rotation);
	}

	void update() {

	}

	void render(Shader* shader) {
		this->updateUniforms();

		this->material->send_to_shader(*shader);
		// Use program
		shader->use();

		// Bind textures
		this->overrideTextureDiffuse->bind(0);
		this->overrideTextureSpecular->bind(1);

		// Draw
		for (auto& i : this->meshes)
			i->render(shader);
	}
};