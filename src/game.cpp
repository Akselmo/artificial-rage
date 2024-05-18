#include "game.h"
#include "hud.h"
#include "player.h"
#include "scene.h"
#include "settings.h"

bool Game::isStarted = false;
raylib::Camera Game::camera;
// Initialization
void Game::initialize(void)
{
	Game::isStarted = false;

	// Initialize game
	Settings_Initialize();

	// Add player camera
	Game::camera = Scene_Initialize();

	Game::isStarted = true;
}

// Main game loop
void Game::update(void)
{
	BeginDrawing();

	ClearBackground(BLACK);

	BeginMode3D(Game::camera);

	if (Game::isStarted)
	{
		Player_Update(&Game::camera);
		Scene_Update();
	}

	EndMode3D();

	Game::hudUpdate();
	Game::menuUpdate();

	EndDrawing();
}

// TODO: Move these to their own files
void Game::hudUpdate(void) { Hud_Draw(); }

void Game::menuUpdate(void)
{
	DisableCursor();
	// menu presses etc come here
	// Enable and disable cursor based on if menu is on or off
}
