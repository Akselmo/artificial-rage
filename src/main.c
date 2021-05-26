//Scifi FPS, uses RayLib library
//Made by Akseli
#include <stdio.h>

#include "../include/raylib.h"
#include "player/player.h"
#include "settings/settings.h"
//Public variables
Camera camera;
Vector2 levelSize;

//Initialization
void Initialize(void)
{
    //Initialize game
    InitializeGame();
        
    //TODO: Instead of setting keys and other settings here, we could set them when creating the player
    float fov = GetFOV();
    float mouseSensitivity = GetSensitivity();
    int moveFront = GetCustomInput(KEY_W);
    int moveBack = GetCustomInput(KEY_S);
    int moveRight = GetCustomInput(KEY_D);
    int moveLeft = GetCustomInput(KEY_A);
    int fireGun = MOUSE_LEFT_BUTTON; //etc etc

    //Firing will happen in different function so no firing key needed here
    camera = CustomFPSCamera(fov, 4.0f, 4.0f, moveFront, moveBack, moveRight, moveLeft, mouseSensitivity);

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

        UpdateFPSCamera(&camera);

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

