#pragma once

#include "defines.h"
#include "Camera.h"

// Enable for fun!
// #define MODE_WIREFRAME

// enumerations
enum shader_enum {
	SHADER_CORE_PROGRAM = 0
};

enum texture_enum {
	TEX_CONTAINER_0 = 0,
	TEX_AWESOMEFACE_1
};

enum material_enum {
	MAT_1 = 0
};

enum mesh_enum {
	MESH_QUAD = 0
};

class Game
{
public:
	Game(const char* title, const int width, const int height, int GLmajorVer, int GLminorVer, bool resizable);
	virtual ~Game();

	//Accessors
	int getWindowShouldClose();

	//Modifiers
	void setWindowShouldClose();

	//Functions
	void updateDelta();
	void updateKeyboardInput();
	void updateMouseInput();
	void updateInput();
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
	
	const int GL_VER_MAJOR;
	const int GL_VER_MINOR;

	float dt;
	float curTime;
	float lastTime;
	
	double lastMouseX;
	double lastMouseY;
	double mouseX;
	double mouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse;

	Camera camera;

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
	std::vector<Model*> models;

	// Private functions
	void initGlfw();
	void initWindow();
	void initGlew();
	void initOpenGLOptions();
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initModels();
	void initLights();
	void initUniforms();

	void updateUniforms();

	bool isKeyPressed(GLenum key);

	// Static variables
};

