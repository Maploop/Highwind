#pragma once

#include "Model.h"
#include "json.hpp"

using json = nlohmann::json;

class GLTFModel : public Model {
public:
	GLTFModel(glm::vec3 position, const char* fileName) {
		GLTFModel::file = fileName;
		Model::position = position;

		data = get_data();
		traverse_node(0);
	}

	~GLTFModel() {

	}

private:
	const char* file;
	std::vector<unsigned char> data;
	json JSON;

	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

	void load_mesh(unsigned int indMesh) {
		// Get all accessor indices
		unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
		unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
		unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
		unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

		// Use accessor indices to get all vertices components
		std::vector<float> posVec = get_floats(JSON["accessors"][posAccInd]);
		std::vector<glm::vec3> positions = GroupFloatsVec3(posVec);
		std::vector<float> normalVec = get_floats(JSON["accessors"][normalAccInd]);
		std::vector<glm::vec3> normals = GroupFloatsVec3(normalVec);
		std::vector<float> texVec = get_floats(JSON["accessors"][texAccInd]);
		std::vector<glm::vec2> texUVs = GroupFloatsVec2(texVec);

		// Combine all the vertex components and also get the indices and textures
		std::vector<Vertex> vertices = assemble_vertices(positions, normals, texUVs);
		std::vector<GLuint> indices = get_indices(JSON["accessors"][indAccInd]);
		std::vector<Texture> textures = get_textures();

		// Combine the vertices, indices, and textures into a mesh
		// TODO: FIX
		// meshes.push_back(Mesh(vertices, indices, textures));
	}

	void traverse_node(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f)) {

	}

	std::vector<unsigned char> get_data() {}
	std::vector<float> get_floats(json accessor) {}
	std::vector<GLuint> get_indices(json accessor) {}
	std::vector<Texture> get_textures() {}

	std::vector<Vertex> assemble_vertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs) {

	}

	std::vector<glm::vec2> GroupFloatsVec2(std::vector<float> floatVec) {
	}
	std::vector<glm::vec3> GroupFloatsVec3(std::vector<float> floatVec) {
	}
	std::vector<glm::vec4> GroupFloatsVec4(std::vector<float> floatVec) {
	}

	std::string get_file_contents(const char* pathName) const noexcept {

	}
};

