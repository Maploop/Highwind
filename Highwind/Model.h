#pragma once

#include "Mesh.h"
#include "OBJLoader.hpp"
#include "Component.h"
#include "GlobalCache.h"

class Model : public Component {
private:
	Material* material;
	Texture* override_texture_diffuse;
	Texture* override_texture_specular;
	std::vector<Mesh*> meshes;

	glm::vec3 position;

	void update_uniforms() {

	}
public:
	Model(glm::vec3 position, Material* material, Texture* overrideTexDif, Texture* overrideTexSpec, std::vector<Mesh*> meshes) {
		this->position = position;
		this->material = material;
		this->override_texture_diffuse = overrideTexDif;
		this->override_texture_specular = overrideTexSpec;

		for (auto& i : meshes)
			this->meshes.push_back(new Mesh(*i));
	}

	// Obj file loaded stuff
	Model(glm::vec3 position, Material* material, Texture* overrideTexDif, Texture* overrideTexSpec, const char* obj_file) {
		this->position = position;
		this->material = material;
		this->override_texture_diffuse = overrideTexDif;
		this->override_texture_specular = overrideTexSpec;

		std::vector<Vertex> meshdata;
		try 
		{
			meshdata = load_from_obj(obj_file);
		}
		catch (std::exception ex)
		{
			FERROR("Complete failure to load a model, will be returning an empty array. (%s)", obj_file);
		}


		this->meshes.push_back(new Mesh(meshdata.data(), meshdata.size(), NULL, 0, glm::vec3(1.0, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
		

		for (auto& i : this->meshes) {
			i->move(this->position);
		}
	}

	~Model() {
		for (auto*& i : this->meshes)
			delete i;
	}

	void set_position(glm::vec3 newPos) {
		this->position = newPos;
		for (auto& i : this->meshes) {
			i->set_position(this->position);
		}
	}

	glm::vec3 getPosition()
	{
		return this->position;
	}

	void scale(glm::vec3 scale) {
		for (auto& i : this->meshes)
			i->set_scale(scale);
	}

	void scale(float scale) {
		this->scale(glm::vec3(scale));
	}

	// functions
	void rotate(glm::vec3 rotation) {
		for (auto& i : this->meshes)
			i->rotate(rotation);
	}

	void update() {

	}

	void render(Shader* shader) {
		this->update_uniforms();

		this->material->send_to_shader(*shader);
		// Use program
		shader->use();

		// Draw & Bind textures
		for (auto& i : this->meshes) {
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, material->get_diffuse_texture_unit());

			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, material->get_specular_texture_unit());

			if (override_texture_specular)
				this->override_texture_specular->bind(1);
			if (override_texture_diffuse)
				this->override_texture_diffuse->bind(0);

			i->render(shader);
		}
	}

	void move(glm::vec3 pos)
	{
		for (auto& i : this->meshes) {
			i->move(this->position);
		}
	}

	void component_render(Shader* shader)
	{
		this->render(shader);
	}
};