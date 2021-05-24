#include"include/raylib.h"

void InitializeGame(void)
{
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "ScifiFPS");
    SetTargetFPS(60);
}