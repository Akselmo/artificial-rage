// Scifi FPS, uses RayLib library
// Made by Akseli
#include "main.h"
#include "enemy.h"
#include "hud.h"
#include "include/raylib.h"
#include "item.h"
#include "level.h"
#include "player.h"
#include "settings.h"
#include "utilities.h"
#include "weapon.h"
#include <stdio.h>
#include <stdlib.h>

// Global variables

Camera playerCamera;
Vector2 levelSize;
Model* levelModel;
// Prototypes
void HudUpdate();
void MenuUpdate();

// Initialization
void Initialize(void)
{
    // Initialize game
    InitializeGame();

    BuildLevel();
    // Add player camera
    Vector3 startPos = GetLevelStartPosition();
    playerCamera = CustomFPSCamera(startPos.x, startPos.z);
}

// Main game loop
void GameUpdate()
{
    BeginDrawing();

    ClearBackground(BLACK);

    BeginMode3D(playerCamera);

    UpdateFPSCamera(&playerCamera);
    PlayerFire(&playerCamera);
    DrawLevel();

    EndMode3D();

    HudUpdate();
    MenuUpdate();

    EndDrawing();
}

void HudUpdate()
{
    // TODO: Get current weapon and its ammo instead of whatever mess this is
    DrawPlayerHud(GetPlayerData().playerHealth, GetWeaponData().currentWeaponAmmo, 0);
}

void MenuUpdate()
{
    // menu presses etc come here
}

// Main loop
int main()
{

    // Initialize game
    Initialize();

    // Main game loop
    while(!WindowShouldClose())
    {
        GameUpdate();
    }

    // End game
    CloseWindow();
    return 0;
}
