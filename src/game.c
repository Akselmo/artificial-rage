#include "game.h"
#include "hud.h"
#include "player.h"
#include "raylib.h"
#include "scene.h"
#include "settings.h"

// Shared variables
Camera Game_camera;

// Prototypes
void Game_HudUpdate(void);
void Game_MenuUpdate(void);
bool Game_isStarted = false;

// Initialization
void Game_Initialize(void)
{
	Game_isStarted = false;

	// Initialize game
	Settings_Initialize();

	// Add player camera
	Game_camera = Scene_Initialize();

	Game_isStarted = true;
}

// Main game loop
void Game_Update(void)
{
	BeginDrawing();

	ClearBackground(BLACK);

	BeginMode3D(Game_camera);

	if (Game_isStarted)
	{
		Player_Update(&Game_camera);
		Scene_Update();
	}

	EndMode3D();

	Game_HudUpdate();
	Game_MenuUpdate();

	EndDrawing();
}

// TODO: Move these to their own files
void Game_HudUpdate(void) { Hud_Draw(); }

void Game_MenuUpdate(void)
{
	DisableCursor();
	// menu presses etc come here
	// Enable and disable cursor based on if menu is on or off
}
