#pragma once

#include "defines.h"
#include "Camera.h"

// Enable for fun!
// #define MODE_WIREFRAME

// enumerations
enum shader_enum {
	SHADER_CORE_PROGRAM = 0,
	SHADER_DEPTH
};

enum texture_enum {
	TEXTURE_CONTAINER = 0,
	TEXTURE_AWESOMEFACE,
	TEXTURE_WALL,
	SPECULAR_CONTAINER,
	TEXTURE_LIGHTBULB,
	TEXTURE_WOODEN_FLOOR,
	TEXTURE_SKYBOX
};

enum material_enum {
	MATERIAL_DEFAULT = 0,
	MATERIAL_LIT
};

class Game
{
public:
	Game(const char* title, const int width, const int height, int GLmajorVer, int GLminorVer, bool resizable);
	virtual ~Game();

	//Accessors
	int get_window_should_close();

	//Modifiers
	void set_window_should_close();

	//Functions
	void update_delta();
	void update_keyboard_input();
	void update_mouse_input();
	void update_input();
	void update();
	void render();
	void shutdown();

	// Static functions
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
private:
	// Variables
	GLFWwindow* window;
	const char* m_windowTitle;
	bool m_windowResizable;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int frameBufferWidth;
	int frameBufferHeight;
	GLuint m_depthMapFBO;
	
	const int GL_VER_MAJOR;
	const int GL_VER_MINOR;

	float dt;
	float curTime;
	float lastTime;
	
	double last_mouse_x;
	double last_mouse_y;
	double mouse_x;
	double mouse_y;
	double mouse_offset_x;
	double mouse_offset_y;
	bool first_mouse;

	Camera camera;
	EditorInterface editorInterface;

	glm::mat4 viewMatrix;
	glm::vec3 camPosition;
	glm::vec3 worldUp;
	glm::vec3 camFront;
	glm::mat4 projectionMatrix;
	float fov;
	float nearPlane;
	float farPlane;

	// Shaders Meshes materials etc.
	std::vector<Shader*> shaders;
	std::vector<Texture*> textures;
	std::vector<Material*> materials;
	std::vector<Mesh*> meshes;
	std::vector<glm::vec3*> lights;
	std::vector<PointLight*> point_lights;
	std::vector<Model*> models;
	std::vector<ShadowMapHandler*> shadow_maps;

	void lighting_pass();

	// Private functions
	void initialize_glfw();
	void initialize_window();
	void initialize_glew();
	void initialize_opengl_options();
	void initialize_matrices();
	void initialize_shaders();
	void initialize_textures();
	void initialize_materials();
	void initialize_obj_models();
	void initialize_models();
	void initialize_point_lights();
	void initialize_lights();
	void initialize_uniforms();

	void update_uniforms();

	bool is_key_pressed(GLenum key);

	// Static variables
};

