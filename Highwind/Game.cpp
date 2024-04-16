#include "Game.h"

Game::Game(const char* title, const int width, const int height, int GLmajorVer, int GLminorVer, bool resizable)
	: m_windowTitle(title), m_windowResizable(resizable),
	WINDOW_WIDTH(width), WINDOW_HEIGHT(height), GL_VER_MAJOR(GLmajorVer), GL_VER_MINOR(GLminorVer),
	camera(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) 
{

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
	this->initialize_imgui();

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

void Game::imgui_update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

bool render_shadows = true;

void Game::imgui_render()
{
	ImGui::Begin("Local PL");
	ImGui::Text("Transform ->");
	ImGui::DragFloat3("Light Position", (float*) & point_lights[0]->position, 0.2f);
	ImGui::Text("Point Light ->");
	ImGui::SliderFloat("Constant", &point_lights[0]->constant, 0.0f, 100.0f);
	ImGui::SliderFloat("Linear", &point_lights[0]->linear, 0.0f, 100.0f);
	ImGui::SliderFloat("Quadratic", &point_lights[0]->quadratic, 0.0f, 100.0f);
	ImGui::Checkbox("Blinn Phong", &point_lights[0]->blinn);
	ImGui::End();

	ImGui::Begin("Engine & Rendering Options");
	
	if (ImGui::Button("Compile & Run"))
	{
		FINFO("Compiling changes and running the game...");
		backToGame(this->window);
	}
	if (ImGui::Button("Exit"))
	{
		FINFO("Exiting...");
		set_window_should_close();
	}
	ImGui::Checkbox("Shadows", &render_shadows);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::render() 
{
	// shadow_map_pass();
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

void Game::shadow_map_pass()
{
	m_shadow_map_fbo.bind_for_writing();
	glClear(GL_DEPTH_BUFFER_BIT);
	
	
}

void Game::lighting_pass()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	this->imgui_update();

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

	this->imgui_render();

	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::initialize_imgui()
{
	FINFO("Initializing ImGUI...");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void Game::initialize_glfw() {
	FINFO("Initializing GLFW...");
	if (glfwInit() == GLFW_FALSE) 
	{
		FFATAL("GLFW initialization failed!");
		this->shutdown();
	}
}

void Game::initialize_window()
{

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
	if (this->window == nullptr) 
	{
		FFATAL("OpenGL has failed to create the window.");
		this->shutdown();
	}
}

void Game::initialize_glew() 
{
	glewExperimental = TRUE;
	if (glewInit() != GLEW_OK) 
	{
		FFATAL("Engine was unable to initialize GLEW32!");
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

#ifdef MODE_WIREFRAME
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	FINFO("Wireframe mode enabled!");
#else
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	FINFO("Wireframe mode is disabled.");
#endif

	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(this->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void Game::initialize_matrices() 
{
	FINFO("Initializing matrices...");
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
	FINFO("Initializing shaders...");
	this->shaders.push_back(new Shader(GL_VER_MAJOR, GL_VER_MINOR, "resources/shaders/vertex_core.glsl", "resources/shaders/fragment_core.glsl"));
	this->shaders.push_back(new Shader(GL_VER_MAJOR, GL_VER_MINOR, "resources/shaders/depth_vertex.glsl", "resources/shaders/depth_fragment.glsl", "resources/shaders/depth_geometry.glsl"));
}

void Game::initialize_textures() 
{
	FINFO("Initializing textures...");
	this->textures.push_back(new Texture("resources/textures/container2.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("resources/textures/awesomeface.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("resources/textures/wall.jpg", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("resources/textures/container2_specular.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("resources/textures/lightbulb.jpg", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("resources/textures/wooden_floor.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("resources/textures/skybox.jpg", GL_TEXTURE_2D));
}

void Game::initialize_materials() 
{
	FINFO("Initializing materials...");
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), 0, 1));
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), 0, 1));
}

void Game::initialize_obj_models()
{
	FINFO("Loading 3D Models...");
	// std::vector<Vertex> mesh = loadObjFile("resources/3d/monkey.obj");

	std::vector<Mesh*> meshes;

	// meshes.push_back(new Mesh(mesh.data(), mesh.size(), NULL, 0, glm::vec3(1.0, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));

	//this->models.push_back(new Model(glm::vec3(0.0f, 0.0f, 0.0f),
	//	this->materials[MATERIAL_DEFAULT],
	//	this->textures[TEXTURE_CONTAINER],
	//	this->textures[TEXTURE_CONTAINER],
	//	"resources/3d/test_max.obj"
	//));

	this->models.push_back(new Model(glm::vec3(0.0f, 0.0f, 0.0f),
		this->materials[MATERIAL_DEFAULT],
		this->textures[TEXTURE_LIGHTBULB],
		this->textures[TEXTURE_LIGHTBULB],
		"resources/3d/monkey.obj"
	));

	this->models.push_back(new Model(glm::vec3(0.0f, 2.0f, 0.0f),
		this->materials[MATERIAL_DEFAULT],
		this->textures[TEXTURE_LIGHTBULB],
		this->textures[TEXTURE_LIGHTBULB],
		"resources/3d/monkey.obj"
	));

	this->models.push_back(new Model(glm::vec3(0.0f, 4.0f, 0.0f),
		this->materials[MATERIAL_DEFAULT],
		this->textures[TEXTURE_LIGHTBULB],
		this->textures[TEXTURE_LIGHTBULB],
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
		this->materials[MATERIAL_DEFAULT],
		this->textures[TEXTURE_WOODEN_FLOOR],
		this->textures[TEXTURE_WOODEN_FLOOR],
		meshes
	));

	Model* skybox = new Model(glm::vec3(0.0f),
		this->materials[MATERIAL_LIT],
		this->textures[TEXTURE_SKYBOX],
		this->textures[TEXTURE_SKYBOX],
		"resources/3d/sphere.obj");
	skybox->scale(10);
	this->models.push_back(skybox);

	for (auto*& i : meshes)
		delete i;
	meshes.clear();
}

void Game::initialize_models() 
{

}

void Game::initialize_point_lights()
{
	PointLight* pl = new PointLight(glm::vec3(0.0f), true, 1.0f, glm::vec3(1.0f), 2.0f);

	Model* model = new Model(glm::vec3(0.0f),
		this->materials[MATERIAL_DEFAULT],
		this->textures[TEXTURE_LIGHTBULB],
		this->textures[TEXTURE_CONTAINER],
		"resources/3d/sphere.obj");
	model->scale(0.5f);
	model->rotate(glm::vec3(-180, 0, 0));

	pl->add_child(model);

	this->point_lights.push_back(pl);
}

void Game::initialize_lights() 
{
	FINFO("Initializing lights...");
	initialize_point_lights();
	this->lights.push_back(new glm::vec3(0.0f, 0.0f, 1.0f));
}

void Game::initialize_uniforms() 
{
	FINFO("Initializing uniforms...");
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
	this->shaders[SHADER_CORE_PROGRAM]->set_1f("useShadows", render_shadows);
	for (auto& pl : this->point_lights) 
	{
		pl->send_to_shader(*this->shaders[SHADER_CORE_PROGRAM]);
	}
	//this->shaders[SHADER_CORE_PROGRAM]->setVec3f("lightPos0", *this->lights[0]);

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
