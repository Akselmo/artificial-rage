#include "utilities.h"

Vector2 Utilities_GetScreenCenter()
{
    Vector2 center = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    return center;
}

BoundingBox Utilities_MakeBoundingBox(Vector3 position, Vector3 size)
{
    BoundingBox bb =
        (BoundingBox) {(Vector3) {position.x - size.x / 2, position.y - size.y / 2, position.z - size.z / 2},
                       (Vector3) {position.x + size.x / 2, position.y + size.y / 2, position.z + size.z / 2}};
    return bb;
}

Color Utilities_GetLevelPixelColor(Color* levelMapPixels, int x, int width, int y)
{
    Color pixelColor = {
        levelMapPixels[y * width + x].r, levelMapPixels[y * width + x].g, levelMapPixels[y * width + x].b};

    return pixelColor;
}

bool Utilities_CompareColors(Color color1, Color color2)
{
    if(color1.r == color2.r && color1.g == color2.g && color1.b == color2.b)
    {
        return true;
    }
    else
    {
        return false;
    }
}