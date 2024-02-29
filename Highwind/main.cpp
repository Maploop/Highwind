#include "Game.h"
// #include "logger.hpp"

//#define MODE_WIREFRAME

const int WIDTH = 640;
const int HEIGHT = 480;

const int gl_major_version = 4;
const int gl_minor_version = 5;

int main() {
	Game game("FR Engine CodeENV0", 1280, 720, gl_major_version, gl_minor_version, true);

	while (!game.getWindowShouldClose()) {
		game.update();
		game.render();

	}

	return 0;
}