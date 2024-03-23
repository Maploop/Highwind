#include "logger.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define ENGINE_VERSION = "v0.0.1-alpha";

int main() {
	// Starting up glfw
	HINFO("Starting HW Engine %s", "v0.0.1-alpha");
	glfwInit();
	HINFO("GLFW successfully initialized.");

	// Setting gl versions and profiles etc.
	HINFO("Using OpenGL library version 3.3:");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	HTRACE("glfwWindowHint (1) - Completion status: 1");

	// Create GLFW window:
	HINFO("Creating GLFWwindow...");
	GLFWwindow* window = glfwCreateWindow(800, 600, "HW Engine v0.0.1-alpha", GLFW_FALSE, GLFW_FALSE);
	

	glfwTerminate();
	return 0;
}