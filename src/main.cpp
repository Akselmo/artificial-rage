// Artificial Rage - a Scifi FPS, uses RayLib library
// Made by Akseli
#include "game.h"
#include <raylib-cpp.hpp>

// Main loop
int main(void)
{

	// Initialize game
	Game game;

	// Main game loop
	while (!WindowShouldClose())
	{
		game.update();
	}

	// End game
	CloseWindow();
	return 0;
}
