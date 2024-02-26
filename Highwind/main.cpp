#include "defines.h"
#include "logger.hpp"

//#define MODE_WIREFRAME

const int WIDTH = 640;
const int HEIGHT = 480;

void update_input(GLFWwindow* window);
void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
bool load_shaders(g_uint32 &program);
g_uint32 generate_texture(const char* path);

Vertex vertices[] = {
	// positins						// colors						// texcoords
	glm::vec3(-0.5f, 0.5f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f),
	glm::vec3(-0.5f, -0.5f, 0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),
	glm::vec3(0.5f, -0.5f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 0.0f),

	glm::vec3(0.5f, 0.5f, 0.0f),	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 1.0f)
};

unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

g_uint32 indices[] = {
	0, 1, 2, // tri #1
	0, 2, 3, // tri #2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(g_uint32);

int main() {
	glfwInit();

	int framebufferWidth = 0;
	int framebufferHeight = 0;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);


	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "FR Engine Primitive", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

	glfwMakeContextCurrent(window);

	glewExperimental = TRUE;
	if (glewInit() != GLEW_OK) {
		FFATAL("Engine was unable to initialize GLEW32!");
		glfwTerminate();
	}

	// opengl settings
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

	g_uint32 core_program;
	if (!load_shaders(core_program)) {
		FFATAL("Failed to load shader programs, application will crash");
		glfwTerminate();
		return -1;
	}

	g_uint32 VAO;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	g_uint32 VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	g_uint32 EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// g_uint32 attribLoc = glGetAttribLocation(core_program, "vertex_position");
	// position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, TRUE, sizeof(Vertex), (glvoid*) offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	// color attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, TRUE, sizeof(Vertex), (glvoid*) offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
	// texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, TRUE, sizeof(Vertex), (glvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);


	// texture init
	g_uint32 texture0 = generate_texture("resources/textures/container2.png");
	g_uint32 texture1 = generate_texture("resources/textures/awesomeface.png");

	glm::mat4 modelMatrix(1.0f);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		update_input(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// use program
		glUseProgram(core_program);

		// update uniforms
		glUniform1i(glGetUniformLocation(core_program, "texture0"), 0);
		glUniform1i(glGetUniformLocation(core_program, "texture1"), 1);

		// move, rotate, scale etc..
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));
		glUniformMatrix4fv(glGetUniformLocation(core_program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

		// activate texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glfwTerminate();
	glDeleteProgram(core_program);
	return 0;
}

void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}

void update_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

bool load_shaders(g_uint32 &program) {
	bool codeSuccess = true;
	char infoLog[512];
	g_int32 success;

	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;
	// vertex shader
	in_file.open("vertex_core.glsl");
	if (in_file.fail()) {
		FFATAL("Failed to open vertex_core file.");
		return false;
	}
	if (in_file.is_open()) {
		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else {
		FERROR("LOADSHADER::COULD_NOT_OPEN_VERTEX_FILE");
		codeSuccess = false;
	}

	in_file.close();
	g_uint32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const g_char* vertSrc = src.c_str();
	
	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		codeSuccess = false;
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		FERROR("LOADSHADER::COULD_NOT_COMPILE_VERTEX_SHADER");
		FERROR(infoLog);
	}
	else
		FINFO("Successfully compiled VERTEX shader");

	temp = "";
	src = "";

	// Fragment shader
	in_file.open("fragment_core.glsl");
	if (in_file.is_open()) {
		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else
		FERROR("LOADSHADER::COULD_NOT_OPEN_FRAGMENT_FILE");

	in_file.close();
	g_uint32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const g_char* fragSrc = src.c_str();

	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		codeSuccess = false;
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		FERROR("LOADSHADER::COULD_NOT_COMPILE_FRAGMENT_SHADER");
		FERROR(infoLog);
	}
	else
		FINFO("Successfully compiled FRAGMENT shader");

	// Program
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		codeSuccess = false;
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		FERROR("Failed to link shader program! Stacktrace:");
		FERROR(infoLog);
	} 
	else
		FINFO("Successfully linked shader programs!");

	// Delete shaders
	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return codeSuccess;
}

g_uint32 generate_texture(const char* path) {
	int image_width = 0;
	int image_height = 0;
	unsigned char* image = SOIL_load_image(path, &image_width, &image_height, NULL, SOIL_LOAD_RGBA);
	g_uint32 texture0;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		FERROR("Error while loading texture!");
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	return texture0;
}