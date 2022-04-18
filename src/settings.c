#include "settings.h"
#include "include/raylib.h"

void InitializeGame(void)
{
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Artificial Rage");
    // SetTargetFPS(60);
}

float GetSensitivity(void)
{
    // Get sensitivity from config file
    // 0.003f is default
    return 0.003f;
}

float GetFOV(void)
{
    // Get FOV from config file
    return 60.0f;
}

// TODO: instead of reading keys from initialization, read a file instead
// Then we dont have to feed parameters
int GetCustomInput(int key)
{
    return key;
}