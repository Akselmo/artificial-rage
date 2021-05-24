//Scifi FPS, uses RayLib library
//Made by Akseli
//Compile: gcc main.c -o game.exe -O1 -Wall -std=c99 -Wno-missing-braces -I include\ -L lib -lraylib -lopengl32 -lgdi32 -lwinmm

#include "include/raylib.h"
#include "player.h"
#include "settings.h"
#include "level.h"

//Main game loop
int main() 
{
    //Initialize game

    InitializeGame();
    Camera camera = PlayerCamera();

    //Random level size
    Vector2 levelSize = (Vector2){ GetRandomValue(20,50), GetRandomValue(20,50)};
    
    // Main game loop
    while (!WindowShouldClose())
    {

        UpdateCamera(&camera);

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