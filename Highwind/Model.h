#pragma once

#include "Mesh.h"
#include "OBJLoader.hpp"

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

	// Obj file loaded stuff
	Model(glm::vec3 position, Material* material, Texture* overrideTexDif, Texture* overrideTexSpec, const char* obj_file) {
		this->position = position;
		this->material = material;
		this->overrideTextureDiffuse = overrideTexDif;
		this->overrideTextureSpecular = overrideTexSpec;

		std::vector<Vertex> mesh = loadObjFile(obj_file);
		this->meshes.push_back(new Mesh(mesh.data(), mesh.size(), NULL, 0, glm::vec3(1.0, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));

		for (auto& i : this->meshes) {
			i->move(this->position);
		}
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

		// Draw & Bind textures
		for (auto& i : this->meshes) {
			this->overrideTextureDiffuse->bind(0);
			this->overrideTextureSpecular->bind(1);

			i->render(shader);
		}
	}
};