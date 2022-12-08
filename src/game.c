//
// Created by akseli on 12/06/22.
//

#include "game.h"

// Shared variables
Camera playerCamera;

// Prototypes
void Game_HudUpdate();
void Game_MenuUpdate();
bool Game_isStarted = false;

// Initialization
void Game_Initialize()
{
    Game_isStarted = false;

    // Initialize game
    Settings_Initialize();

    // Add player camera
    playerCamera = Scene_Initialize();

    Game_isStarted = true;
}

// Main game loop
void Game_Update()
{
    BeginDrawing();

    ClearBackground(BLACK);

    BeginMode3D(playerCamera);

    if(Game_isStarted)
    {
        Player_Update(&playerCamera);
        Scene_Update();
    }

    EndMode3D();

    Game_HudUpdate();
    Game_MenuUpdate();

    EndDrawing();
}

// TODO: Move these to their own files
void Game_HudUpdate()
{
    Hud_Draw();
}

void Game_MenuUpdate()
{
    DisableCursor();
    // menu presses etc come here
    // Enable and disable cursor based on if menu is on or off
}