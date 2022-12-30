#include "utilities.h"

Vector2 Utilities_GetScreenCenter()
{
    const Vector2 center = { GetScreenWidth() / 2, GetScreenHeight() / 2 };
    return center;
}

BoundingBox Utilities_MakeBoundingBox(const Vector3 position, const Vector3 size)
{
    BoundingBox bb = (BoundingBox) {
        (Vector3) {position.x - size.x / 2,  position.y - size.y / 2, position.z - size.z / 2},
        (Vector3) { position.x + size.x / 2, position.y + size.y / 2, position.z + size.z / 2}
    };
    return bb;
}

Color Utilities_GetLevelPixelColor(const Color* levelMapPixels,
                                   const int x,
                                   const int width,
                                   const int y)
{
    Color pixelColor = { levelMapPixels[y * width + x].r,
                         levelMapPixels[y * width + x].g,
                         levelMapPixels[y * width + x].b };

    return pixelColor;
}

bool Utilities_CompareColors(const Color color1, const Color color2)
{
    return (color1.r == color2.r && color1.g == color2.g && color1.b == color2.b);
}
