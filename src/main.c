//Scifi FPS, uses RayLib library
//Made by Akseli
#include <stdio.h>
#include <stdlib.h>
#include "../include/raylib.h"
#include "player/player.h"
#include "settings/settings.h"
#include "level/level.h"
#include "enemy/enemy.h"
#include "main.h"

//Global variables

Camera playerCamera;
Vector2 levelSize;
Model *levelModel;
//Should be enough room for entities, such as clutter, weapon pickups, etc...
Model allEntities[MAX_ENTITIES];

//Initialization
void Initialize(void)
{
    //Initialize game
    InitializeGame();

    BuildLevel();
    //Add enemies

    //Add all models in the level to allModels
    //allModels[0] = levelModel;
    //Foreach every model in enemy model array

    //Add player camera
    Vector3 startPos = GetLevelStartPosition();
    playerCamera = CustomFPSCamera(startPos.x, startPos.z);
}

//Main game loop
void GameUpdate()
{
    BeginDrawing();

    ClearBackground(BLACK);

    BeginMode3D(playerCamera);

    UpdateFPSCamera(&playerCamera);
    PlayerFire(&playerCamera);
    DrawLevel();

    EndMode3D();

    DrawFPS(10, 10);

    EndDrawing();
}

//Main loop
int main()
{

    //Initialize game
    Initialize();

    // Main game loop
    while (!WindowShouldClose())
    {
        GameUpdate();
    }

    // End game
    CloseWindow();
    return 0;
}

Model *GetAllEntities()
{
    return allEntities;
}
