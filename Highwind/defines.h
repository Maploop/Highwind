#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <SOIL2.h>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define g_int32 GLint
#define g_uint32 GLuint
#define g_char GLchar
#define glvoid GLvoid
#define TRUE GL_TRUE
#define FALSE GL_FALSE

struct Vertex {
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texcoord;
};