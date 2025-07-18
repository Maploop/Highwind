#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
#include <map>

#include "Model.h"
#include "GLTFModel.h"
#include "Light.h"
#include "Component.h"
#include "EditorInterface.h"
#include "ShadowMapHandler.h"
#include "GraphicsOptions.h"
#include "SkyBoxHandler.h"

#define g_int32 GLint
#define g_uint32 GLuint
#define g_char GLchar
#define glvoid GLvoid
#define TRUE GL_TRUE
#define FALSE GL_FALSE

static bool ends_with(std::string_view str, std::string_view suffix) {
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

static bool starts_with(std::string_view str, std::string_view prefix) {
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}
