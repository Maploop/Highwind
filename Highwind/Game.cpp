#include "Game.h"

Game::Game(const char* title, const int width, const int height, int GLmajorVer, int GLminorVer, bool resizable) 
	: m_windowTitle(title), m_windowResizable(resizable),
	WINDOW_WIDTH(width), WINDOW_HEIGHT(height), GL_VER_MAJOR(GLmajorVer), GL_VER_MINOR(GLminorVer), 
	camera(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) {
	
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

	this->lastMouseX = 0.0f;
	this->lastMouseY = 0.0f;
	this->mouseX = 0.0f;
	this->mouseY = 0.0f;
	this->mouseOffsetX = 0.0f;
	this->mouseOffsetY = 0.0f;
	this->firstMouse = true;

	this->initGlfw();
	this->initWindow();
	this->initGlew();
	this->initOpenGLOptions();
	this->initMatrices();

	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initOBJModels();
	this->initLights();
	this->initModels();
	this->initUniforms();
}

Game::~Game() {
	shutdown();
}

// Accessor
int Game::getWindowShouldClose() {
	return glfwWindowShouldClose(this->window);
}

// Modifier
void Game::setWindowShouldClose() {
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

void Game::updateDelta() {
	this->curTime = static_cast<float>(glfwGetTime());
	this->dt = this->curTime - this->lastTime;
	this->lastTime = this->curTime;
}

void Game::updateKeyboardInput() {
	// Camera movement
	if (isKeyPressed(GLFW_KEY_W)) this->camera.move(this->dt, FWD);
	if (isKeyPressed(GLFW_KEY_S)) this->camera.move(this->dt, BCK);
	if (isKeyPressed(GLFW_KEY_A)) this->camera.move(this->dt, LEFT);
	if (isKeyPressed(GLFW_KEY_D)) this->camera.move(this->dt, RIGHT);
	if (isKeyPressed(GLFW_KEY_SPACE)) this->camera.move(this->dt, UP);
	if (isKeyPressed(GLFW_KEY_LEFT_CONTROL)) this->camera.move(this->dt, DOWN);

	if (isKeyPressed(GLFW_KEY_ESCAPE)) {
		setWindowShouldClose();
	}

	if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		this->pointLights[0]->setPosition(this->camera.getPosition());
	}
}

void Game::updateMouseInput() {
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if (this->firstMouse) {
		this->lastMouseX = mouseX;
		this->lastMouseY = mouseY;
		this->firstMouse = false;
	}

	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->mouseY - this->lastMouseY;

	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;
}

void Game::updateInput() {
	glfwPollEvents();
	this->updateKeyboardInput();
	this->updateMouseInput();
}

// Functions
void Game::update() {
	this->updateDelta();
	this->updateInput();
	this->camera.updateInput(this->dt, -1, this->mouseOffsetX, this->mouseOffsetY);
}

void Game::render() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Update uniforms
	this->updateUniforms();
	for (auto& model : this->models) {
		model->render(this->shaders[SHADER_CORE_PROGRAM]);
	}
	for (auto& plLightModel : this->pointLights) {
		plLightModel->render(this->shaders[SHADER_CORE_PROGRAM]);
	}

	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::shutdown() {
	glfwDestroyWindow(this->window);
	glfwTerminate();

	for (size_t i = 0; i < this->shaders.size(); i++) delete this->shaders[i];
	for (size_t i = 0; i < this->textures.size(); i++) delete this->textures[i];
	for (size_t i = 0; i < this->materials.size(); i++) delete this->materials[i];
	// for (size_t i = 0; i < this->meshes.size(); i++) delete this->meshes[i];
	for (size_t i = 0; i < this->lights.size(); i++) delete this->lights[i];
	for (size_t i = 0; i < this->pointLights.size(); i++) delete this->pointLights[i];

	for (auto*& i : this->models) delete i;
}

void Game::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}

void Game::initGlfw() {
	FINFO("Initializing GLFW...");
	if (glfwInit() == GLFW_FALSE) {
		FFATAL("GLFW initialization failed!");
		this->shutdown();
	}
}

void Game::initWindow() {

	FINFO("Creating a window with OpenGL version %i.%i", GL_VER_MAJOR, GL_VER_MINOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VER_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VER_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, m_windowResizable);


	this->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, m_windowTitle, NULL, NULL);
	glfwGetFramebufferSize(this->window, &frameBufferWidth, &frameBufferHeight);
	glfwSetFramebufferSizeCallback(this->window, Game::framebuffer_resize_callback);

	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwMakeContextCurrent(this->window);
	if (this->window == nullptr) {
		FFATAL("OpenGL has failed to create the window.");
		this->shutdown();
	}
}

void Game::initGlew() {
	glewExperimental = TRUE;
	if (glewInit() != GLEW_OK) {
		FFATAL("Engine was unable to initialize GLEW32!");
		this->shutdown();
	}
}

void Game::initOpenGLOptions() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef MODE_WIREFRAME
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	FINFO("Wireframe mode enabled!");
#else
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	FINFO("Wireframe mode is disabled.");
#endif

	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::initMatrices() {
	FINFO("Initializing matrices...");
	this->viewMatrix = glm::mat4(1.0f);
	this->viewMatrix = camera.getViewMatrix();

	this->projectionMatrix = glm::mat4(1.0f);
	this->projectionMatrix = glm::perspective(glm::radians(this->fov),
		static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight,
		this->nearPlane,
		this->farPlane);
}

void Game::initShaders() {
	FINFO("Initializing shaders...");
	this->shaders.push_back(new Shader(GL_VER_MAJOR, GL_VER_MINOR, "resources/shaders/vertex_core.glsl", "resources/shaders/fragment_core.glsl"));
}

void Game::initTextures() {
	FINFO("Initializing textures...");
	this->textures.push_back(new Texture("resources/textures/container2.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("resources/textures/awesomeface.png", GL_TEXTURE_2D));
}

void Game::initMaterials() {
	FINFO("Initializing materials...");
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), 0, 1));
}

void Game::initOBJModels()
{
	FINFO("Loading 3D Models...");
	// std::vector<Vertex> mesh = loadObjFile("resources/3d/monkey.obj");

	std::vector<Mesh*> meshes;

	// meshes.push_back(new Mesh(mesh.data(), mesh.size(), NULL, 0, glm::vec3(1.0, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));

	this->models.push_back(new Model(glm::vec3(0.0f, 0.0f, 0.0f),
		this->materials[MAT_1],
		this->textures[TEX_CONTAINER_0],
		this->textures[TEX_AWESOMEFACE_1],
		"resources/3d/monkey.obj"
	));

	auto quadTemp = Quad();
	meshes.push_back(
		new Mesh(&quadTemp,
			glm::vec3(0.0f, -3.0f, 0.0f),
			glm::vec3(-90.0f, 0.0f, 0.0f),
			glm::vec3(100.0f)
		));

	this->models.push_back(new Model(glm::vec3(0.0f),
		this->materials[MAT_1],
		this->textures[TEX_CONTAINER_0],
		this->textures[TEX_AWESOMEFACE_1],
		meshes
	));

	for (auto*& i : meshes)
		delete i;
	meshes.clear();
}

void Game::initModels() {
	
}

void Game::initPointLights()
{
	PointLight* pl = new PointLight(glm::vec3(0.0f));

	Model* model = new Model(glm::vec3(0.0f),
		this->materials[MAT_1],
		new Texture("resources/textures/lightbulb.jpg", GL_TEXTURE_2D),
		this->textures[TEX_CONTAINER_0],
		"resources/3d/sphere.obj");
	model->scale(0.5f);
	model->rotate(glm::vec3(-180, 0, 0));
	
	pl->setLightModel(model);

	this->pointLights.push_back(pl);
}

void Game::initLights() {
	FINFO("Initializing lights...");
	initPointLights();
	this->lights.push_back(new glm::vec3(0.0f, 0.0f, 1.0f));
}

void Game::initUniforms() {
	FINFO("Initializing uniforms...");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv("viewMatrix", viewMatrix);
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv("projectionMatrix", projectionMatrix);

	for (auto& pl : this->pointLights) {
		pl->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
	}
	// this->shaders[SHADER_CORE_PROGRAM]->setVec3f("lightPos0", *this->lights[0]);
}

void Game::updateUniforms() {
	this->materials[MAT_1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);

	// Update frame buffer size and projection matrix
	glfwGetFramebufferSize(this->window, &frameBufferWidth, &frameBufferHeight);

	this->viewMatrix = this->camera.getViewMatrix();
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv("viewMatrix", this->viewMatrix);
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f("cameraPos", camera.getPosition());
	for (auto& pl : this->pointLights) {
		pl->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
	}
	//this->shaders[SHADER_CORE_PROGRAM]->setVec3f("lightPos0", *this->lights[0]);

	this->projectionMatrix = glm::perspective(glm::radians(fov),
		static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight,
		this->nearPlane,
		this->farPlane);
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv("projectionMatrix", this->projectionMatrix);
}

bool Game::isKeyPressed(GLenum key) {
	return glfwGetKey(this->window, key) == GLFW_PRESS;
}
