#include"../../include/raylib.h"

void DrawLevel(Vector2 size)
{
    //Draw plane in center of map
    //TODO: Change color to a tiling texture
    DrawPlane((Vector3){0.0f,0.0f,0.0f}, size, DARKGRAY);
    //Create walls 
    //Vector3: (X, Y, Z)
    float xHalfSize = size.x/2;
    float yHalfSize = size.y/2;
    DrawCube((Vector3){ -xHalfSize, 2.5f, 0.0f }, 1.0f, 5.0f, size.y, BLUE);
    DrawCube((Vector3){ xHalfSize, 2.5f, 0.0f }, 1.0f, 5.0f, size.y, LIME);
    DrawCube((Vector3){ 0.0f, 2.5f, yHalfSize}, size.x, 5.0f, 1.0f, GOLD); 
    DrawCube((Vector3){ 0.0f, 2.5f, -yHalfSize}, size.x, 5.0f, 1.0f, PURPLE);
}