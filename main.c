//Scifi FPS, uses RayLib library
//Made by Akseli
//Compile: gcc main.c -o game.exe -O1 -Wall -std=c99 -Wno-missing-braces -I include\ -L lib -lraylib -lopengl32 -lgdi32 -lwinmm

#include "include/raylib.h"
#include <stdio.h>
#include "player.h"
#include "settings.h"
#include "level.h"


//Main game loop
int main() 
{
    //Initialize game

    InitializeGame();
    float fov = GetFOV();
    float sensitivity = GetSensitivity();
    Camera camera = PlayerCamera(fov);

    //Random level size
    Vector2 levelSize = (Vector2){ GetRandomValue(20,50), GetRandomValue(20,50)};

    //Show level size in logs
    char str[64];
    float f1 = levelSize.x;
    float f2 = levelSize.y;
    snprintf(str, sizeof(str), "{\"levelSize.x\":\"%.2f\", \"levelSize.y\":\"%.3f\"}", f1, f2);
    TraceLog(LOG_INFO,str);


    SetCameraMode(camera, CAMERA_FIRST_PERSON); 
    

    // Main game loop
    while (!WindowShouldClose())
    {

        UpdateCamera(&camera, 20.0f, sensitivity);

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawLevel(levelSize);

            EndMode3D();

            DrawFPS(10, 10);

        EndDrawing();
    }

    // End game 
    CloseWindow();
    return 0;
}