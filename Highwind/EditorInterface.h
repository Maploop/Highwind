#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <vector>

//#include "logger.hpp"
#include "Light.h"
//#include "Game.h"

class EditorInterface {
public:
	EditorInterface();
	~EditorInterface();

	void initialize(GLFWwindow* window);

	void update();
	void render();

	void add_point_light(PointLight* pl);

	struct GraphicsOptions {
		bool castShadows = true;
		bool wireframeMode = false;
	} graphicsOptions;
private:
	// Variables
	std::vector<PointLight*> controllablePointLights;
};

