#include "Game.h"

#include <fstream>
#include <vector>

const char* CURRENT_LEVEL = "debug_level";
// TODO: Fix loading models from json

Game::Game(const char* title, const int width, const int height, int GLmajorVer, int GLminorVer, bool resizable)
	: m_windowTitle(title), m_windowResizable(resizable),
	WINDOW_WIDTH(width), WINDOW_HEIGHT(height), GL_VER_MAJOR(GLmajorVer), GL_VER_MINOR(GLminorVer),
	camera(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f))
{
	FINFO("Loading data from JSON...");
	std::fstream frdataFile("resources/alyssa/frdata.archive");
	frdata = json::parse(frdataFile);
	std::fstream levelFile("resources/alyssa/level.archive");
	leveldata = json::parse(levelFile);

	this->window = nullptr;
	this->frameBufferHeight = WINDOW_HEIGHT;
	this->frameBufferWidth = WINDOW_WIDTH;


	this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->camFront = glm::vec3(0.0f, 0.0f, -1.0f);
	this->camPosition = glm::vec3(0.0f, 0.0f, 1.0f);
	this->fov = 90.0f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.0f;

	this->dt = 0.0f;
	this->curTime = 0.0f;
	this->lastTime = 0.0f;

	this->last_mouse_x = 0.0f;
	this->last_mouse_y = 0.0f;
	this->mouse_x = 0.0f;
	this->mouse_y = 0.0f;
	this->mouse_offset_x = 0.0f;
	this->mouse_offset_y = 0.0f;
	this->first_mouse = true;

	this->initialize_glfw();
	this->initialize_window();
	this->initialize_glew();
	this->initialize_opengl_options();
	this->editorInterface.initialize(this->window);

	this->initialize_matrices();
	this->initialize_shaders();
	this->initialize_textures();
	this->initialize_materials();
	this->initialize_obj_models();
	this->initialize_lights();
	this->initialize_models();
	this->initialize_uniforms();
}

Game::~Game() 
{
	shutdown();
}

// Accessor
int Game::get_window_should_close() 
{
	return glfwWindowShouldClose(this->window);
}

// Modifie
void Game::set_window_should_close() 
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

void Game::update_delta() 
{
	this->curTime = static_cast<float>(glfwGetTime());
	this->dt = this->curTime - this->lastTime;
	this->lastTime = this->curTime;
}

void Game::update_keyboard_input() 
{
	// Camera movement
	if (is_key_pressed(GLFW_KEY_W)) this->camera.move(this->dt, FWD);
	if (is_key_pressed(GLFW_KEY_S)) this->camera.move(this->dt, BCK);
	if (is_key_pressed(GLFW_KEY_A)) this->camera.move(this->dt, LEFT);
	if (is_key_pressed(GLFW_KEY_D)) this->camera.move(this->dt, RIGHT);
	if (is_key_pressed(GLFW_KEY_SPACE)) this->camera.move(this->dt, UP);
	if (is_key_pressed(GLFW_KEY_LEFT_CONTROL)) this->camera.move(this->dt, DOWN);
	//if (glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT)) this->camera.boost_speed();

	if (is_key_pressed(GLFW_KEY_ESCAPE)) 
	{
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

static void backToGame(GLFWwindow* win)
{
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::update_mouse_input() 
{
	glfwGetCursorPos(this->window, &this->mouse_x, &this->mouse_y);

	if (this->first_mouse) 
	{
		this->last_mouse_x = mouse_x;
		this->last_mouse_y = mouse_y;
		this->first_mouse = false;
	}

	this->mouse_offset_x = this->mouse_x - this->last_mouse_x;
	this->mouse_offset_y = this->mouse_y - this->last_mouse_y;

	this->last_mouse_x = this->mouse_x;
	this->last_mouse_y = this->mouse_y;
}

void Game::update_input() 
{
	glfwPollEvents();
	this->update_keyboard_input();
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		this->update_mouse_input();
}

// Functions
void Game::update() 
{
	this->update_delta();
	this->update_input();
	this->camera.update_input(this->dt, -1, this->mouse_offset_x, this->mouse_offset_y);
}



void Game::render() 
{

	for (ShadowMapHandler* smh : shadow_maps) {
		smh->pre_render();
		smh->render(this->meshes, this->camera);
	}

	lighting_pass();
}

void Game::shutdown()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();

	for (size_t i = 0; i < this->shaders.size(); i++) delete this->shaders[i];
	for (size_t i = 0; i < this->textures.size(); i++) delete this->textures[i];
	for (size_t i = 0; i < this->materials.size(); i++) delete this->materials[i];
	// for (size_t i = 0; i < this->meshes.size(); i++) delete this->meshes[i];
	for (size_t i = 0; i < this->lights.size(); i++) delete this->lights[i];
	for (size_t i = 0; i < this->point_lights.size(); i++) delete this->point_lights[i];

	for (auto*& i : this->models) delete i;
}

void Game::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}

void Game::lighting_pass()
{
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	this->editorInterface.update();

	// Update uniforms
	this->update_uniforms();
	for (auto& model : this->models)
	{
		model->render(this->shaders[SHADER_CORE_PROGRAM]);
	}
	for (auto& plLightModel : this->point_lights)
	{
		plLightModel->render(this->shaders[SHADER_CORE_PROGRAM]);
	}

	this->editorInterface.render();

	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::initialize_glfw() {
	FINFO("Manager > Initializing GLFW...");
	if (glfwInit() == GLFW_FALSE) 
	{
		FFATAL("Manager > Error: GLFW initialization failed!");
		this->shutdown();
	}
}

void Game::initialize_window()
{

	FINFO("Manager > Creating a window with OpenGL version %i.%i", GL_VER_MAJOR, GL_VER_MINOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VER_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VER_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, m_windowResizable);


	this->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, m_windowTitle, NULL, NULL);
	glfwGetFramebufferSize(this->window, &frameBufferWidth, &frameBufferHeight);
	glfwSetFramebufferSizeCallback(this->window, Game::framebuffer_resize_callback);

	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwMakeContextCurrent(this->window);
	if (this->window == nullptr) 
	{
		FFATAL("Manager > Error: OpenGL has failed to create the window.");
		this->shutdown();
	}
}

void Game::initialize_glew() 
{
	glewExperimental = TRUE;
	if (glewInit() != GLEW_OK) 
	{
		FFATAL("Manager > Error: Engine was unable to initialize GLEW32!");
		this->shutdown();
	}
}

void Game::initialize_opengl_options() 
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (editorInterface.graphicsOptions.wireframeMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		FINFO("Manager > Wireframe mode enabled!");
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		FINFO("Manager > Wireframe mode is disabled.");
	}

	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(this->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void Game::initialize_matrices() 
{
	FINFO("Manager > Initializing matrices...");
	this->viewMatrix = glm::mat4(1.0f);
	this->viewMatrix = camera.get_view_matrix();

	this->projectionMatrix = glm::mat4(1.0f);
	this->projectionMatrix = glm::perspective(glm::radians(this->fov),
		static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight,
		this->nearPlane,
		this->farPlane);
}

void Game::initialize_shaders() 
{
	FINFO("Manager > Initializing shaders...");
	std::string coreVert = leveldata[CURRENT_LEVEL]["data"]["shader"]["core"]["vertex"];
	std::string coreFrag = leveldata[CURRENT_LEVEL]["data"]["shader"]["core"]["fragment"];
	this->shaders.push_back(new Shader(GL_VER_MAJOR, GL_VER_MINOR, coreVert.c_str(), coreFrag.c_str()));;
}

void Game::initialize_textures() 
{
	FINFO("Manager > Initializing textures...");
	std::vector<json> vec = leveldata[CURRENT_LEVEL]["data"]["textures"];
	for (json dataObject : vec) {
		std::string path = dataObject["path"];
		this->textures.push_back(new Texture(path.c_str(), GL_TEXTURE_2D));
	}
}

void Game::initialize_materials() 
{
	FINFO("Initializing materials...");
	std::vector<json> vec = leveldata[CURRENT_LEVEL]["data"]["materials"];
	for (json dataObject : vec) {
		glm::vec3 diffuse(dataObject["diffuse"][0], dataObject["diffuse"][1], dataObject["diffuse"][2]);
		glm::vec3 specular(dataObject["specular"][0], dataObject["specular"][1], dataObject["specular"][2]);
		glm::vec3 ambient(dataObject["ambient"][0], dataObject["ambient"][1], dataObject["ambient"][2]);
		int diffuseTex = dataObject["diffuseTexture"];
		int specularTex = dataObject["specularTexture"];

		this->materials.push_back(new Material(diffuse, specular, ambient, diffuseTex, specularTex));
	}
}

void Game::initialize_obj_models()
{
	FINFO("Loading 3D Models...");
	json modelsData = leveldata[CURRENT_LEVEL]["data"]["models"];
	for (auto it = modelsData.begin(); it != modelsData.end(); ++it) {
		std::string id = it.key();
		json data = it.value();

		glm::vec3 position(data["position"][0], data["position"][1], data["position"][2]);
		int materialIndex = data["materialIndex"];
		int overrideTexDif = data["overrideTextureDiffuse"];
		int overrideTexSpec = data["overrideTextureSpecular"];

		std::string objectPath = data["objectPath"];
		if (objectPath == "MESH_BASED") {
			std::vector<Mesh*> meshList;

			for (json meshDat : data["meshes"]) {
				std::string type = meshDat["type"];

				if (type == "QUAD") {
					auto quadTemp = Quad();
					meshList.push_back(
						new Mesh(&quadTemp,
							glm::vec3(meshDat["position"][0], meshDat["position"][1], meshDat["position"][2]),
							glm::vec3(meshDat["rotation"][0], meshDat["rotation"][1], meshDat["rotation"][2]),
							glm::vec3(meshDat["scale"][0], meshDat["scale"][1], meshDat["scale"][2])
						));
				}
				// TODO: Add other mesh types and implement an enum to use a switch instead of if statements
			}
			Model* model = new Model(position,
				this->materials[materialIndex],
				this->textures[overrideTexDif],
				this->textures[overrideTexSpec],
				meshList
			);
			if (data.contains("rotation"))
				model->rotate(glm::vec3(data["rotation"][0], data["rotation"][1], data["rotation"][2]));
			if (data.contains("scale"))
				model->scale(glm::vec3(data["scale"][0], data["scale"][1], data["scale"][2]));

			this->models.push_back(model);
			this->model_map.insert(std::pair(id, model));
			
			for (auto*& i : meshList)
				delete i;
			meshList.clear();
		}
		else {
			Model* model = new Model(position,
				this->materials[materialIndex],
				this->textures[overrideTexDif],
				this->textures[overrideTexSpec],
				objectPath.c_str());
			
			if (data.contains("rotation"))
				model->rotate(glm::vec3(data["rotation"][0], data["rotation"][1], data["rotation"][2]));
			if (data.contains("scale"))
				model->scale(glm::vec3(data["scale"][0], data["scale"][1], data["scale"][2]));

			this->models.push_back(model);
			this->model_map.insert(std::pair(id, model));
		}
	}
}

void Game::initialize_models() 
{

}

void Game::initialize_point_lights()
{
	std::string shadowVert = leveldata[CURRENT_LEVEL]["data"]["shader"]["shadowDepth"]["vertex"];
	std::string shadowFrag = leveldata[CURRENT_LEVEL]["data"]["shader"]["shadowDepth"]["fragment"];
	std::string shadowGeo = leveldata[CURRENT_LEVEL]["data"]["shader"]["shadowDepth"]["geometry"];

	for (json plData : leveldata[CURRENT_LEVEL]["data"]["pointLights"]) {
		glm::vec3 position(plData["position"][0], plData["position"][1], plData["position"][2]);
		bool blinn = plData["blinnPhong"];
		float intensity = plData["intensity"];
		glm::vec3 color(plData["color"][0], plData["color"][1], plData["color"][2]);
		float constant = plData["constant"];
		float quadratic = plData["quadratic"];
		float linear = plData["linear"];

		PointLight* pl = new PointLight(position, blinn, intensity, color, constant, linear, quadratic);
		if (plData.contains("children")) {
			for (std::string childName : plData["children"]) {
				pl->add_child(model_map[childName]);
			}
		}
		if (!plData.contains("castShadows")) {
			FERROR("Missing property in PointLight 'castShadows'!");
		}

		bool castShadows = plData["castShadows"];
		if (castShadows) {
			ShadowMapHandler shadowMapHandler(shadowVert.c_str(), shadowFrag.c_str(), shadowGeo.c_str());
			shadowMapHandler.initialize(pl->position);
			shadow_maps.push_back(&shadowMapHandler);
		}

		this->point_lights.push_back(pl);
		this->editorInterface.add_point_light(pl);
	}

/*
	PointLight* pl = new PointLight(glm::vec3(0.0f), true, 1.0f, glm::vec3(1.0f), 2.0f);

	Model* model = new Model(glm::vec3(0.0f),
		this->materials[MATERIAL_DEFAULT],
		this->textures[TEXTURE_LIGHTBULB],
		this->textures[TEXTURE_CONTAINER],
		"resources/3d/sphere.obj");
	model->scale(0.5f);
	model->rotate(glm::vec3(-180, 0, 0));

	// pl->add_child(model);
	ShadowMapHandler shadowMapHandler(
		"resources/shaders/shadow_map_vertex.glsl",
		"resources/shaders/shadow_map_fragment.glsl",
		"resources/shaders/shadow_map_geometry.glsl");
	shadowMapHandler.initialize(pl->position);
	shadow_maps.push_back(&shadowMapHandler);

	this->point_lights.push_back(pl);
	this->editorInterface.add_point_light(pl);*/
}

void Game::initialize_lights() 
{
	FINFO("Manager > Initializing lights...");
	initialize_point_lights();
	this->lights.push_back(new glm::vec3(0.0f, 0.0f, 1.0f));
}

void Game::initialize_uniforms() 
{
	FINFO("Manager > Initializing uniforms...");
	this->shaders[SHADER_CORE_PROGRAM]->set_mat4fv("viewMatrix", viewMatrix);
	this->shaders[SHADER_CORE_PROGRAM]->set_mat4fv("projectionMatrix", projectionMatrix);


	for (auto& pl : this->point_lights) 
	{
		pl->send_to_shader(*this->shaders[SHADER_CORE_PROGRAM]);
	}
	// this->shaders[SHADER_CORE_PROGRAM]->setVec3f("lightPos0", *this->lights[0]);
}

void Game::update_uniforms() 
{
	this->materials[MATERIAL_DEFAULT]->send_to_shader(*this->shaders[SHADER_CORE_PROGRAM]);

	// Update frame buffer size and projection matrix
	glfwGetFramebufferSize(this->window, &frameBufferWidth, &frameBufferHeight);

	this->viewMatrix = this->camera.get_view_matrix();
	this->shaders[SHADER_CORE_PROGRAM]->set_mat4fv("viewMatrix", this->viewMatrix);
	this->shaders[SHADER_CORE_PROGRAM]->set_vec3f("cameraPos", camera.getPosition());
	this->shaders[SHADER_CORE_PROGRAM]->set_1f("far_plane", camera.get_far_plane());
	this->shaders[SHADER_CORE_PROGRAM]->set_1f("useShadows", editorInterface.graphicsOptions.castShadows);
	for (auto& pl : this->point_lights) 
	{
		pl->send_to_shader(*this->shaders[SHADER_CORE_PROGRAM]);
	}
	// this->shaders[SHADER_CORE_PROGRAM]->set_vec3f("lightPos0", *this->lights[0]);

	this->projectionMatrix = glm::perspective(glm::radians(fov),
		static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight,
		this->nearPlane,
		this->farPlane);
	this->shaders[SHADER_CORE_PROGRAM]->set_mat4fv("projectionMatrix", this->projectionMatrix);
}

bool Game::is_key_pressed(GLenum key) 
{
	return glfwGetKey(this->window, key) == GLFW_PRESS;
}
