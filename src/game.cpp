module;
#include "hud.h"
#include "player.h"
#include "raylib.h"
#include "scene.h"
#include "settings.h"
#include <stdbool.h>
export module Game;

export class Game
{
  public:
	// Shared variables
	Camera camera;

	// Prototypes
	bool isStarted = false;

	// Initialization
	void Initialize(void)
	{
		isStarted = false;

		// Initialize game
		Settings_Initialize();

		// Add player camera
		camera = Scene_Initialize();

		isStarted = true;
	}

	// Main game loop
	void Update(void)
	{
		BeginDrawing();

		ClearBackground(BLACK);

		BeginMode3D(camera);

		if (isStarted)
		{
			Player_Update(&camera);
			Scene_Update();
		}

		EndMode3D();

		HudUpdate();
		MenuUpdate();

		EndDrawing();
	}

	// TODO: Move these to their own files
	void HudUpdate(void) { Hud_Draw(); }

	void MenuUpdate(void)
	{
		DisableCursor();
		// menu presses etc come here
		// Enable and disable cursor based on if menu is on or off
	}
};
