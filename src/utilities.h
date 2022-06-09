#ifndef UTILITIES
#define UTILITIES

#include "raylib.h"

Vector2 GetScreenCenter();
BoundingBox MakeBoundingBox(Vector3 position, Vector3 size);
Color GetLevelPixelColor(Color* levelMapPixels, int x, int width, int y);
bool CompareColors(Color color1, Color color2);

#endif