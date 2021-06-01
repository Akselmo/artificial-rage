//Scifi FPS, uses RayLib library
//Made by Akseli
#include <stdio.h>
#include <stdlib.h>
#include "../include/raylib.h"
#include "player/player.h"
#include "settings/settings.h"
#include "level/level.h"



//Global variables

Camera playerCamera;
Vector2 levelSize;
Model levelModel;
//Should be enough room for models...
Model allModels[128];

//Initialization
void Initialize(void)
{
    //Initialize game
    InitializeGame();

    levelModel = BuildLevel();
    //Add enemies

    //Add all models in the level to allModels
    allModels[0] = levelModel;
    //Foreach every model in enemy model array


    //Add player camera
    Vector3 startPos = GetStartPosition();
    playerCamera = CustomFPSCamera(startPos.x, startPos.z);

}

//Main game loop
int main() 
{

    //Initialize game
    Initialize();

    // Main game loop
    while (!WindowShouldClose())
    {
        Vector3 oldPlayerPos = playerCamera.position;
        UpdateFPSCamera(&playerCamera);
        if (CheckLevelCollision((Vector2){playerCamera.position.x, playerCamera.position.z},0.1f))
        {
            playerCamera.position = oldPlayerPos;
        }

        BeginDrawing();

            ClearBackground(BLACK);

            BeginMode3D(playerCamera);
                
                DrawLevel(levelModel);
                DrawPlayerAim(&playerCamera);
                
            EndMode3D();

            DrawFPS(10, 10);

        EndDrawing();
    }

    // End game 
    CloseWindow();
    return 0;
}

Model* GetAllModels()
{
    return allModels;
}