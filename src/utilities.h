#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

Vector2 Utilities_GetScreenCenter();
BoundingBox Utilities_MakeBoundingBox(Vector3 position, Vector3 size);
Color Utilities_GetLevelPixelColor(Color* levelMapPixels, int x, int width, int y);
bool Utilities_CompareColors(Color color1, Color color2);

#endif