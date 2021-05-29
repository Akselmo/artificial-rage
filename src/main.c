//Scifi FPS, uses RayLib library
//Made by Akseli
#include <stdio.h>
#include "../include/raylib.h"
#include "player/player.h"
#include "settings/settings.h"

//Global variables
Camera playerCamera;
Vector2 levelSize;

//Initialization
void Initialize(void)
{
    //Initialize game
    InitializeGame();

    //Add player camera
    playerCamera = CustomFPSCamera(4.0f, 4.0f);

    //Random level size
    levelSize = (Vector2){ GetRandomValue(32,128), GetRandomValue(32,128)};

    //Show level size in logs
    char str[64];
    float f1 = levelSize.x;
    float f2 = levelSize.y;
    snprintf(str, sizeof(str), "{\"levelSize.x\":\"%.2f\", \"levelSize.y\":\"%.3f\"}", f1, f2);
    TraceLog(LOG_INFO,str);

}

//Main game loop
int main() 
{

    //Initialize game
    Initialize();

    // Main game loop
    while (!WindowShouldClose())
    {

        UpdateFPSCamera(&playerCamera);

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(playerCamera);

                DrawPlayerHitbox(playerCamera);
                DrawLevel(levelSize);

            EndMode3D();

            DrawFPS(10, 10);

        EndDrawing();
    }

    // End game 
    CloseWindow();
    return 0;
}

