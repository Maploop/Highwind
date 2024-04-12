#include "Game.h"
// #include "logger.hpp"

//#define MODE_WIREFRAME

const int WIDTH = 640;
const int HEIGHT = 480;

const int gl_major_version = 4;
const int gl_minor_version = 5;

int main() 
{
	Game game("FR Engine CodeENV0", 1280, 720, gl_major_version, gl_minor_version, true);

	// HNetworkingAgent communicationAgent;
	// communicationAgent.start();

	while (!game.get_window_should_close()) 
	{
		game.update();
		game.render();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return 0;
}