// Artificial Rage - a Scifi FPS, uses RayLib library
// Made by Akseli
#include "main.h"

// Main loop
int main(void)
{

	// Initialize game
	Game_Initialize();

	// Main game loop
	while (!WindowShouldClose())
	{
		Game_Update();
	}

	// End game
	CloseWindow();
	return 0;
}
