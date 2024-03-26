#include "Shader.hpp"
#include "Texture.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 outColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   outColor = aColor;\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 outColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(outColor, 1.0f);\n"
"}\n\0";

#define ENGINE_VERSION = "v0.0.1-alpha";

const int source_width = 800;
const int source_height = 600;

int main() {
	// Starting up glfw
	HINFO("Starting HW Engine %s", "v0.0.1-alpha");
	glfwInit();
	HINFO("GLFW successfully initialized.");

	// Setting gl versions and profiles etc.
	HINFO("Using OpenGL library version 3.3");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	HCOMPLETION("glfwWindowHint (1)");

	// Vertices coordinates
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
	};

	// Create GLFW window:
	HINFO("Creating GLFWwindow...");
	GLFWwindow* window_handle = glfwCreateWindow(source_width, source_height, "HW Engine v0.0.1-alpha", NULL, NULL);
	if (!window_handle) {
		HFAILURE("glfwCreateWindow");
		return -1;
	}
	HCOMPLETION("glfwCreateWindow");
	glfwMakeContextCurrent(window_handle);
	HCOMPLETION("glfwMakeContextCurrent");

	// Load OpenGL itself before calling it's functions
	HINFO("Initializing GLAD...");
	gladLoadGL();
	HCOMPLETION("gladLoadGL");

	// Set the viewport on start
	glViewport(0, 0, source_width, source_height);
	HCOMPLETION("glViewport");

	HINFO("Initializing shaders...");
	// Use the shader class here
	Shader ourShader("shaders/basic_vertex.glsl", "shaders/basic_fragment.glsl");
	HCOMPLETION("SHADER_CREATION");

	HINFO("Loading textures...");
	Texture container("textures/container.jpg");
	HCOMPLETION("TEXUTRE_CREATION");

	HINFO("Generating buffers (debug)...");
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	


	// Setting the screen colour to a navy blue
	glfwSwapBuffers(window_handle);

	HINFO("Starting main loop...");
	while (!glfwWindowShouldClose(window_handle)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use_program();
		container.bind();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window_handle);

		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwDestroyWindow(window_handle);
	glfwTerminate();
	return 0;
}